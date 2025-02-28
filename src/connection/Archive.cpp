#include "Archive.hpp"
#include "AStream.hpp"
#include "Connection.hpp"
#include "IStream.hpp"
#include "WebServ.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

Archive::Archive(Connection *connection)
	: AStream(-1, connection->getPath()),
	  _connection(connection) {

	//_default_fd = fd;
	//WebServ::getInstance()->controlEpoll(_connection->getFd(), EPOLL, EPOLL_CTL_MOD);

	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, _sock) == -1) {
		perror("socketpair");
		return;
	}

	_pid = fork();
	
	if (_pid == 0) {
		close(_sock[0]);
		dup2(_sock[1], STDOUT_FILENO);
		close(_sock[1]);

		char *argv[] = {strdup(connection->getPath().c_str()), NULL};
		execve(connection->getPath().c_str(), argv, __environ);

		exit(1);
	}
	close(_sock[1]);

	_fd = _sock[0];
	WebServ::getInstance()->addStream(_sock[0], this);
	WebServ::getInstance()->controlEpoll(_sock[0], EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
	cout << "connection fd: " << connection->getFd() << endl;
	cout << "archive fd: " << _sock[0] << endl;
	_connection->setStep(IStream::BODY);
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

	if (_pid != -1)
		kill(_pid, SIGKILL);
	close(_sock[0]);
}

void Archive::teste(void) {

	kill(_pid, SIGKILL);
}

void Archive::setData(vector<char> &buffer, size_t bytes) {

	if (!bytes || buffer.empty()) {
		waitpid(_pid, 0, WUNTRACED);
		_size = _input.size();
		_connection->buildResponse();
		_connection->setStep(IStream::RESPONSE);
		_step = CLOSE;
		return;
	}

	_input.append(buffer.begin(), buffer.begin() + bytes);

	cout << "pid: " << _pid << endl;
	//if (waitpid(_pid, &status, WNOHANG) != 0) {
	_wpid = waitpid(_pid, &_status, WUNTRACED);
	if (_wpid == _pid) {
		_size = _input.size();
		_connection->buildResponse();
		//WebServ::getInstance()->addFdToEpoll(_connection->getFd(), _connection);
		//WebServ::getInstance()->controlEpoll(_connection->getFd(), EPOLLOUT | EPOLLET, EPOLL_CTL_ADD);
		//WebServ::getInstance()->controlEpoll(_sock[0], 0, EPOLL_CTL_DEL);
		cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>yes" << endl;
		cout << "socket: " << _sock[0] << endl;
		//close(_sock[0]);
		//WebServ::getInstance()->controlEpoll(_sock[0], 0, EPOLL_CTL_MOD);
		//WebServ::getInstance()->delStream(_sock[0]);
		//close(_sock[0]);
		_connection->setStep(IStream::RESPONSE);
		_step = IStream::CLOSE;
		//_connection->setReady(true);
		//WebServ::getInstance()->controlEpoll(_connection->getFd(), EPOLLOUT | EPOLLET, EPOLL_CTL_ADD);
		_pid = -1;
	}

	_time = time(NULL);
}
