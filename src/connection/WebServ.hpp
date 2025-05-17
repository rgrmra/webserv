#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <map>
#include <netdb.h>
#include <string>

class IStream;
class Http;

class WebServ
{
	private:
		static WebServ *_instance;

		int _epoll_fd;
		std::map<std::string, int> _sockets;
		std::map<int, IStream *> _connections;

		WebServ(void);

		void removeBindedPorts(const std::string &port);
		bool isBinded(const std::string &host);
		struct addrinfo *getAddrInfo(const std::string &host);
		int createSocket(const std::string &host);
		std::string getIpByFileDescriptor(const int &client_fd);
		void acceptNewConnection(const int &client_fd);
		void closeConnection(const int &client_fd);
		void inputHandler(std::map<int, IStream *>::iterator &stream);
		void outputHandler(std::map<int, IStream *>::iterator &stream);
		void checkTimeOut(void);

	public:
		static WebServ *getInstance(void);

		virtual ~WebServ(void);

		void addStream(IStream *stream);
		void delStream(const int &socket_fd);
		void controlEpoll(const int &client_fd, const int &flag, const int &option);

		void run(void);
		void stop(void);
};

#endif // WEBSERV_HPP
