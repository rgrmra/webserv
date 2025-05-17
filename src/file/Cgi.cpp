#include "Cgi.hpp"
#include "Connection.hpp"
#include "Page.hpp"
#include "WebServ.hpp"
#include "code.hpp"
#include "env.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "response.hpp"
#include "standard.hpp"
#include "step.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

using namespace std;

Cgi::Cgi(Connection *connection) : Resource(connection)
{
	std::memset(_sock, EOF, 2 * sizeof(int));

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sock) == -1)
	{
		response::builder(connection, code::INTERNAL_SERVER_ERROR);
		return;
	}

	_fd = _sock[1];

	populateEnv(connection);

	vector<char *> _envp = createVector(_env);

	vector<string> _args;
	_args.push_back(connection->getLocation().getFastCgi());

	vector<char *> _argv = createVector(_args);

	_pid = fork();
	if (_pid < 0)
	{
		closeSockets();
		deleteVector(_argv);
		deleteVector(_envp);
		return;
	}
	else if (_pid == 0)
	{
		dup2(_sock[0], STDOUT_FILENO);
		dup2(_sock[0], STDIN_FILENO);
		closeSockets();

		execve(_argv.data()[0], _argv.data(), _envp.data());

		deleteVector(_argv);
		deleteVector(_envp);
		throw runtime_error("execve failed");
	}
	close(_sock[0]);
	_sock[0] = -1;

	deleteVector(_argv);
	deleteVector(_envp);
	
	_output = connection->getBody();
	_step = step::RESPONSE;

	WebServ *webserv = WebServ::getInstance();
	webserv->addStream(this);
	if (_output.empty())
		webserv->controlEpoll(_fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
	else
		webserv->controlEpoll(_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_ADD);
}

void Cgi::addEnv(const string &key, const string &value)
{
	_env.push_back(key + "=" + value);
}

void Cgi::populateEnv(Connection *connection)
{
	URL *url = connection->getUri();
	Server &server = connection->getServer();

	string server_name;
	if (server.getNames().size())
		server_name = server.getNames()[0];

	list<string> tmp = parser::split(connection->getId(), ':');
	addEnv(env::CONTENT_LENGTH, (*connection)[header::CONTENT_LENGTH]);
	addEnv(env::CONTENT_TYPE, (*connection)[header::CONTENT_TYPE]);
	addEnv(env::GATEWAY_INTERFACE, standard::GATEWAY_INTERFACE);
	addEnv(env::QUERY_STRING, connection->getUri()->getQuery());
	addEnv(env::REMOTE_ADDR, connection->getId());
	addEnv(env::REMOTE_HOST, (*connection)[header::HOST]);
	addEnv(env::REMOTE_PORT, tmp.back());
	addEnv(env::REQUEST_METHOD, connection->getMethod());
	addEnv(env::SCRIPT_NAME, url->getPath());
	addEnv(env::SERVER_NAME, server_name);
	addEnv(env::SERVER_PROTOCOL, standard::PROTOCOL);
	addEnv(env::SERVER_SOFTWARE, standard::SERVER_SOFTWARE);
	addEnv(env::SCRIPT_FILENAME, connection->getUri()->getAbsolutePath());
	addEnv(env::REDIRECT_STATUS, code::OK);

	if (url->getPathInfo().size())
	{
		addEnv(env::PATH_INFO, url->getPathInfo());
		addEnv(env::PATH_TRANSLATED, url->getPathTranslated());
	}

	map<string, string> headers = connection->getHeaders();
	map<string, string>::const_iterator header = headers.begin();
	for (; header != headers.end(); ++header)
	{
		string key = header->first;
		string value = header->second;

		string transformed_key = parser::toUpper(key);
		parser::replace(transformed_key, '-', '_');

		addEnv(env::HTTP_PREFIX + transformed_key, value);
	}
}

Cgi::Cgi(const Cgi &src) : Resource(src._connection)
{
	*this = src;
}

Cgi &Cgi::operator=(const Cgi &rhs)
{
	if (this == &rhs)
		return *this;

	return *this;
}

Cgi::~Cgi(void)
{
	if (_pid != -1)
		kill(_pid, SIGKILL);

	close(_sock[0]);
}

vector<char *> Cgi::createVector(vector<string> &container)
{
	vector<char *> tmp;

	for (size_t i = 0; i < container.size(); ++i) {
		char *env = new char[container[i].size() + 1];
		tmp.push_back(strcpy(env, container[i].c_str()));
	}
	tmp.push_back(NULL);

	return tmp;
}

void Cgi::deleteVector(vector<char *> &container)
{
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

void Cgi::parseCgiResponse(void)
{
	istringstream iss(_output);
	string line;

	_connection->setHeaders(standard::EMPTY_HEADER);

	while(getline(iss, line) && !line.empty())
	{
		_output.erase(0, line.size() + 1);

		if (line == "\r")
			break;

		size_t separator = line.find_first_of(":");
		if (separator == string::npos)
			return response::builder(_connection, code::INTERNAL_SERVER_ERROR);

		string key = line.substr(0, separator);
		string value = line.substr(separator + 1);

		parser::trim(value, " \t\v\r");

		_connection->addHeader(key, value);
	}
}

void Cgi::sendCGI(void)
{
	WebServ *webserv = WebServ::getInstance();
	webserv->controlEpoll(_fd, 0, EPOLL_CTL_DEL);

	if (_output.find_first_of("\r\n\r\n") == string::npos)
		return response::builder(_connection, code::INTERNAL_SERVER_ERROR);

	parseCgiResponse();

	if (*_connection == header::STATUS)
	{
		string status = (*_connection)[header::STATUS];

		parser::trim(status, " \t\v\r");

		status = status.erase(status.find_first_of(" "));

		if (status != code::OK)
			return response::builder(_connection, status);
	}

	_size = _output.size();
	_step = step::CLOSE;
	_connection->buildResponse();
	_pid = -1;
}

void Cgi::processInput(const size_t &bytes)
{
	(void) bytes;

	_output.append(_input);
	_input.erase();

	if (waitpid(_pid, NULL, WNOHANG))
		sendCGI();
}
