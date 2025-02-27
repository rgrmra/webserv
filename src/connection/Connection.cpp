#include "Connection.hpp"
#include "Cgi.hpp"
#include "Location.hpp"
#include "Page.hpp"
#include "Server.hpp"
#include "header.hpp"
#include "Http.hpp"
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
	  _file(NULL),
	  _time(time(NULL)),
	  _startline_parsed(false),
	  _headers_parsed(false),
	  _send(false),
	  _has_content_lenght(false),
	  _has_transfer_enconding(false),
	  _transfers(0) {

	_http = Http::getInstance();
}

Connection::Connection(const Connection &src) {

	*this = src;
}

Connection &Connection::operator=(const Connection &rhs) {

	if (this == &rhs)
		return *this;

	_http = rhs._http;
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
	_file = rhs._file;
	_server = rhs._server;
	_response = rhs._response;
	_time = rhs._time;
	_startline_parsed = rhs._startline_parsed;
	_headers_parsed = rhs._headers_parsed;
	_send = rhs._send;
	_transfers = rhs._transfers;
	_has_content_lenght = rhs._has_content_lenght;
	_has_transfer_enconding = rhs._has_transfer_enconding;

	return *this;
}

Connection::~Connection(void) {

	if (_file)
		delete _file;
}

void Connection::parseRequest(void) {

	istringstream iss(_buffer);
	string line;

	while (getline(iss, line) && !line.empty() && _code.empty()) {

		if (_send)
			break;

		if (!_headers_parsed) {
			request::parseRequest(this, line);

			size_t pos = _buffer.find("\r\n");
			if (pos != string::npos)
				_buffer = _buffer.substr(pos + 2);
		} else
			request::parseRequest(this, _buffer);
	}

	if (!_send)
		return;
	else if (_headers_parsed && _headers.empty())
		return response::pageBadRequest(this);
	else if (_code.empty() && _host.empty())
		return response::pageBadRequest(this);
	else if (_code.empty())
		return response::pageOK(this);
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

void Connection::setUri(string uri) {

	_uri = uri;
}

string Connection::getUri(void) const {

	return _uri;
}

void Connection::setPath(string path) {

	_path = path;
}

string Connection::getPath(void) const {

	return _path;
}

std::string Connection::getQueryString(void) const {
	return _query_string;
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

	if (value.empty())
		return;

	if (key == header::HOST) {

		_host = value;

		list<string> tmp = parser::split(value, ':');

		_server = _http->getServerByName(tmp.front());
		if (_server.empty())
			_server = _http->getServerByListen(value);
		if (_server.empty())
			_server = _http->getServerByListen(_ip);
	}

	if (key == header::CONTENT_LENGTH)
		_has_content_lenght = true;

	if (key == header::TRANSFER_ENCONDING)
		_has_transfer_enconding = true;

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

void Connection::setFile(AFile *file) {

	if (_file)
		delete _file;

	_file = file;
}

void Connection::setServer(Server server) {

	_server = server;
}

Server &Connection::getServer(void){

	return _server;
}

void Connection::setLocation(Location location) {

	_location = location;
}

Location &Connection::getLocation(void) {

	return _location;
}

time_t Connection::getTime(void) const {

	return _time;
}

void Connection::buildResponse(void) {

	if (dynamic_cast<Cgi *>(_file) && _file->empty())
		return;
	if (_file && _file->empty())
		return response::pageNotFound(this);

	if (getHeaderByKey(header::CONNECTION) != "keep-alive")
		_headers[header::CONNECTION] = "close";
	else
		_transfers++;

	if (_file) {
		_headers[header::CONTENT_LENGTH] = parser::toString(_file->getSize());
		_headers[header::CONTENT_TYPE] = _file->getMime();
	}
	_headers[header::SERVER] = "webserv/0.1.0";
	ostringstream oss;
	oss <<  _protocol + " " + _code + " " + _status + "\r\n";

	map<string, string>::iterator it = _headers.begin();
	for (; it != _headers.end(); it++)
		oss << it->first + ": " + it->second + "\r\n";

	_response = oss.str() + "\r\n";
}

string Connection::getResponse(int bytes) {

	if (_file)
		_response += _file->getBuffer(bytes);

	if (_response.empty())
		return "";

	string tmp = _response.substr(0, bytes);
	_response.erase(0, bytes);

	_time = time(NULL);

	return tmp;
}

string Connection::getResponse(void) {

	if (dynamic_cast<Page *>(_file))
		_response += _file->getBuffer(_file->getSize());

	_time = time(NULL);

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

bool Connection::hasContentLenght(void) const {

	return _has_content_lenght;
}

bool Connection::hasTransferEnconding(void) const {

	return _has_transfer_enconding;
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

size_t Connection::getTransfers(void) const {
  return _transfers;
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

	delete _file;
	_file = NULL;

	_response.clear();
	_query_string.clear();
	_time = time(NULL);

	_startline_parsed = false;
	_headers_parsed = false;
	_send = false;
	_has_content_lenght = false;
	_has_transfer_enconding = false;
}

std::string Connection::operator[](std::string key) {

	static string empty;

	map<string, string>::iterator it = _headers.find(key);
	if (it->first == key)
		return it->second;

	return empty ;
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
	//os << "http {\n" << src.getServer() << "\n}" << endl;
	//os << "response: " << src.getResponse() << endl;

	return os;
}
