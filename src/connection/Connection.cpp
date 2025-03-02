#include "Cgi.hpp"
#include "Connection.hpp"
#include "Http.hpp"
#include "Resource.hpp"
#include "WebServ.hpp"
#include "header.hpp"
#include "request.hpp"
#include "response.hpp"
#include <list>
#include <sstream>
#include <string>
#include <sys/epoll.h>

using namespace std;

Connection::Connection(int fd, string ip)
	: AStream(fd, ip),
	  _file(NULL),
	  _has_content_lenght(false),
	  _has_transfer_enconding(false) {

}

Connection::Connection(const Connection &src)
	: AStream(src) {

	*this = src;
}

Connection &Connection::operator=(const Connection &rhs) {

	if (this == &rhs)
		return *this;

	_host = rhs._host;
	_method = rhs._method;
	_path = rhs._path;
	_protocol = rhs._protocol;
	_code = rhs._code;
	_status = rhs._status;
	_headers = rhs._headers;
	_body = rhs._body;
	_file = rhs._file;
	_server = rhs._server;
	_has_content_lenght = rhs._has_content_lenght;
	_has_transfer_enconding = rhs._has_transfer_enconding;

	return *this;
}

Connection::~Connection(void) {

	if (_file)
		delete _file;
}

void Connection::parseRequest(void) {

	istringstream iss(_input);
	string line;

	while (getline(iss, line) && !line.empty() && _code.empty()) {

		if (_step == IStream::BODY)
			break;

		if (_step < IStream::HEADERS) {
			request::parseRequest(this, line);

			size_t pos = _input.find("\r\n");
			if (pos != string::npos)
				_input = _input.substr(pos + 2);
		} else
			request::parseRequest(this, _input);
	}

	if (_step < IStream::BODY)
		return;
	else if (_step == IStream::HEADERS && _headers.empty())
		return response::pageBadRequest(this);
	else if (_code.empty() && _host.empty())
		return response::pageBadRequest(this);
	else if (_code.empty())
		return response::pageOK(this);
}

void Connection::setHost(string host) {

	host = _host;
}

string Connection::getHost(void) const {

	return _host;
}

void Connection::processInput(size_t bytes) {

	(void) bytes;

	if (_input.find("\r\n") != string::npos)
		parseRequest();
}

std::string Connection::getInput(void) const {

	return _input;
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

		Http *http = Http::getInstance();
		_server = http->getServerByName(tmp.front());
		if (_server.empty())
			_server = http->getServerByListen(value);
		if (_server.empty())
			_server = http->getServerByListen(_id);
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

void Connection::setResource(Resource *file) {

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

void Connection::buildResponse(void) {

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

	_output = oss.str() + "\r\n";
	_step = IStream::RESPONSE;
	WebServ::getInstance()->controlEpoll(_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
}

void Connection::processOutput(size_t bytes) {

	if (_file)
		_output += _file->getData(bytes);

	if (_output.empty()) {
		if ((*this)[header::CONNECTION] != "keep-alive")
			_step = IStream::CLOSE;
		else
			_step = IStream::KEEPALIVE;
	}
}

bool Connection::hasContentLenght(void) const {

	return _has_content_lenght;
}

bool Connection::hasTransferEnconding(void) const {

	return _has_transfer_enconding;
}

void Connection::setQueryString(string query_string) {

	_query_string = query_string;
}

void Connection::resetConnection(void) {

	_input.clear();
	_size = 0;
	_step = NONE;

	_host.clear();
	_method.clear();
	_path.clear();
	_protocol.clear();
	_code.clear();
	_status.clear();
	_headers.clear();
	_body.clear();

	if (_file) {
		delete _file;
		_file = NULL;
	}

	_query_string.clear();
	_time = time(NULL);

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

void Connection::sendTimeOut(void) {

	if (_file && dynamic_cast<Cgi *>(_file))
		WebServ::getInstance()->controlEpoll(_file->getFd(), 0, EPOLL_CTL_DEL);
	response::pageGatewayTimeOut(this);
}

ostream &operator<<(ostream &os, const Connection &src) {

	os << "Connection" << endl;
	os << "client_fd: " << src.getFd() << endl;
	//os << "IP: " << src.getIp() << endl;
	os << "hostname: " << src.getHost() << endl;
	//os << "buffer: " << src.getBuffer() << endl;
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
