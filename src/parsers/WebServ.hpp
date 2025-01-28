/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:47:52 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/28 19:57:44 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Http.hpp"
#include <list>
#include <netdb.h>

using namespace std;

typedef struct s_socket {
	int fd;
	addrinfo addr;
	addrinfo *p;
	int opt;
}	t_socket;

class WebServ {
	private:
		Http *_http;
		list<t_socket> _sockets;

	protected:
		bool handle_accept_new_connections(int epoll_fd, int client_fd);
		void handle_client_request(int epoll_fd, int client_fd);
		void handle_client_response(int client_fd);
		

	public:
		WebServ(Http *http);
		WebServ(const WebServ &src);
		WebServ &operator=(const WebServ &rhs);
		~WebServ(void);

		void run(void);
};

#endif /* WEBSERV_HPP */
