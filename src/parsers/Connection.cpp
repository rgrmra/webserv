#include "Connection.hpp"
#include "parser.hpp"
#include "response.hpp"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Connection::Connection(int fd, string ip)
	: _fd(fd),
	  _ip(ip),
	  _time(time(NULL)),
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
	_send = rhs._send;

	return *this;
}

Connection::~Connection(void) {

}

void Connection::parseRequest(void) {

	_request.parseRequest(_buffer);
	_request.printRequest(); //debug purposes

	_protocol = response::PROTOCOL;
	_code = "200";
	_status = "Ok";

	_headers["Content-Type"] = "text/plain";
	_headers["Content-Length"] = "3";
	if (_buffer.find("Keep-alive: true") != string::npos)
		_headers["Keep-alive"] = "true";
	else
		_headers["Connection"] = "closed";

	_body = "Ok\n";

	_send = true;
	// buildResponse();
	response::pageOk(this);
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

	if (_buffer.find("\r\n\r\n") != string::npos)
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
	if (it == _headers.end())
		return "";

	return it->second;
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

Server Connection::getServer(void) const {

	return _server;
}

time_t Connection::getTime(void) const {

	return _time;
}

void Connection::buildResponse(void) {

	if (_protocol.empty() || _code.empty() || _status.empty())
		response::pageInternalServerError(this);

	ostringstream oss;
	oss <<  _protocol + " " + _code + " " + _status + "\r\n";

	map<string, string>::iterator it = _headers.begin();
	for (; it != _headers.end(); it++)
		oss << it->first + ": " + it->second + "\r\n";

	oss << "\r\n" << _body;

	_response = oss.str();
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

void Connection::setSend(bool send) {

	_send = send;
}

bool Connection::getSend(void) const {

	return _send;
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

	_time = time(NULL);
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
