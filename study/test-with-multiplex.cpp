#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <sys/epoll.h>
#include <cerrno>

typedef struct addrinfo t_addrinfo;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr_storage t_sockaddr_storage;

// socket: Create an endpoint for communication. Returns a file descriptor.
	// socket(int domain, int type, int protocol)
	// domain: Address family. AF_INET: IPv4, AF_INET6: IPv6.
	// type: Communication semantics. SOCK_STREAM: TCP, SOCK_DGRAM: UDP.
	// protocol: Protocol. 0: Default protocol for domain and type.

int	open_server(std::string port)
{
	t_addrinfo hints;
	t_addrinfo *res;
	int	status;
	int socket_fd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(NULL, port.c_str(), &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		return (-1);
	}

	socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	status = bind(socket_fd, res->ai_addr, res->ai_addrlen);
	if (status != 0)
	{
		std::cerr << "bind: " << strerror(status) << std::endl;
		return (-1);
	}
	freeaddrinfo(res);
	if (listen(socket_fd, 10) == -1)
    {
        close(socket_fd);
        std::cerr << "listen error" << std::endl;
        return -1;
    }
	return (socket_fd);
}

int	run_without_multiplex(int socket_fd)
{
	int status = 0;

	listen(socket_fd, 10);

	t_sockaddr_storage client_addr;
	socklen_t client_addr_size;
	int client_fd;
	client_addr_size = sizeof(client_addr);
	client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_fd == -1)
	{
		std::cerr << "accept: " << strerror(status)  
		<< std::endl;
		return (4);
	}
	std::cout << "New Connection." << std::endl
		<< " Server fd: " << socket_fd << std::endl
		<< "Client fd: " << client_fd << std::endl;
	

	char buffer[BUFSIZ];
	int	bytes_read = 1;
	std::cout << "Size of buffer: " << BUFSIZ << std::endl;

	(void)buffer;
	while (bytes_read >= 0)
	{
		bytes_read = recv(client_fd, buffer, BUFSIZ, 0);
		if (not bytes_read)
			break;
		else if (bytes_read == -1)
		{
			std::cerr << "recv: " << strerror(status) << std::endl;
			break;
		}
		buffer[bytes_read] = '\0';
		std::string message_back = "Message received ";
		std::stringstream ss;
		ss << "HTTP/1.1 200 OK\r\n"
		<< "Content-Type: text/html\r\n"
		<< "Content-Length: " << message_back.size() << "\r\n"
		<< "Connection: close\r\n"
		<< "\r\n"
		<< message_back;

		std::string response = ss.str();
		std::cout << "Received. Bytes readed: " << bytes_read << std::endl;
		std::cout << "Message: " << buffer << std::endl;
		int bytes_sent = send(client_fd, response.c_str(), response.size(), 0);
		if (bytes_sent == -1)
		{
			std::cerr << "send: " << strerror(status) << std::endl;
			break;
		}
		else if ((unsigned long)bytes_sent == response.size())
			std::cout << "Sent full message. Size: " << bytes_sent << std::endl;
		else
			std::cout << "Sent partial message. Size: " << bytes_sent << std::endl;


	}

	close(client_fd);
	close(socket_fd);
	return (0);
}


int	run_with_epoll(int socket_fd)
{
	int epoll_fd = epoll_create1(0); // Cria uma instância de epoll
    if (epoll_fd == -1)
    {
        std::cerr << "epoll_create1 error" << std::endl;
        return -1;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
    {
        std::cerr << "epoll_ctl error" << std::endl;
        close(epoll_fd);
        return -1;
    }

    const int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];

    while (true)
    {
        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_fds == -1)
        {
            std::cerr << "epoll_wait error" << std::endl;
            close(epoll_fd);
            return -1;
        }

        for (int i = 0; i < num_fds; ++i)
        {
            if (events[i].data.fd == socket_fd)
            {
                int client_fd = accept(socket_fd, NULL, NULL);
                if (client_fd == -1)
                {
                    std::cerr << "accept error" << std::endl;
                    continue;
                }

                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
                {
                    std::cerr << "epoll_ctl error" << std::endl;
                    close(client_fd);
                    continue;
                }
				std::cout << "New Connection. Fd = " << client_fd << std::endl;
            }
            else
            {
                char buffer[512];
                int bytes_read = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (bytes_read <= 0)
                {
                    if (bytes_read == 0)
                    {
                        // Conexão fechada pelo cliente
                        std::cout << "Client disconnected" << std::endl;
                    }
                    else
                    {
                        std::cerr << "read error: " << strerror(errno) << std::endl;
                    }
                    close(events[i].data.fd);
                }
                else
                {
                    std::string message_back = "Hello";
                    std::stringstream ss;
                    ss << "HTTP/1.1 200 OK\r\n"
                       << "Content-Type: text/html\r\n"
                       << "Content-Length: " << message_back.size() << "\r\n"
                       << "Connection: close\r\n"
                       << "\r\n"
                       << message_back;

                    std::string response = ss.str();
                    int bytes_sent = send(events[i].data.fd, response.c_str(), response.size(), 0);
                    if (bytes_sent == -1)
                    {
                        std::cerr << "send error: " << strerror(errno) << std::endl;
                    }
                    else
                    {
                        std::cout << "Sent: " << response << std::endl;
                    }
                }
            }
        }
    }

    close(epoll_fd);
    return 0;
}


int	main()
{
	int status;
	
	int socket_fd = open_server("4243");
	status = run_with_epoll(socket_fd);
	close(socket_fd);
	return (status);
}

// sockaddr_in: Structure that contains an internet address
// ai_family:
// AF_UNSPEC: Any address family is accepted
// AF_INET: IPv4
// AF_INET6: IPv6
// ai_socktype:
// SOCK_STREAM: TCP
// SOCK_DGRAM: UDP
// SOCK_RAW: Raw socket
// SOCK_SEQPACKET: Sequenced packet stream
// SOCK_RDM: Reliably-delivered message
// SOCK_PACKET: Linux specific way of getting packets at the dev level
// hints.ai_flags = AI_PASSIVE;
// AI_PASSIVE: Used for bind() call
// AI_CANONNAME: Used for getnameinfo() call
// AI_NUMERICHOST: Prevents DNS lookup
// AI_NUMERICSERV: Prevents DNS lookup