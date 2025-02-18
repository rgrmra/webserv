#include "URL.hpp"
#include <iostream>

using namespace std;

URL::URL(string uri) {

	size_t pos = 0;
	size_t scheme_end = uri.find("://", pos);
	
	// Parse scheme
	if (scheme_end != string::npos) {
		_scheme = uri.substr(pos, scheme_end);
		uri.erase(0, scheme_end + 3);
	}

	// Parse user and password (if present)
	size_t user_info_end = uri.find('@', pos);
	if (user_info_end != string::npos) {
		size_t colon_pos = uri.find(':', pos);
		if (colon_pos != string::npos && colon_pos < user_info_end) {
			_user = uri.substr(pos, colon_pos - pos);
			_password = uri.substr(colon_pos + 1, user_info_end - colon_pos - 1);
		} else {
			_user = uri.substr(pos, user_info_end - pos);
		}
		pos = user_info_end + 1;  // Skip '@'
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
	if (path_end == string::npos) {
		path_end = uri.find('#', pos);
	}
	if (path_end != string::npos) {
		_path = uri.substr(pos, path_end - pos);
		pos = path_end;
	} else {
		_path = uri.substr(pos);
		pos = uri.length();
	}

	// Parse query (if present)
	size_t query_end = uri.find('#', pos);
	if (query_end == string::npos) {
		query_end = uri.length();
	}
	size_t query_start = uri.find('?', pos);
	if (query_start != string::npos && query_start < query_end) {
		_query = uri.substr(query_start + 1, query_end - query_start - 1);
	}
}

URL::URL(const URL &src) {

	*this = src;
}

URL &URL::operator=(const URL &rhs) {

	if (this == &rhs)
		return *this;

	_scheme = rhs._scheme;
	_user = rhs._user;
	_password = rhs._password;
	_host = rhs._host;
	_port = rhs._port;
	_path = rhs._path;
	_param = rhs._param;
	_query = rhs._query;

	return *this;
}

URL::~URL(void) {

}

string URL::getScheme(void) const {

	return _scheme;
}

string URL::getUser(void) const {

	return _user;
}

string URL::getPassword(void) const {

	return _host;
}

string URL::getHost(void) const {

	return _host;
}

string URL::getPort(void) const {

	return _port;
}

string URL::getPath(void) const {

	return _path;
}

string URL::getParam(void) const {
	
	return _param;
}

string URL::getQuery(void) const {

	return _query;
}

ostream &operator<<(ostream &os, const URL &src) {

	return os << (src.getScheme().size() ? src.getScheme() + "://" : "")
		+ src.getUser() + (src.getPassword().size() ? ":" + src.getPassword() : "")
		+ (src.getUser().size() ? "@" : "")
		+ src.getPort() + (src.getPort().size() ? ":" + src.getPort() : "") + src.getPath()
		+ (src.getQuery().size() ? "?" + src.getQuery() : "");
}
