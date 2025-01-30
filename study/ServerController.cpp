#include "ServerController.hpp"

ServerController::ServerController()
{
	servers = std::vector<int>();
}

int ServerController::set_non_blocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK, 0) == -1)
	{
		std::cerr << "ioctl FIONBIO error" << std::endl;
		return -1;
	}
	return 0;
}

int ServerController::modify_fd_in_epoll(int epoll_fd, int fd, uint32_t events)
{
	struct epoll_event event = {};
	event.data.fd = fd;
	event.events = events;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1)
	{
		std::cerr << "epoll_ctl error" << std::endl;
		return -1;
	}
	return 0;
}

int ServerController::processRequest(int fd) // criar uma classe para tratar a requisição
{
	char buffer[8192];
	int bytes_read = recv(fd, buffer, sizeof(buffer), 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			std::cout << "Client disconnected" << std::endl;
		else
			std::cerr << "recv error: " << strerror(errno) << std::endl;
		close(fd);
	}
	else
	{
		buffer[bytes_read] = '\0';
		std::cout << "Received: " << buffer << std::endl;
		if (modify_fd_in_epoll(this->epoll_fd, fd, EPOLLOUT | EPOLLET) == -1)
		{
			close(fd);
			return -1;
		}
	}
	return 0;
}
int ServerController::processResponse(int fd) // criar uma classe para tratar a resposta
{
	std::string message = "Hello";
	std::stringstream ss;
	ss << "HTTP/1.1 200 OK\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Content-Length: " << message.length() << "\r\n"
		<< "Connection: close\r\n"
		<< "\r\n"
		<< message;
	std::string response = ss.str();
	int bytes_sent = send(fd, response.c_str(), response.length(), 0);
	if (bytes_sent == -1)
	{
		std::cerr << "send error: " << strerror(errno) << std::endl;
		return -1;
	}
	else if ((unsigned long)bytes_sent == response.length())
		std::cout << "Sent full message. Size: " << bytes_sent << std::endl;
	else
		std::cout << "Sent partial message. Size: " << bytes_sent << std::endl;

	close(fd);
	return 0;
}

int	ServerController::openServer(std::string host, std::string port)
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
	this->servers.push_back(socket_fd);
	return (socket_fd);
}

int	ServerController::addFdToEpoll(int fd, int events)
{
	struct epoll_event event = {};
	event.data.fd = fd;
	event.events = events;
	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		std::cerr << "epoll_ctl error" << std::endl;
		return -1;
	}
	return 0;
}

int	ServerController::createEpoll()
{
	this->epoll_fd = epoll_create1(0);
	if (this->epoll_fd == -1)
	{
		std::cerr << "epoll_create1 error" << std::endl;
		return -1;
	}
	return 0;
}

int	ServerController::addServersToEpoll()
{
	typedef std::vector<int>::iterator iterator;
	for (iterator it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		if (this->addFdToEpoll(*it, EPOLLIN | EPOLLET) == -1)
		{
			close(this->epoll_fd);
			return -1;
		}
	}
	return 0;
}

int	ServerController::acceptNewConnections(int serverFd)
{
	while (true)
	{
		struct sockaddr_in client_addr;
		socklen_t client_addr_size = sizeof(client_addr);

		int client_fd = accept(serverFd,
			reinterpret_cast<struct sockaddr *>(&client_addr),
			&client_addr_size);
		if (client_fd == -1)
		{
			std::cerr << "accept: " << strerror(errno) << std::endl;
			return -1;
		}

		if (this->set_non_blocking(client_fd) == -1)
		{
			close(client_fd);
			continue;
		}

		if (this->addFdToEpoll(client_fd, EPOLLIN | EPOLLET) == -1)
		{
			close(client_fd);
			continue;
		}
	}
	return 0;
}

int	ServerController::serverRun()
{
	const int MAX_EVENTS = 10;
	struct epoll_event events[MAX_EVENTS];
	while (true)
	{
		int num_fds = epoll_wait(this->epoll_fd, events, MAX_EVENTS, -1);
		if (num_fds == -1)
		{
			std::cerr << "epoll_wait error" << std::endl;
			close(this->epoll_fd);
			return -1;
		}
		if (num_fds == 0)
			continue;
		for (int i = 0; i < num_fds; ++i)
		{


			if (searchInServers(events[i].data.fd))
				continue;
			else
			{
				if (events[i].events & EPOLLIN)
					this->processRequest(events[i].data.fd);
				else if (events[i].events & EPOLLOUT)
				{
					this->processResponse(events[i].data.fd);
				}
			}
		}
	}
}

bool	ServerController::searchInServers(int fd)
{
	typedef std::vector<int>::iterator iterator;
	for (iterator it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		if (*it == fd)
		{
			this->acceptNewConnections(*it);
			return true;
		}
	}
	return false;
}

int	ServerController::runWithEpoll()
{
	if (this->createEpoll() == -1)
		return -1;
	if (this->addServersToEpoll() == -1)
		return -1;
	this->serverRun();
	return 0;
}