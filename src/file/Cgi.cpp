#include "Cgi.hpp"
#include "Connection.hpp"
#include "Page.hpp"
#include "WebServ.hpp"
#include "code.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "response.hpp"
#include "status.hpp"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include <iostream>

using namespace std;

Cgi::Cgi(Connection *connection, string fastcgi_pass)
	: Resource(connection) {

	_sock[0] = -1;
	_sock[1] = -1;

	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, _sock) == -1)
	 	throw runtime_error("socketpair failed");

	_fd = _sock[0];

	vector<string> _env;
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SCRIPT_FILENAME=" + connection->getPath());
	_env.push_back("REQUEST_METHOD=" + connection->getMethod());
	_env.push_back("QUERY_STRING=" + connection->getQueryString());
	_env.push_back("CONTENT_LENGTH=" + parser::toString(connection->getBody().size()));
	_env.push_back("CONTENT_TYPE=" + (*connection)[header::CONTENT_TYPE]);
	_env.push_back("SERVER_PROTOCOL=" + response::PROTOCOL);
	_env.push_back("HTTP_USER_AGENT=" + (*connection)[header::USER_AGENT]);
	_env.push_back("HTTP_COOKIE=" + (*connection)[header::COOKIE]);
	_env.push_back("REDIRECT_STATUS=200");

	vector<char *> _envp = createVector(_env);

	vector<string> _args;
	_args.push_back(fastcgi_pass);
	_args.push_back(connection->getPath());

	vector<char *> _argv = createVector(_args);

	_pid = fork();
	if (_pid < 0) {
		closeSockets();
		throw runtime_error("fork failed");
	} else if (_pid == 0) {
		close(_sock[0]);
		dup2(_sock[1], STDOUT_FILENO);
		close(_sock[1]);

		execve(_argv.data()[0], _argv.data(), _envp.data());

		deleteVector(_argv);
		deleteVector(_envp);
		throw runtime_error("execve failed");
	}
	close(_sock[1]);

	deleteVector(_argv);
	deleteVector(_envp);

	WebServ *webserv = WebServ::getInstance();
	webserv->addStream(this);
	webserv->controlEpoll(_fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
}

Cgi::Cgi(const Cgi &src)
	: Resource(src._connection) {

	*this = src;
}

Cgi &Cgi::operator=(const Cgi &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

Cgi::~Cgi(void) {

	if (_pid != -1)
		kill(_pid, SIGKILL);
	close(_sock[0]);
}

vector<char *> Cgi::createVector(vector<string> &container) {

	vector<char *> tmp;

	for (size_t i = 0; i < container.size(); i++) {
		char * env = new char[container[i].size() + 1];
		tmp.push_back(strcpy(env, container[i].c_str()));
	}
	tmp.push_back(NULL);

	return tmp;
}

void Cgi::deleteVector(vector<char *> &container) {

	vector<char *>::iterator it = container.begin();
	for (; it != container.end() && *it; ++it) {
		delete[] *it;
	}
	container.clear();
}

void Cgi::closeSockets(void) {

	if (_sock[0] > -1)
		close(_sock[0]);

	if (_sock[1] > -1)
		close(_sock[1]);
}

void Cgi::sendCGI(void) {

	int status = 0;

	if (waitpid(_pid, &status, WUNTRACED) && WEXITSTATUS(status)) {
		_connection->setCode(code::BAD_GATEWAY);
		_connection->setStatus(status::BAD_GATEWAY);
		_output = Page(_connection).getData(1024);
		_type = "text/html";
	} else {
		string tmp = parser::find("Content-type: ", _output, "\n");
		if (tmp.size()) {
			_type = tmp;
			size_t pos = _output.find("\n");
			if (pos != string::npos)
				_output.erase(0, pos + 1);
		} else
			_type = "text/plain";
	}
	_size = _output.size();
	_connection->buildResponse();
	_connection->setStep(IStream::RESPONSE);
	_step = IStream::CLOSE;
	_pid = -1;
}

void Cgi::processInput(size_t bytes) {

	(void) bytes;

	_output.append(_input);
	_input.erase();
}
