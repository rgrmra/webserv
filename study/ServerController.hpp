#ifndef SERVERCONTROLLER_HPP
# define SERVERCONTROLLER_HPP

# include <iostream>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstring>
# include <unistd.h>
# include <cstdlib>

typedef struct addrinfo t_addrinfo;



class ServerController
{
	private:
		// Server &server;
	public:
		ServerController()
		{

		}
		int	openServer(std::string host, std::string port)
		{
			int status = 0;
			int socket_fd;



			socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
			if (socket_fd < 0)
			{
				std::cerr << "socket: " << strerror(status) << std::endl;
				return (-1);
			}

			int opt = 1;
			if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
			{
				std::cerr << "setsockopt error" << std::endl;
				close(socket_fd);

				return -1;
			}

			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(atoi(port.c_str()));
			addr.sin_addr.s_addr = INADDR_ANY;

			if (host != "0.0.0.0" && !host.empty())
			{
				if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
				{
					close(socket_fd);
					return false;
				}
			}


			status = bind(socket_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
			if (status != 0)
			{
				std::cerr << "bind: " << strerror(status) << std::endl;
				return (-1);
			}
			if (listen(socket_fd, SOMAXCONN) == -1)
			{
				close(socket_fd);
				std::cerr << "listen error" << std::endl;
				return -1;
			}
			return (socket_fd);
		}
};


#endif