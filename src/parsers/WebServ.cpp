/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:01:41 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/28 20:30:50 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Server.hpp"
#include "logger.hpp"
#include "WebServ.hpp"
#include <asm-generic/socket.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 10
#define TIMEOUT 6

WebServ::WebServ(Http *http)
	: _http(http) {

	set<Server> servers = http->getServers();

	set<Server>::iterator it = servers.begin();
	for (; it != servers.end(); it++) {

		t_socket sock = (t_socket){};

		sock.addr = (struct addrinfo){};
		sock.addr.ai_family = AF_INET;
		sock.addr.ai_socktype = SOCK_STREAM;
		sock.addr.ai_flags = AI_PASSIVE;

		if (getaddrinfo(it->getHost().c_str(), it->getPort().c_str(), &sock.addr, &sock.p))
			throw runtime_error("getaddrinfo");

		sock.fd = socket(sock.p->ai_family, sock.p->ai_socktype, sock.p->ai_protocol);
		
		if (setsockopt(sock.fd, SOL_SOCKET, SO_REUSEADDR, &sock.opt, sizeof(sock.opt)) == -1)
			throw runtime_error("setsockopt");

		if (fcntl(sock.fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
			throw runtime_error("fcntl");

		if (bind(sock.fd, sock.p->ai_addr, sock.p->ai_addrlen) != 0)
			throw runtime_error("bind");
		freeaddrinfo(sock.p);

		if (listen(sock.fd, MAX_EVENTS) == -1)
			throw runtime_error("listen");

		_sockets.push_back(sock);
	}
}

WebServ::WebServ(const WebServ &src) {

	*this = src;
}

WebServ &WebServ::operator=(const WebServ &rhs) {

	if (this == &rhs)
		return *this;

	_http = rhs._http;
	_sockets = rhs._sockets;

	return *this;
}

WebServ::~WebServ(void) {

}

bool WebServ::handle_accept_new_connections(int epoll_fd, int client_fd) {

	list<t_socket>::iterator it = _sockets.begin();
	for (; it != _sockets.end(); it++) {

		if (client_fd != it->fd)
			continue;

		while (true) {
			int fd = accept(it->fd, NULL, NULL);
			if (fd == -1) {
				logger::fatal("accept");
				break;
			}

			if (fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
				logger::fatal("fcntl");
				continue;
			}

			epoll_event event = {};
			event.events = EPOLLIN | EPOLLET;
			event.data.fd = fd;

			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
				logger::fatal("epoll_ctl");
				continue;
			}
		}

		return true;
	}
	return false;
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

void WebServ::handle_client_response(int client_fd) {

	string response = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\n\r\nwhat?\r\n";

	if (send(client_fd, response.c_str(), response.size(), 0) == -1)
		logger::fatal("send");

	close(client_fd);
}

void WebServ::run(void) {

	int fd = epoll_create1(0);
	if (fd == -1)
		throw runtime_error("epoll");

	list<t_socket> sockets = _sockets;
	list<t_socket>::iterator it = sockets.begin();
	for (; it != sockets.end(); it++) {
		epoll_event event = (epoll_event){};
		event.events = EPOLLIN;
		event.data.fd = it->fd;

		if (epoll_ctl(fd, EPOLL_CTL_ADD, it->fd, &event) == -1)
			throw runtime_error("epool_ctl");
	}

	epoll_event events[MAX_EVENTS];

	while (true) {

		int num_events = epoll_wait(fd, events, MAX_EVENTS, TIMEOUT);
		if (num_events == -1)
			throw runtime_error("epoll_wait");
		else if (num_events == 0)
			continue;

		for (int i = 0; i < num_events; i++) {
			if (handle_accept_new_connections(fd, events[i].data.fd)) {
				continue;
			} else if (events[i].events & (EPOLLIN| EPOLLET)) {
				handle_client_request(fd, events[i].data.fd);
			} else if (events[i].events & (EPOLLOUT| EPOLLET)) {
				handle_client_response(events[i].data.fd);
			}
		}
	}

	close(fd);
}
