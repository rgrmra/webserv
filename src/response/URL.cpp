#include "URL.hpp"
#include <csetjmp>
#include <iostream>

using namespace std;

URL::URL(void) {

}

URL::URL(string uri) {

	size_t pos = 0;
	size_t scheme_end = uri.find("://", pos);
	
	// Parse scheme
	if (scheme_end != string::npos) {
		_scheme = uri.substr(pos, scheme_end);
		uri.erase(0, scheme_end + 3);
	}

	// Parse host
	size_t host_end = uri.find(':', pos);
	if (host_end == string::npos) {
		host_end = uri.find('/', pos);
	}
	_host = uri.substr(pos, host_end - pos);
	pos = host_end;

	// Parse port (if present)
	if (uri[pos] == ':') {
		size_t port_end = uri.find('/', pos);
		_port = uri.substr(pos + 1, port_end - pos - 1);
		pos = port_end;
	}

	// Parse path
	size_t path_end = uri.find('?', pos);
	if (path_end != string::npos) {
		_path = uri.substr(pos, path_end - pos);
		pos = path_end;
	} else {
		_path = uri.substr(pos);
		pos = uri.length();
	}

	size_t query_start = uri.find('?', pos);
	if (query_start != string::npos) {
		_query = uri.substr(query_start + 1);
	}
}

URL::URL(const URL &src) {

	*this = src;
}

URL &URL::operator=(const URL &rhs) {

	if (this == &rhs)
		return *this;

	_scheme = rhs._scheme;
	_host = rhs._host;
	_port = rhs._port;
	_path = rhs._path;
	_query = rhs._query;

	return *this;
}

URL::~URL(void) {

}

void URL::setScheme(string scheme) {

	_scheme = scheme;
}

string URL::getScheme(void) const {

	return _scheme;
}

void URL::setHost(string host) {

	_host = host;
}

string URL::getHost(void) const {

	return _host;
}

void URL::setPort(string port) {

	_port = port;
}

string URL::getPort(void) const {

	return _port;
}

void URL::setPath(string path) {

	_path = path;
}

string URL::getPath(void) const {

	return _path;
}

void URL::setQuery(string query) {

	_query = query;
}

string URL::getQuery(void) const {

	return _query;
}

std::string URL::getLocation(void) {

	return (_scheme.size() ? _scheme : "http") + "://"
		+ _host + (_port.size() ? ":" + _port : "") + _path;
}

void URL::clear(void) {

	_scheme.clear();
	_host.clear();
	_port.clear();
	_path.clear();
	_query.clear();
}

ostream &operator<<(ostream &os, const URL &src) {

	return os << (src.getScheme().size() ? src.getScheme() + "://" : "http://")
		+ src.getHost() + (src.getPort().size() ? ":" + src.getPort() : "")
		+ src.getPath() + (src.getQuery().size() ? "?" + src.getQuery() : "");
}
