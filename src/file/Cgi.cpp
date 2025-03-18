#include "Cgi.hpp"
#include "Connection.hpp"
#include "Page.hpp"
#include "WebServ.hpp"
#include "code.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "request.hpp"
#include "response.hpp"
#include "status.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

Cgi::Cgi(Connection *connection)
	: Resource(connection) {

	_sock[0] = -1;
	_sock[1] = -1;

	if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, _sock) == -1)
	 	throw runtime_error("socketpair failed");

	_fd = _sock[0];

	URL *uri = _connection->getUri();

	cout << "enter cgi: " << uri->getAbsolutePath() << endl;
	vector<string> _env;
	_env.push_back("SERVER_SOFTWARE=webserv/0.1.0");
	_env.push_back("SERVER_NAME=");
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=" + response::PROTOCOL);
	_env.push_back("SERVER_PORT=");
	_env.push_back("REQUEST_METHOD=" + connection->getMethod());
	_env.push_back("PATH_INFO=");
	_env.push_back("PATH_TRANSLATED=");
	_env.push_back("SCRIPT_FILENAME=" + uri->getAbsolutePath());
	_env.push_back("SCRIPT_NAME=" + uri->getFile());
	_env.push_back("QUERY_STRING=" + uri->getQuery());
	_env.push_back("REMOTE_HOST=");
	_env.push_back("REMOTE_ADDR=");
	_env.push_back("AUTH_TYPE=");
	_env.push_back("REMOTE_USER=");
	_env.push_back("REMOTE_IDENT=");
	_env.push_back("CONTENT_TYPE=" + (*connection)[header::CONTENT_TYPE]);
	if (*connection == header::CONTENT_LENGTH)
		_env.push_back("CONTENT_LENGTH=" + (*connection)[header::CONTENT_LENGTH]);
	_env.push_back("HTTP_COOKIE=" + (*connection)[header::COOKIE]);
	_env.push_back("REDIRECT_STATUS=200");
	_env.push_back("HTTP_ACCEPT=" + (*connection)[header::ACCEPT]);
	_env.push_back("HTTP_USER_AGENT=" + (*connection)[header::USER_AGENT]);

	vector<char *> _envp = createVector(_env);

	vector<string> _args;
	_args.push_back(connection->getLocation().getFastCgi());
	_args.push_back(uri->getAbsolutePath());

	vector<char *> _argv = createVector(_args);

	_pid = fork();
	if (_pid < 0) {
		closeSockets();
		throw runtime_error("fork failed");
	} else if (_pid == 0) {
		close(_sock[0]);
		int fd = dup(_sock[1]);
		dup2(_sock[1], STDOUT_FILENO);
		dup2(fd, STDIN_FILENO);
		close(_sock[1]);
		close(fd);

		execve(_argv.data()[0], _argv.data(), _envp.data());

		deleteVector(_argv);
		deleteVector(_envp);
		throw runtime_error("execve failed");
	}
	close(_sock[1]);
	_sock[1] = -1;

	deleteVector(_argv);
	deleteVector(_envp);

	_output = connection->getBody();
	_step = IStream::RESPONSE;

	WebServ *webserv = WebServ::getInstance();
	webserv->addStream(this);
	if (_output.empty())
		webserv->controlEpoll(_fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
	else
		webserv->controlEpoll(_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_ADD);
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

void Cgi::parse(void) {

	istringstream iss(_output);
	string line;

	_connection->setHeaders(response::EMPTY_HEADER);

	while(getline(iss, line) && !line.empty()) {

		_output.erase(0, line.size() + 1);

		if (line == "\r")
			break;

		size_t separator = line.find_first_of(":");
		if (separator == string::npos)
			return response::pageInternalServerError(_connection);

		string key = line.substr(0, separator);
		string value = line.substr(separator + 1);

		parser::trim(value, " \t\v\r");

		_connection->addHeader(key, value);
	}
}

void Cgi::sendCGI(void) {

	if (_output.find_first_of("\r\n\r\n") == string::npos)
		return response::pageInternalServerError(_connection);

	parse();

	if (*_connection == header::STATUS) {

		string status = (*_connection)[header::STATUS];

		parser::trim(status, " \t\v\r");

		status = status.erase(status.find_first_of(" "));

		if (status != code::OK)
			return response::builder(_connection, status);
	}

	_size = _output.size();
	_step = IStream::CLOSE;
	_connection->buildResponse();
	_pid = -1;
}

void Cgi::processInput(size_t bytes) {

	(void) bytes;

	_output.append(_input);
	_input.erase();

	if (waitpid(_pid, NULL, WNOHANG))
		sendCGI();
}
