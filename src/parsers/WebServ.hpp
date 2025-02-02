#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Connection.hpp"
#include "Http.hpp"
#include <map>
#include <netdb.h>
#include <set>

typedef struct s_socket {
	int fd;
	addrinfo addr;
	addrinfo *p;
	int opt;
}	t_socket;

class WebServ {
	private:
		std::string getIpByFileDescriptor(int client_fd);

	public:
		Http *_http;
		std::set<int> _sockets;
		std::map<int, Connection*> client_connections;
		int epoll_fd;
		static const int MAX_EVENTS = 10;
		
		WebServ(Http *http);
		WebServ(const WebServ &src);
		WebServ &operator=(const WebServ &rhs);
		~WebServ(void);

		void handle_accept_new_connections(int epoll_fd, int client_fd);
		void handle_client_request(int epoll_fd, int client_fd);
		void handle_client_response(int client_fd);

		void run(void);
};

#endif /* WEBSERV_HPP */
