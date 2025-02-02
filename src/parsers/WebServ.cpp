#include "Http.hpp"
#include "Server.hpp"
#include "logger.hpp"
#include "WebServ.hpp"
#include "parser.hpp"
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
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

#define MAX_EVENTS 10
#define TIMEOUT 6

using namespace std;

WebServ::WebServ(Http *http)
	: _http(http) {

	vector<Server> servers = http->getServers();

	vector<Server>::iterator it = servers.begin();
	for (; it != servers.end(); it++) {

		vector<string> listens = it->getListen();
		vector<string>::iterator itl = listens.begin();
		for (; itl != listens.end(); itl++) {

			t_socket sock = (t_socket){};

			sock.addr = (struct addrinfo){};
			sock.addr.ai_family = AF_INET;
			sock.addr.ai_socktype = SOCK_STREAM;
			sock.addr.ai_flags = AI_PASSIVE;

			list<string> tmp = parser::split(*itl, ':');
			if (getaddrinfo(tmp.front().c_str(), tmp.back().c_str(), &sock.addr, &sock.p))
				throw runtime_error("getaddrinfo");

			sock.fd = socket(sock.p->ai_family, sock.p->ai_socktype | SOCK_NONBLOCK, sock.p->ai_protocol);

			if (setsockopt(sock.fd, SOL_SOCKET, SO_REUSEADDR, &sock.opt, sizeof(sock.opt)) == -1)
				throw runtime_error("setsockopt");

			if (bind(sock.fd, sock.p->ai_addr, sock.p->ai_addrlen) != 0)
				throw runtime_error("bind: " + *itl);
			freeaddrinfo(sock.p);

			if (listen(sock.fd, MAX_EVENTS) == -1)
				throw runtime_error("listen");

			_sockets.insert(sock.fd);
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

	for (size_t i = 0; i < _sockets.size(); i++)
		close(i);
}

void WebServ::handle_accept_new_connections(int epoll_fd, int client_fd) {

	//if (_sockets.find(client_fd) == _sockets.end())
	//	return false;
	//set<int>::iterator it = _sockets.begin();
	//for (; it != _sockets.end(); it++) {

	//	if (client_fd != *it)
	//		continue;

	//while (true) {
	int fd = accept(client_fd, NULL, NULL);
		if (fd == -1) {
			if (not (errno == EAGAIN || errno == EWOULDBLOCK))
				logger::fatal("accept");
	//		break;
		}

		epoll_event event = {};
		event.events = EPOLLIN | EPOLLET;
		event.data.fd = fd;

		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
			logger::fatal("epoll_ctl");
	//		continue;
		}
	//}

	//	return true;
	//}
	//return false;
}

void WebServ::handle_client_request(int epoll_fd, int client_fd) {

	char buffer[8192];
	int bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
	if (bytes_read == -1)
		return (close(client_fd), logger::fatal("recv"));

	if (bytes_read == 0)
		return (close(client_fd), logger::warning("client disconected"));

	buffer[bytes_read] = '\0';
	logger::info("received: ");
	cout << buffer << endl;

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

void WebServ::handle_client_response(int client_fd) {

	cout << getIpByFileDescriptor(client_fd);

	Server it = _http->getServerByListen(getIpByFileDescriptor(client_fd));

	cout << it.getMaxBodySize() << endl;

	string response;
	//if (socket_to_config.find(client_fd) != socket_to_config.end())
		response = "HTTP/1.1 200 OK\r\nContent-Length: "+ parser::toString(it.getListen()[0].size() + 2)  +"\r\n\r\n" + it.getListen()[0] + "\r\n";
	//else
	//	response = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\n\r\nwhat?\r\n";

	if (send(client_fd, response.c_str(), response.size(), 0) == -1)
		logger::fatal("send");

	close(client_fd);
}

void WebServ::run(void) {

	epoll_fd = epoll_create(1);
	if (epoll_fd < 0)
		throw runtime_error("epoll");

	set<int>::iterator it = _sockets.begin();
	for (; it != _sockets.end(); it++) {
		epoll_event event = (epoll_event){};
		event.events = EPOLLIN;
		event.data.fd = *it;

		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, *it, &event) == -1)
			throw runtime_error("epool_ctl");
	}

	epoll_event events[MAX_EVENTS];

	while (true) {

		int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT);
		if (num_events == -1)
			throw runtime_error("epoll_wait");

		for (int i = 0; i < num_events; i++) {
			if (_sockets.find(events[i].data.fd) != _sockets.end()) {
				handle_accept_new_connections(epoll_fd, events[i].data.fd);
			} else if (events[i].events & (EPOLLIN| EPOLLET)) {
				handle_client_request(epoll_fd, events[i].data.fd);
			} else if (events[i].events & (EPOLLOUT| EPOLLET)) {
				handle_client_response(events[i].data.fd);
			}
		}
	}

	close(epoll_fd);
}
