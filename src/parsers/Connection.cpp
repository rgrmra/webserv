#include "Connection.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "Request.hpp"
#include "response.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Connection::Connection(int fd, string ip)
	: _fd(fd),
	  _ip(ip),
	  _time(time(NULL)),
	  _startline_parsed(false),
	  _headers_parsed(false),
	  _send(false) {

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
	_code = rhs._code;
	_status = rhs._status;
	_headers = rhs._headers;
	_body = rhs._body;
	_server = rhs._server;
	_response = rhs._response;
	_time = rhs._time;
	_startline_parsed = rhs._startline_parsed;
	_headers_parsed = rhs._headers_parsed;
	_send = rhs._send;

	return *this;
}

Connection::~Connection(void) {

}

void Connection::parseRequest(void) {

	istringstream iss(_buffer);
	string line;

	while (getline(iss, line) && !line.empty()) {

		if (!_headers_parsed) {
		request::parseRequest(this, line);

		size_t pos = _buffer.find("\r\n");
		if (pos != string::npos)
			_buffer = _buffer.substr(pos + 2);
		} else {
			request::parseRequest(this, _buffer);
		}
	}

	if (!_send)
		return;

	if (_code.empty()) {
		_code = "200";
		_status = "Ok";
	}

	buildResponse();
}

int Connection::getFd(void) const {

	return _fd;
}

string Connection::getIp(void) const {

	return _ip;
}

void Connection::setHost(string host) {

	host = _host;
}

string Connection::getHost(void) const {

	return _host;
}

void Connection::append(vector<char> &text, int bytes) {

	if (!bytes || text.empty())
		return;

	_buffer.append(text.begin(), text.begin() + bytes);

	if (_buffer.find("\r\n") != string::npos)
		parseRequest();

	_time = time(NULL);
}

string Connection::getBuffer(void) const {

	return string().append(_buffer.begin(), _buffer.end());
}

void Connection::setMethod(string &method) {

	_method = method;
}

string Connection::getMethod(void) const {

	return _method;
}

void Connection::setPath(string path) {

	_path = path;
}

string Connection::getPath(void) const {

	return _path;
}

void Connection::setProtocol(string protocol) {

	_protocol = protocol;
}

string Connection::getProtocol(void) const {

	return _protocol;
}

void Connection::setCode(string code) {

	_code = code;
}

void Connection::setCode(size_t code) {

	_code = parser::toString(code);
}

string Connection::getCode(void) const {

	return _code;
}

void Connection::setStatus(string status) {

	_status = status;
}

string Connection::getStatus(void) const {

	return _status;
}

void Connection::addHeader(string key, string value) {

	if (key == header::HOST) {
		if (value.empty())
			return response::pageBadRequest(this);

		_host = value;
	}

	_headers[key] = value;
}

void Connection::addHeader(string key, size_t value) {

	_headers[key] = parser::toString(value);
}

void Connection::setHeaders(map<string, string> headers) {

	_headers = headers;
}

string Connection::getHeaderByKey(string key) const {

	map<string, string>::const_iterator it = _headers.find(key);
	if (it->first == key)
		return it->second;

	return "";
}

string Connection::getHeaders(void) const {

	ostringstream oss;

	map<string, string>::const_iterator it = _headers.begin();
	for (; it != _headers.end(); it++)
		oss << it->first << ": " << it->second << endl;

	return oss.str();
}

void Connection::setBody(string body) {

	_body = body;
}

string Connection::getBody(void) const {

	return _body;
}

void Connection::setServer(Server server) {

	_server = server;
}

Server Connection::getServer(void) const {

	return _server;
}

time_t Connection::getTime(void) const {

	return _time;
}

void Connection::buildResponse(void) {
	// if (_protocol.empty() || _code.empty() || _status.empty())
	// 	response::pageInternalServerError(this);
	response::setResponse(this);

	if (getHeaderByKey(header::CONNECTION) != "keep-alive")
		_headers[header::CONNECTION] = "close";

	_headers[header::SERVER] = "webserv/0.1.0";

	ostringstream oss;
	oss <<  _protocol + " " + _code + " " + _status + "\r\n";

	map<string, string>::iterator it = _headers.begin();
	for (; it != _headers.end(); it++)
		oss << it->first + ": " + it->second + "\r\n";

	oss << "\r\n" << _body;

	_response = oss.str();
	_send = true;
}

string Connection::getResponse(int bytes) {

	if (_response.empty())
		return "";

	string tmp = _response.substr(0, bytes);
	_response.erase(0, bytes);

	return tmp;
}

string Connection::getResponse(void) const {

	return _response;
}

size_t Connection::getResponseSize(void) const {

	return _response.size();
}
void Connection::setStartLineParsed(bool value) {

	_startline_parsed = value;
}

bool Connection::getStartLineParsed(void) const {

	return _startline_parsed;
}

void Connection::setHeadersParsed(bool value) {

	_headers_parsed = value;
}

bool Connection::getHeadersParsed(void) const {

	return _headers_parsed;
}

void Connection::setSend(bool send) {

	_send = send;
}

bool Connection::getSend(void) const {

	return _send;
}

void Connection::setQueryString(string query_string) {

	_query_string = query_string;
}

std::string Connection::getQueryString(void) const {

	return _query_string;
}

void Connection::resetConnection(void) {

	_host.clear();
	_buffer.clear();
	_method.clear();
	_path.clear();
	_protocol.clear();
	_code.clear();
	_status.clear();
	_headers.clear();
	_body.clear();
	_response.clear();
	_query_string.clear();

	_time = time(NULL);

	_startline_parsed = false;
	_headers_parsed = false;
	_send = false;
}

ostream &operator<<(ostream &os, const Connection &src) {

	os << "Connection" << endl;
	os << "client_fd: " << src.getFd() << endl;
	os << "IP: " << src.getIp() << endl;
	os << "hostname: " << src.getHost() << endl;
	os << "buffer: " << src.getBuffer() << endl;
	os << "method: " << src.getMethod() << endl;
	os << "path: " << src.getPath() << endl;
	os << "protocol: " << src.getProtocol() << endl;
	os << "code: " << src.getCode() << endl;
	os << "status: " << src.getStatus() << endl;
	os << "request headers: " << src.getHeaders() << endl;
	os << "request body: " << src.getBody() << endl;
	os << "http {\n" << src.getServer() << "\n}" << endl;
	os << "response: " << src.getResponse() << endl;

	return os;
}
