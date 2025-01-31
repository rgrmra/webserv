#ifndef SERVERCONTROLLER_HPP
# define SERVERCONTROLLER_HPP

# include <iostream>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstring>
# include <unistd.h>
# include <cstdlib>
# include <vector>
# include <sys/epoll.h>
# include <sstream>
# include <cerrno>
# include <fcntl.h>

typedef struct addrinfo t_addrinfo;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr_storage t_sockaddr_storage;


class ServerController
{
	private:
		// Server &server;
		std::vector<int>	servers;
		int					epoll_fd;
	public:
		ServerController();

		// openServer opens a server on the specified host and port.
		int	runWithEpoll();
		int	serverRun();
		int	openServer(std::string host, std::string port);
		int	acceptNewConnections(int serverFd);
		bool	searchInServers(int fd);

		// epoll
		int	createEpoll();
		int	addServersToEpoll();
		int	addFdToEpoll(int fd, int events);
		int modify_fd_in_epoll(int epoll_fd, int fd, uint32_t events);

		// request / response
		int processRequest(int fd);
		int processResponse(int fd);
};


#endif