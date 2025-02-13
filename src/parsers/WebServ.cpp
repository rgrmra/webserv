#include "Connection.hpp"
#include "header.hpp"
#include "Http.hpp"
#include "logger.hpp"
#include "response.hpp"
#include "WebServ.hpp"
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

using namespace std;

WebServ::WebServ(Http *http)
	: _http(http),
	  _epoll_fd(-1) {

	vector<Server> servers = _http->getServers();
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {

		vector<string> listens = it->getListen();
		vector<string>::iterator itl = listens.begin();
		for (; itl != listens.end(); itl++) {

			if (isBinded(*itl))
				continue;

			_binded_sockets[*itl] = createSocket(*itl);
		}
	}
}

WebServ::WebServ(const WebServ &src) {

	*this = src;
}

WebServ &WebServ::operator=(const WebServ &rhs) {

	if (this == &rhs)
		return *this;

	_http = rhs._http;

	return *this;
}

WebServ::~WebServ(void) {

	if (_epoll_fd != -1)
		close(_epoll_fd);

	while (_client_connections.begin() != _client_connections.end()) {
		map<int, Connection *>::iterator ic = _client_connections.begin();

		response::pageInternalServerError(ic->second);

		if (sendMessage(ic->second, ic->second->getResponse()) == -1)
			continue;
			
		closeConnection(ic->first);
	}

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++) {
		close(it->second);
	}
}

void WebServ::removeBindedPorts(string port) {

	set<string> remove;

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++) {

		list<string> tmp2 = parser::split(it->first, ':');
		if (port != tmp2.back())
			continue;

		close(it->second);
		remove.insert(it->first);
	}

	for(set<string>::iterator it = remove.begin(); it != remove.end(); it++)
		_binded_sockets.erase(_binded_sockets.find(*it));
}

bool WebServ::isBinded(string host) {
	
	if (_binded_sockets.find(host) != _binded_sockets.end())
		return true;

	list<string> tmp = parser::split(host, ':');
	if (_binded_sockets.find("0.0.0.0:" + tmp.back()) != _binded_sockets.end())
		return true;

	if (tmp.front() != "0.0.0.0")
		return false;

	removeBindedPorts(tmp.back());

	return false;
}

struct addrinfo *WebServ::getAddrInfo(string host) {

	struct addrinfo hints = (struct addrinfo){};
	struct addrinfo *res = NULL;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	list<string> tmp = parser::split(host, ':');
	if (getaddrinfo(tmp.front().c_str(), tmp.back().c_str(), &hints, &res))
		throw runtime_error("getaddrinfo failed");

	return res;
}

int WebServ::createSocket(string host) {

	struct addrinfo *res = getAddrInfo(host);

	int fd = socket(res->ai_family, res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol);
	if (fd < 0)
		throw runtime_error("socket failed");

	struct linger opt = (struct linger){};
	opt.l_onoff = 1;
	opt.l_linger = 10;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		(close(fd), freeaddrinfo(res));
		throw runtime_error("setsockopt failed");
	}

	if (bind(fd, res->ai_addr, res->ai_addrlen) != 0) {
		(close(fd), freeaddrinfo(res));
		throw runtime_error("failed to bind on " + host);
	}

	freeaddrinfo(res);

	if (listen(fd, WebServ::MAX_EVENTS) == -1) {
		close(fd);
		throw runtime_error("listen failed");
	}

	return fd;
}

void WebServ::controlEpoll(int client_fd, int flag, int option) {

	struct epoll_event event = {};
	event.events = flag;
	event.data.fd = client_fd;

	if (epoll_ctl(_epoll_fd, option, client_fd, &event) == -1)
		logger::fatal("epoll_ctl failed");
}

string WebServ::getIpByFileDescriptor(int client_fd) {

	sockaddr_in local_addr;
	socklen_t addr_len = sizeof(local_addr);

	if (getsockname(client_fd, (sockaddr *) &local_addr, &addr_len) == -1)
		throw runtime_error("getsockname failed");

	int ip = htonl(local_addr.sin_addr.s_addr);
	unsigned short port = htons(local_addr.sin_port);

	stringstream ss;

	ss << ((ip & 0xFF000000) >> 24) << ".";
	ss << ((ip & 0x00FF0000) >> 16) << ".";
	ss << ((ip & 0x0000FF00) >> 8) << ".";
	ss << (ip & 0x000000FF);

	return ss.str() + ":" + parser::toString(port);
}

void WebServ::acceptNewConnection(int client_fd) {

	int fd = accept(client_fd, NULL, NULL);
	if (fd == -1) {
		if (not (errno == EAGAIN || errno == EWOULDBLOCK))
			logger::fatal("accept failed");
		return;
	}

	string host = getIpByFileDescriptor(fd);

	logger::debug(host + " connection accepted");
	
	controlEpoll(fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);

	_client_connections[fd] = new Connection(fd, host);
}

void WebServ::closeConnection(int client_fd) {

	map<int, Connection *>::iterator it = _client_connections.find(client_fd);

	if (it == _client_connections.end())
		return;

	logger::debug(it->second->getIp() + " connection closed");

	close(it->first);
	delete it->second;
	_client_connections.erase(it);
}

void WebServ::handleRequest(int client_fd) {

	map<int, Connection *>::iterator it = _client_connections.find(client_fd);
	Connection *connection = it->second;

	vector<char> buffer(BUFFER_SIZE);
	int bytes_read = recv(client_fd, buffer.data(), BUFFER_SIZE, MSG_NOSIGNAL);
	if (bytes_read == -1) {
		logger::fatal("recv");
		return closeConnection(client_fd);
	} else if (bytes_read == 0 || buffer.at(0) == EOF) {
		logger::warning(connection->getIp() + " disconected");
		return closeConnection(client_fd);
	}

	connection->append(buffer, bytes_read);

	if (connection->getSend() == false)
		return controlEpoll(client_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);

	controlEpoll(client_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
}

int WebServ::sendMessage(Connection *connection, std::string message) {

	int status = send(connection->getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
	if (status == -1) {
		logger::fatal("client is no longer available to receive messages");
		closeConnection(connection->getFd());
	}

	return status;
}

void WebServ::handleResponse(int client_fd) {

	map<int, Connection *>::iterator it = _client_connections.find(client_fd);
	Connection *connection = it->second;

	if (sendMessage(it->second, connection->getResponse(BUFFER_SIZE)) == -1)
		return;

	if (connection->getResponseSize())
		return controlEpoll(client_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
	else if (connection->getHeaderByKey(header::CONNECTION) == "keep-alive") {
		connection->resetConnection();
		return controlEpoll(client_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);
	}

	closeConnection(client_fd);
}

int WebServ::isBindedSocket(int fd) {

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++)
		if (fd == it->second)
			return true;

	return false;
}

bool WebServ::isTimedOut(int client_fd) {

	map<int, Connection *>::iterator it = _client_connections.find(client_fd);
	Connection *connection = it->second;

	if (it == _client_connections.end())
		return false;

	if (connection->getTransfers() && time(NULL) - connection->getTime() >= KEEP_ALIVE) {
		closeConnection(client_fd);
		return true;
	}

	if (time(NULL) - connection->getTime() <= WebServ::TIMEOUT)
		return false;

	response::pageGatewayTimeOut(connection);

	controlEpoll(client_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);

	return true;
}

void WebServ::run(void) {

	_epoll_fd = epoll_create(1);
	if (_epoll_fd < 0)
		throw runtime_error("epoll failed");

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++) {
		controlEpoll(it->second, EPOLLIN, EPOLL_CTL_ADD);
		logger::debug("server started, listening on " + it->first);
	}

	epoll_event events[MAX_EVENTS];

	while (true) {
		int num_events = epoll_wait(_epoll_fd, events, MAX_EVENTS, 0);
		if (num_events == -1)
			throw runtime_error("epoll_wait failed");

		for (int i = 0; i < num_events; i++) {
			if (isBindedSocket(events[i].data.fd))
				acceptNewConnection(events[i].data.fd);
			else if (events[i].events & (EPOLLIN | EPOLLET))
				handleRequest(events[i].data.fd);
			else if (events[i].events & (EPOLLOUT| EPOLLET))
				handleResponse(events[i].data.fd);
		}
		map<int, Connection *>::iterator ite = _client_connections.begin();
		for (; ite != _client_connections.end(); ite++)
			if (isTimedOut(ite->first))
				break;
	}
}
