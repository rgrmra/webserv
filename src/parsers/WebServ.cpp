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

#define TIMEOUT 6

using namespace std;

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

addrinfo *WebServ::getAddrInfo(string host) {

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
		freeaddrinfo(res);
		throw runtime_error("setsockopt");
	}

	if (bind(fd, res->ai_addr, res->ai_addrlen) != 0) {
		freeaddrinfo(res);
		throw runtime_error("bind");
	}

	freeaddrinfo(res);

	if (listen(fd, WebServ::MAX_EVENTS) == -1)
		throw runtime_error("listen");

	return fd;
}

WebServ::WebServ(Http *http)
	: _http(http) {

	vector<Server> servers = http->getServers();
	vector<Server>::iterator it = servers.begin();
	for (; it != servers.end(); it++) {

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

	close(epoll_fd);

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++)
		close(it->second);

}

void WebServ::acceptNewConnection(int client_fd) {

	int fd = accept(client_fd, NULL, NULL);
	if (fd == -1) {
		if (not (errno == EAGAIN || errno == EWOULDBLOCK))
			logger::fatal("accept");
		return;
	}

	epoll_event event = {};
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
		logger::fatal("epoll_ctl");
}

std::string hostname;

void WebServ::handle_client_request(int epoll_fd, int client_fd) {

	char buffer[8192];
	int bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
	if (bytes_read == -1)
		return (close(client_fd), logger::fatal("recv"));

	if (bytes_read == 0)
		return;
		//return (close(client_fd), logger::warning("client disconected"));

	buffer[bytes_read] = '\0';
	logger::info("received: ");
	cout << buffer << endl;
	
	string nbuffer = buffer;
	size_t pos = nbuffer.find("\r\nHost: ");
	if (pos != string::npos) {
		hostname = nbuffer.substr(pos + 2, nbuffer.size() - pos - 2);
		hostname = parser::find("Host: ", hostname, "\r\n");
		cout << "hostname: " + hostname << endl;
	}

	epoll_event event = {};
	event.events = EPOLLOUT | EPOLLET;
	event.data.fd = client_fd;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &event) == -1) {
		logger::fatal("epoll_ctl");
		close(client_fd);
	}
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

std::vector<char> readGIF(const std::string& filename) {
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file) {
		std::cerr << "Error opening file: " << filename << std::endl;
		//return std::vector<char>();
	}

	// Seek to the end to get size
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	// Read contents
	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		std::cerr << "Error reading file: " << filename << std::endl;
		return std::vector<char>();
	}

	return buffer;
}

void WebServ::handle_client_response(int client_fd) {

	cout << getIpByFileDescriptor(client_fd);

	//Server it;
	//if (directive::validateHttpListen(hostname))
	//	it = _http->getServerByListen(getIpByFileDescriptor(client_fd));
	//else {
	//	list<string> tmp = parser::split(hostname, ':');
	//	if (tmp.empty())
	//		return;
	//	it = _http->getServerByName(tmp.front());
	//	hostname.clear();
	//}

	//cout << it.getMaxBodySize() << endl;

	//vector<char> gif = readGIF("./teste.gif");

	//ostringstream response;
	//response << HTTP/1.1 200 OK\r\nContent-Type: image/gif\r\nContent-Length: " << gif.size() << "\r\n\r\n";
	//response << "HTTP/1.1 200 OK\r\n";
	//response << "Content-Type: image/gif\r\n";
    //response << "Content-Length: " << gif.size() << "\r\n";
    //response << "Connection: close\r\n\r\n";

	//string s = response.str();
	//s.append(gif.begin(), gif.end());

	string response;
	//if (!isBindedSocket(client_fd))
	//	response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "+ parser::toString(it.getNames()[0].size() + 2)  +"\r\n\r\n" + it.getNames()[0] + "\r\n";
	//else
		response = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin\r\nContent-Length: 7\r\n\r\nwhat?\r\n";

	if (send(client_fd, response.c_str(), response.size(), 0) == -1)
	//if (send(client_fd, s.c_str(), s.size(), 0) == -1)
		logger::fatal("send");

	close(client_fd);
}

int WebServ::isBindedSocket(int fd) {

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++)
		if (fd == it->second)
			return true;

	return false;
}

void WebServ::run(void) {

	epoll_fd = epoll_create(1);
	if (epoll_fd < 0)
		throw runtime_error("epoll");

	map<string, int>::iterator it = _binded_sockets.begin();
	for (; it != _binded_sockets.end(); it++) {
		epoll_event event = (epoll_event){};
		event.events = EPOLLIN;
		event.data.fd = it->second;

		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, it->second, &event) == -1)
			throw runtime_error("epool_ctl");
	}

	epoll_event events[MAX_EVENTS];

	while (true) {

		int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT);
		if (num_events == -1)
			throw runtime_error("epoll_wait");

		for (int i = 0; i < num_events; i++) {
			cout << events[i].data.fd << endl;
			if (isBindedSocket(events[i].data.fd)) {
				acceptNewConnection(events[i].data.fd);
			} else if (events[i].events & (EPOLLIN| EPOLLET)) {
				handle_client_request(epoll_fd, events[i].data.fd);
			} else if (events[i].events & (EPOLLOUT| EPOLLET)) {
				handle_client_response(events[i].data.fd);
			}
		}
	}

	close(epoll_fd);
}
