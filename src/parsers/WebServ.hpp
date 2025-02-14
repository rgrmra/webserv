#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "parser.hpp"
#include <map>
#include <netdb.h>
#include <string>

class Connection;
class Http;

class WebServ {
	private:
		Http *_http;
		int _epoll_fd;
		std::map<std::string, int> _binded_sockets;
		std::map<int, Connection *> _client_connections;

		void removeBindedPorts(std::string port);
		bool isBinded(std::string listen);
		struct addrinfo *getAddrInfo(std::string host);
		int createSocket(std::string listen);
		void controlEpoll(int client_fd, int flag, int option);
		std::string getIpByFileDescriptor(int client_fd);
		void acceptNewConnection(int client_fd);
		void closeConnection(int client_fd);
		void handleRequest(int client_fd);
		int sendMessage(Connection *connection, std::string message);
		void handleResponse(int client_fd);
		int isBindedSocket(int fd);
		bool isTimedOut(int client_fd);

	public:
		static const int BUFFER_SIZE = parser::MEGABYTE;
		static const int MAX_EVENTS = 252;
		static const long TIMEOUT = 30;
		static const long KEEP_ALIVE = 5;
		
		WebServ(Http *http);
		WebServ(const WebServ &src);
		WebServ &operator=(const WebServ &rhs);
		virtual ~WebServ(void);

		void run(void);
};

#endif /* WEBSERV_HPP */
