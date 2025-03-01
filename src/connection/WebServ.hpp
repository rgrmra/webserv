#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "parser.hpp"
#include <map>
#include <netdb.h>
#include <string>

class IStream;
class Http;

class WebServ {
	private:
		static WebServ *_instance;

		int _epoll_fd;
		std::map<std::string, int> _binded_sockets;
		std::map<int, IStream *> _client_connections;

		WebServ(void);

		void removeBindedPorts(std::string port);
		bool isBinded(std::string listen);
		struct addrinfo *getAddrInfo(std::string host);
		int createSocket(std::string listen);
		std::string getIpByFileDescriptor(int client_fd);
		void acceptNewConnection(int client_fd);
		void closeConnection(int client_fd);
		void inputHandler(std::map<int, IStream *>::iterator it);
		void outputHandler(std::map<int, IStream *>::iterator it);
		void checkTimeOut(void);

	public:
		static const int BUFFER_SIZE = 128 * parser::KILOBYTE;
		static const int MAX_EVENTS = 252;
		static const long TIMEOUT = 30;
		static const long KEEP_ALIVE_TIMEOUT = 3;
		
		static WebServ *getInstance(void);

		virtual ~WebServ(void);

		void addStream(IStream *stream);
		void delStream(int fd);
		void controlEpoll(int client_fd, int flag, int option);

		void run(void);
		void stop(void);
};

#endif /* WEBSERV_HPP */
