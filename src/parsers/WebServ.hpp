#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Http.hpp"
#include <list>
#include <netdb.h>

typedef struct s_socket {
	int fd;
	addrinfo addr;
	addrinfo *p;
	int opt;
}	t_socket;

class WebServ {
	private:
		Http *_http;
		std::list<t_socket> _sockets;

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
