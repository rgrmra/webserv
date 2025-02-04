#include "Connection.hpp"
#include "parser.hpp"
#include <ctime>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <unistd.h>

using namespace std;

Connection::Connection(int fd, string ip)
	: _fd(fd),
	  _ip(ip),
	  _time(time(NULL)) {
	
	_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5 \r\n\r\nwhat?";
	
}

Connection::Connection(const Connection &src) {

	*this = src;
}

Connection &Connection::operator=(const Connection &rhs) {

	if (this == &rhs)
		return *this;
	
	_fd = rhs._fd;
	_ip = rhs._ip;
	_host = rhs._host;
	_buffer = rhs._buffer;
	_method = rhs._method;
	_path = rhs._path;
	_protocol = rhs._protocol;
	_headers = rhs._headers;
	_body = rhs._body;
	_server = rhs._server;
	_response = rhs._response;
	_time = rhs._time;

	return *this;
}

Connection::~Connection(void) {

}

bool Connection::validateRequest(void) {

	string tmp = _buffer.data();

	if (tmp.find("GET ") == 0)
		return validateGet();
	else if (tmp.find("POST ") == 0)
		return validatePost();
	else if (tmp.find("DELETE") == 0)
		return validateDelete();

	return false;
}

bool Connection::validateGet(void) {
	
	string tmp = _buffer.data();
	
	return true;
}

bool Connection::validatePost(void) {

	return true;
}

bool Connection::validateDelete(void) {

	return true;
}

bool Connection::validatePut(void) {
	
	return true;
}

bool Connection::validatePatch(void) {

	return true;
}

void Connection::append(std::vector<char> &text, int bytes) {

	if (text.empty())
		return;

	vector<char>::iterator it = text.begin();
	for (; it != text.begin() + bytes; it++)
		_buffer.push_back(*it);

	_time = time(NULL);
}

std::string Connection::getResponse(void) const {
	ostringstream oss;
	oss << "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " << getBuffer().size() << "\r\n\r\n" << getBuffer();

	return oss.str();
}

std::string Connection::getResponse(int bytes) {

	if (_response.empty())
		return "";

	string tmp = _response.substr(0, bytes);
	_response.erase(0, bytes);

	return tmp;
}

int Connection::getFd(void) const {

	return _fd;
}

std::string Connection::getIp(void) const {

	return _ip;
}

std::string Connection::getHost(void) const {

	return _host;
}

std::string Connection::getBuffer(void) const {

	return string().append(_buffer.begin(), _buffer.end());
}

std::string Connection::getMethod(void) const {

	return _method;
}

std::string Connection::getPath(void) const {

	return _path;
}

std::string Connection::getProtocol(void) const {

	return _protocol;
}

std::string Connection::getHeaders(void) const {

	ostringstream oss;

	map<string, string>::const_iterator it = _headers.begin();
	for (; it != _headers.end(); it++)
		oss << it->first << ": " << it->second << endl;
	
	return oss.str();
}

std::string Connection::getBody(void) const {

	return string().append(_body.begin(), _body.end());
}

Server Connection::getServer(void) const {

	return _server;
}

time_t Connection::getTime(void) const {

	return _time;
}

size_t Connection::getResponseSize(void) const {

	return _response.size();
}

std::ostream &operator<<(std::ostream &os, const Connection &src) {
	
	os << "Connection" << endl;
	os << "client_fd: " << src.getFd() << endl;
	os << "IP: " << src.getIp() << endl;
	os << "hostname: " << src.getHost() << endl;
	os << "buffer: " << src.getBuffer() << endl;
	os << "method: " << src.getMethod() << endl;
	os << "path: " << src.getPath() << endl;
	os << "protocol: " << src.getProtocol() << endl;
	os << "request headers: " << src.getHeaders() << endl;
	os << "request body: " << src.getBody() << endl;
	os << "http {\n" << src.getServer() << "\n}" << endl;
	os << "response: " << src.getResponse() << endl;

	return os;
}
