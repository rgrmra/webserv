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

	populateEnv(connection);

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

void Cgi::populateEnv(Connection *connection) {

	Server &server = connection->getServer();
	string server_name;
	if (server.getNames().size())
		server_name = server.getNames()[0];

	list<string> tmp = parser::split(connection->getId(), ':');
	//Server side
	//example for http://example.com/cgi-bin/script.php/extra/path
	_env.push_back("GATEWAY_INTERFACE=" + response::GATEWAY_INTERFACE);
	// Só se tiver path info, path info é o que vem depois do script name
	// ex, "/extra/path" precisa setar PATH_INFO e PATH_TRANSLATED
	// _env.push_back("PATH_INFO=");// ex, "/extra/path"
	// _env.push_back("PATH_TRANSLATED=");// ex, "/var/www/extra/path"
	_env.push_back("QUERY_STRING=" + connection->getUri()->getQuery());
	_env.push_back("REMOTE_ADDR=" + connection->getId());// Client IP address
	_env.push_back("REMOTE_HOST=" + (*connection)[header::HOST]);// Client host name or IP if not Host Name
	_env.push_back("REQUEST_METHOD=" + connection->getMethod());
	_env.push_back("SCRIPT_NAME=");//ex, "/cgi-bin/script.php"
	_env.push_back("SERVER_NAME=" + server_name);// ex "example.com"
	_env.push_back("REMOTE_PORT=" + tmp.back());// Client port
	_env.push_back("SERVER_PROTOCOL=" + response::PROTOCOL);
	_env.push_back("SERVER_SOFTWARE=" + response::SERVER_SOFTWARE);
	_env.push_back("REQUEST_METHOD=" + connection->getMethod());
	_env.push_back("SCRIPT_FILENAME=" + connection->getUri()->getAbsolutePath());
	_env.push_back("SERVER_SOFTWARE=" + response::SERVER_SOFTWARE);


	//From headers
	map<string, string> headers = connection->getHeaders();
	for (map<string, string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		string key = it->first;
		string value = it->second;

		string transformed_key;
		for (size_t i = 0; i < key.size(); ++i) {
			transformed_key += (key[i] == '-') ? '_' : toupper(key[i]);
		}

		if (transformed_key == "CONTENT_TYPE") {
			_env.push_back("CONTENT_TYPE=" + value);
		} else if (transformed_key == "CONTENT_LENGTH") {
			_env.push_back("CONTENT_LENGTH=" + value);
		} else if (transformed_key == "AUTHORIZATION") {
			// TODO: precisamos verificar se splitamos em AUTH_TYPE e REMOTE_USER
			_env.push_back("HTTP_AUTHORIZATION=" + value); // or parse it
		} else {
			_env.push_back("HTTP_" + transformed_key + "=" + value);
		}
	}
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
