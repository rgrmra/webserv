#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

typedef struct addrinfo t_addrinfo;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr_storage t_sockaddr_storage;

int	main()
{
	t_addrinfo hints;
	t_addrinfo *res;
	int	status;
	int socket_fd;
	int client_fd;

	t_sockaddr_storage client_addr;
	socklen_t client_addr_size;


	// char buffer[INET6_ADDRSTRLEN];

	// (void)buffer;
	// (void)argc;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;



	status = getaddrinfo(NULL, "4243", &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		return (2);
	}

	// socket: Create an endpoint for communication. Returns a file descriptor.
	// socket(int domain, int type, int protocol)
	// domain: Address family. AF_INET: IPv4, AF_INET6: IPv6.
	// type: Communication semantics. SOCK_STREAM: TCP, SOCK_DGRAM: UDP.
	// protocol: Protocol. 0: Default protocol for domain and type.
	socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	status = bind(socket_fd, res->ai_addr, res->ai_addrlen);
	if (status != 0)
	{
		std::cerr << "bind: " << strerror(status) << std::endl;
		return (3);
	}
	listen(socket_fd, 10);

	client_addr_size = sizeof(client_addr);
	client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_fd == -1)
	{
		std::cerr << "accept: " << strerror(status) << std::endl;
		return (4);
	}
	std::cout << "New Connection." << std::endl
		<< " Server fd: " << socket_fd << std::endl
		<< "Client fd: " << client_fd << std::endl;
	freeaddrinfo(res);
	return (0);
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