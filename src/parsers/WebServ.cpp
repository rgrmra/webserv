#include "Connection.hpp"
#include "Http.hpp"
#include "Server.hpp"
#include "directive.hpp"
#include "logger.hpp"
#include "WebServ.hpp"
#include "parser.hpp"
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
#include <cerrno>

using namespace std;

void sigint(int signal);

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
		throw runtime_error("getaddrinfo");

	return res;
}

int WebServ::createSocket(string host) {

	struct addrinfo *res = getAddrInfo(host);

	int fd = socket(res->ai_family, res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol);
	if (fd < 0)
		throw runtime_error("socket");

	int opt = 0;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		(close(fd), freeaddrinfo(res));
		throw runtime_error("setsockopt");
	}

	if (bind(fd, res->ai_addr, res->ai_addrlen) != 0) {
		(close(fd), freeaddrinfo(res));
		throw runtime_error("failed to bind on " + host);
	}

	freeaddrinfo(res);

	if (listen(fd, WebServ::MAX_EVENTS) == -1) {
		close(fd);
		throw runtime_error("listen");
	}

	return fd;
}

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

	for (map<string, int>::iterator it = _binded_sockets.begin(); it != _binded_sockets.end(); it++)
		cout << "host: " << it->first << ", fd: " << it->second << endl;
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

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++) {
		close(it->second);
	}

	map<int, Connection *>::iterator ic = _client_connections.begin();
	for (; ic != _client_connections.end(); ic++) {
		close(ic->first);
		delete ic->second;
	}
}

void WebServ::controlEpoll(int client_fd, int flag, int option) {

	struct epoll_event event = {};
	event.events = flag;
	event.data.fd = client_fd;

	if (epoll_ctl(_epoll_fd, option, client_fd, &event) == -1)
		logger::fatal("epoll_ctl");
}

void WebServ::acceptNewConnection(int client_fd) {

	int fd = accept(client_fd, NULL, NULL);
	if (fd == -1) {
		if (not (errno == EAGAIN || errno == EWOULDBLOCK))
			logger::fatal("accept");
		return;
	}
	
	cout << fd << endl;

	controlEpoll(fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);

	_client_connections[fd] = new Connection(fd, getIpByFileDescriptor(fd));
}

void WebServ::closeConnection(int client_fd) {

	map<int, Connection *>::iterator it = _client_connections.find(client_fd);

	if (it == _client_connections.end())
		return;

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
	} else if (bytes_read == 0) {
		logger::warning("client disconected: " + connection->getIp());
		return closeConnection(client_fd);
	}

	if (bytes_read > 0)
		connection->append(buffer, bytes_read);

	if (connection->getBuffer().find("\r\n\r\n") == string::npos)
		return controlEpoll(client_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);

	logger::info("received: ");
	cout << connection->getBuffer() << endl;
	
	controlEpoll(client_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
}

string WebServ::getIpByFileDescriptor(int client_fd) {

	sockaddr_in local_addr;
	socklen_t addr_len = sizeof(local_addr);

	if (getsockname(client_fd, (sockaddr *) &local_addr, &addr_len) == -1)
		throw runtime_error("getsockname");

	int ip = htonl(local_addr.sin_addr.s_addr);
	unsigned short port = htons(local_addr.sin_port);

	stringstream ss;

	ss << ((ip & 0xFF000000) >> 24) << ".";
	ss << ((ip & 0x00FF0000) >> 16) << ".";
	ss << ((ip & 0x0000FF00) >> 8) << ".";
	ss << (ip & 0x000000FF);

	return ss.str() + ":" + parser::toString(port);
}

void WebServ::handleResponse(int client_fd) {

	map<int, Connection *>::iterator it = _client_connections.find(client_fd);
	Connection *connection = it->second;

	string tmp = connection->getResponse(BUFFER_SIZE);

	if (send(client_fd, tmp.c_str(), tmp.size(), MSG_NOSIGNAL) == -1) {
		logger::fatal("client is no longer available to receive messages");
		return closeConnection(client_fd);
	}

	if (connection->getResponseSize())
		return controlEpoll(client_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);

	cout << *_client_connections.find(client_fd)->second << endl;

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
	if (it == _client_connections.end()) {
		controlEpoll(client_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);
		return false;
	}

	if (time(NULL) - it->second->getTime() <= WebServ::TIMEOUT) {
		controlEpoll(client_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);
		return false;
	}

	logger::warning("client timed out: " + it->second->getIp());
	string response = "HTTP/1.1 504 Gateway Timeout\r\nConnection: closed\r\n\r\n";

	if (send(client_fd, response.c_str(), response.size(), MSG_NOSIGNAL) == -1)
		logger::fatal("send");

	closeConnection(client_fd);

	return true;
}

void WebServ::run(void) {

	_epoll_fd = epoll_create(1);
	if (_epoll_fd < 0)
		throw runtime_error("epoll");

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++)
		controlEpoll(it->second, EPOLLIN, EPOLL_CTL_ADD);

	epoll_event events[MAX_EVENTS];

	while (true) {

		int num_events = epoll_wait(_epoll_fd, events, MAX_EVENTS, 0);
		if (num_events == -1)
			throw runtime_error("epoll_wait");

		for (int i = 0; i < num_events; i++) {
			cout << events[i].data.fd << endl;
			if (isBindedSocket(events[i].data.fd))
				acceptNewConnection(events[i].data.fd);
			else if (events[i].events & (EPOLLIN| EPOLLET))
				handleRequest(events[i].data.fd);
			else if (events[i].events & (EPOLLOUT| EPOLLET))
				handleResponse(events[i].data.fd);
			if (isTimedOut(events[i].data.fd))
				continue;
		}
	}
}
