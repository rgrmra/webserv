#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Connection.hpp"
#include "Http.hpp"
#include <map>
#include <netdb.h>
#include <sys/epoll.h>

class WebServ {
	private:
		std::map<std::string, int> _binded_sockets;

		bool isBinded(std::string listen);
		void removeBindedPorts(std::string port);
		struct addrinfo *getAddrInfo(std::string host);
		int createSocket(std::string listen);
		struct epoll_event createEpollEvent(int client_fd, int flags, int ctl);
		int isBindedSocket(int fd);
		std::string getIpByFileDescriptor(int client_fd);
		std::string getHostnameByFileDescriptor(int client_fd);
		void acceptNewConnection(int client_fd);

	public:
		Http *_http;
		std::map<int, Connection*> client_connections;
		int epoll_fd;
		static const int MAX_EVENTS = 10;
		
		WebServ(Http *http);
		WebServ(const WebServ &src);
		WebServ &operator=(const WebServ &rhs);
		virtual ~WebServ(void);

		void handle_client_request(int epoll_fd, int client_fd);
		void handle_client_response(int client_fd);

		void run(void);
};

#endif /* WEBSERV_HPP */
