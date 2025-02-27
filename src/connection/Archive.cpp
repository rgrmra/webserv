#include "Archive.hpp"
#include "AStream.hpp"
#include "WebServ.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

Archive::Archive(int fd, std::string id)
	: AStream(fd, id) {

	_default_fd = fd;

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sock) == -1) {
		perror("socketpair");
		return;
	}

	pid = fork();
	
	if (pid == 0) {
		close(sock[0]);

		char *argv[] = {strdup("./index.php"), NULL};
		execve("./index.php", argv, __environ);

		close(sock[1]);
		exit(1);
	}
	close(sock[1]);
	waitpid(pid, NULL, WNOHANG);

	_fd = sock[0];
	WebServ::getInstance()->addFdToEpoll(sock[0], this);
}

Archive::Archive(const Archive &src)
	: AStream(src) {

	*this = src;
}

Archive &Archive::operator=(const Archive &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

Archive::~Archive(void) {

}
