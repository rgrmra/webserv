#include "directive.hpp"
#include "Http.hpp"
#include "Location.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "Server.hpp"
#include <limits>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

void directive::setAcessLog(string access_log, string &_access_log) {

	if (access_log.empty())
		return;

	if (access_log.find_first_of(" ") != string::npos)
		throw runtime_error("invalid access_log: " + access_log);

	_access_log = access_log;
}

void directive::setErrorLog(string error_log, string &_error_log) {

	if (error_log.empty())
		return;

	if (error_log.find_first_of(" ") != string::npos)
		throw runtime_error("invalid error_log: " + error_log);

	_error_log = error_log;
}

bool directive::validateHttpListen(string listen) {

	if (listen.empty())
		return false;

	if (listen.find_first_not_of(".:0123456789") != string::npos)
		return false;

	if (listen.at(0) == ':' || listen.at(listen.size() - 1) == ':')
		return false;

	if (listen.find("::") != string::npos)
		return false;

	list<string> tmp = parser::split(listen, ':');
	if (tmp.size() > 2)
		return false;

	return true;
}

bool directive::validateHttpHost(string host) {

 	if (host.empty())
          return false;

	if (host.find_first_not_of(".0123456789") != string::npos)
		return false;

	if (host.find("..") != string::npos)
		return false;

	if (host.at(0) == '.' || host.at(host.size() - 1) == '.')
		return false;

	list<string> octets = parser::split(host, '.');

	if (octets.size() != 4)
		return false;

	list<string>::iterator it = octets.begin();
	while (it != octets.end()) {
		if (parser::toSizeT(*it) > 255)
			return false;

		it++;
	}

	return true;
}

bool directive::validateHttpPort(string port) {

  if (port.empty())
    return false;

  if (port.find_first_not_of("0123456789") != string::npos)
		return false;

	size_t portNum = parser::toSizeT(port);

	if (portNum > 65535)
		return false;

	// Check for well-known ports (0-1023)
	if (portNum < 1024) {
		// Allow common HTTP/HTTPS ports
		if (portNum != 80 && portNum != 443)
			return false;
	}

	// For other ports, prefer using registered ports (1024-49151)
	// rather than dynamic/private ports (49152-65535)
	if (portNum >= 1024 && portNum <= 49151)
		return true;

	return true;
}

void directive::addListen(string listen, vector<string> &_listen) {

	if (listen.empty())
		return;

	string host = "0.0.0.0";
	string port = "80";

	if (not directive::validateHttpListen(listen))
		throw runtime_error("invalid listen: " + listen);

	list<string> tmp = parser::split(listen, ':');

	if (tmp.size() >= 1 && directive::validateHttpPort(tmp.back()))
		port = tmp.back();
	else if (tmp.size() == 1 && directive::validateHttpHost(tmp.back()))
		host = tmp.back();
	else
		throw runtime_error("invalid listen: " + listen);

	if (tmp.size() == 2 && directive::validateHttpHost(tmp.front()))
		host = tmp.front();
	else if (tmp.size() == 2)
		throw runtime_error("invalid host: " + tmp.front());

	listen = host + ":" + port;

	for (vector<string>::iterator it = _listen.begin(); it != _listen.end(); it++) {
		if (*it == listen)
			throw runtime_error("duplicated listen: " + listen);
	}

	_listen.push_back(listen);
}

bool directive::validateName(string name) {
	if (name.empty())
		return false;

	if (name[0] == '-' || name[0] == '.' ||
		name[name.size() - 1] == '-' || name[name.size() - 1] == '.') {
		return false;
		}

	for (size_t i = 0; i < name.size() - 1; i++) {
		if ((name[i] == '-' && name[i + 1] == '-') ||
			(name[i] == '.' && name[i + 1] == '.')) {
			return false;
		}
	}

	for (size_t i = 0; i < name.size(); i++) {
		char ch = name[i];
		if (!isalnum(ch) && ch != '-' && ch != '.') {
			return false;
		}
	}

	return true;
}

void directive::addName(string name, vector<string> &_name) {
	if (name.empty())
		return;

	list<string> names = parser::split(name, ' ');

	list<string>::iterator it = names.begin();
	while (it != names.end()) {
		if (not directive::validateName(*it))
			throw runtime_error("invalid server name: " + *it);

		_name.push_back(parser::toLower(*it));

		it++;
	}
}

void directive::setURI(string uri, string &_uri) {
	if (uri.empty())
		return;

    // TODO: implement uriValidation function
	if (uri.find_first_of(" ") != string::npos)
		throw runtime_error("invalid path: " + uri);

	_uri= uri;
}

bool directive::validateHttpMethod(string method) {
	list<string> allowed_methods = parser::split(DEFAULT_ALLOW_METHODS, ' ');

	list<string>::iterator it = allowed_methods.begin();
	for (; it != allowed_methods.end(); it++)
		if (*it == method)
			return true;

	return false;
}

void directive::addMethod(string method, set<string> &_allow_methods) {
	if (method.empty())
		return;

	list<string> methods = parser::split(method, ' ');

	_allow_methods.clear();

	list<string>::iterator it = methods.begin();
	for (; it != methods.end(); it++) {
		if (not validateHttpMethod(*it))
			throw runtime_error("invalid method: " + *it);

		_allow_methods.insert(*it);
	}
}

void directive::setDenyMethods(string deny_methods, bool &_deny_methods) {
	if (deny_methods.empty())
		return;

	if (deny_methods == "all")
		_deny_methods = true;
	else
		throw runtime_error("invalid deny: " + deny_methods);
}


void directive::setRoot(string root, string &_root) {
	if (root.empty())
		return;

	if (root.find_first_of(" ") != string::npos)
		throw runtime_error("invalid root: " + root);

	_root = root;
}

void directive::setAutoIndex(string autoindex, bitset<2> &_autoindex) {
	if (autoindex.empty())
		return;

	if (autoindex == "on")
		_autoindex = AUTOINDEX_ON;
	else if (autoindex == "off")
		_autoindex = AUTOINDEX_OFF;
	else
		throw runtime_error("invalid autoindex: " + autoindex);
}

void directive::setMaxBodySize(string max_body_size, size_t &_max_body_size) {
	if (max_body_size.empty())
		return;

	size_t pos = max_body_size.find_first_not_of("0123456789");

	string format;
	if (pos != string::npos)
		format = max_body_size.substr(pos, max_body_size.size() - pos);

	size_t tmp = parser::toSizeT(max_body_size);
	if (tmp == 0)
		_max_body_size = numeric_limits<size_t>::max();
	else
		_max_body_size = tmp;

	if (format.empty() || format == "B")
		_max_body_size *= BYTE;
	else if (format == "K")
		_max_body_size *= KILOBYTE;
	else if (format == "M")
		_max_body_size *= MEGABYTE;
	else if (format == "G")
		_max_body_size *= GIGABYTE;
	else
		throw runtime_error("invalid value to max_body_size: " + max_body_size);
}

void directive::addIndex(string index, set<string> &_index) {
	if (index.empty())
		return;

	list<string> indexes = parser::split(index, ' ');

	_index.clear();

	list<string>::iterator it = indexes.begin();
	while (it != indexes.end()) {
		_index.insert(*it);
		it++;
	}
}

void directive::addErrorPage(string error_page, map<string, string> &_error_pages) {
	if (error_page.empty())
		return;

	list<string> tmp = parser::split(error_page, ' ');

	if (tmp.size() < 2)
		throw runtime_error("invalid error page: " + error_page);

	string path = tmp.back();
	tmp.pop_back();

	for (list<string>::iterator it = tmp.begin(); it != tmp.end(); it++) {
		if (not directive::validateHttpCode(*it))
			throw runtime_error("invalid error code: " + *it);

		_error_pages[*it] = path;
	}
}

void directive::mergeErrorPages(map<string, string> error_pages, map<string, string> &_error_pages) {
	map<string, string>::iterator error_pagesIt = error_pages.begin();
	for (; error_pagesIt != error_pages.end(); error_pagesIt++)
		if (_error_pages[error_pagesIt->first].empty())
			_error_pages[error_pagesIt->first] = error_pagesIt->second;
}

bool directive::validateHttpCode(string code) {
	if (code.find_first_not_of("0123456789") != string::npos)
		return false;

	size_t tmp = parser::toSizeT(code);
	if (tmp < 100 || tmp > 599)
		return false;

	return true;
}

void directive::setReturn(string value, string &_code, string &_uri) {
	if (value.empty())
		return;

	list<string> tmp = parser::split(value, ' ');

	if (tmp.size() < 1 || tmp.size() > 2)
		throw runtime_error("invalid return: " + value);

	if (not directive::validateHttpCode(tmp.front()))
		throw runtime_error("invalid return code: " + tmp.front());

	_code = tmp.front();

	if (tmp.size() == 2)
		_uri = tmp.back();
}

void directive::addServer(Server server, vector<Server> &_servers) {
	vector<Server>::iterator serverIt = _servers.begin();
	for (; serverIt != _servers.end(); serverIt++) {

		vector<string> exitenListen = serverIt->getListen();
		vector<string>::iterator exitenListenIt = exitenListen.begin();
		for (; exitenListenIt != exitenListen.end(); exitenListenIt++) {

			list<string> tmp = parser::split(*exitenListenIt, ':');

			vector<string> newListen = server.getListen();
			vector<string>::iterator newListenIt = newListen.begin();
			for (; newListenIt != newListen.end(); newListenIt++) {

				list<string> tmp2 = parser::split(*newListenIt, ':');
				if ((tmp.front() == "0.0.0.0" || tmp.front() == tmp2.front()) && tmp.back() == tmp2.back()) {

					logger::warning("conflicting server name \"" + (server.getNames().size()?  server.getNames()[0] : "") + "\" on " + *newListenIt + ", ignored");
					newListen.erase(newListenIt);
					server.setListen(newListen);

					return directive::addServer(server, _servers);
				}
			}
		}
	}
	_servers.push_back(server);
}

void directive::setHttpDefaultValues(Http &http) {
	if (http.getMaxBodySize() == 0)
		http.setMaxBodySize(DEFAULT_MAX_BODY_SIZE);

	if (http.getIndexes().size() == 0)
		http.addIndex(DEFAULT_INDEXES);

	if (http.getAccessLog().empty())
		http.setAccessLog(DEFAULT_ACCESS_LOG);

	if (http.getAutoIndexBitSet() == AUTOINDEX_NOT_SET)
		http.setAutoIndex(AUTOINDEX_OFF);

	if (http.getErrorLog().empty())
		http.setErrorLog(DEFAULT_ERROR_LOG);

	if (http.getRoot().empty())
		http.setRoot(DEFAULT_ROOT);

	vector<Server> servers = http.getServers();
	vector<Server>::iterator serversIt = servers.begin();
	for (; serversIt != servers.end(); serversIt++)
		directive::setServerDefaultValues(http, *serversIt);

	http.setServers(servers);
}

void directive::setServerDefaultValues(Http &http, Server &server) {
	if (server.getMaxBodySize() == 0)
		server.setMaxBodySize(parser::toString(http.getMaxBodySize()));
	
	if (server.getRoot().empty())
		server.setRoot(http.getRoot());

	if (server.getIndexes().empty())
		server.setIndexes(http.getIndexes());

	if (server.getAutoIndexBitSet() == AUTOINDEX_NOT_SET)
		server.setAutoIndex(http.getAutoIndexBitSet());

	map<string, string> error_pages = server.getErrorPages();
	directive::mergeErrorPages(http.getErrorPages(), error_pages);
	server.setErrorPages(error_pages);

	map<string, Location> locations = server.getLocations();
	map<string, Location>::iterator locationsIt = locations.begin();
	for (; locationsIt != locations.end(); locationsIt++)
		directive::setLocationDefaultValues(server, locationsIt->second);

	server.setLocations(locations);
}

void directive::setLocationDefaultValues(Server &server, Location &location) {
	if (location.getRoot().empty())
		location.setRoot(server.getRoot());

	if (location.getIndexes().empty())
		location.setIndexes(server.getIndexes());

	if (location.getDenyMethods() == false)
		location.addMethod(DEFAULT_ALLOW_METHODS);

	if (location.getAutoIndexBitSet() == AUTOINDEX_NOT_SET)
		location.setAutoIndex(server.getAutoIndexBitSet());

	if (location.getMaxBodySize() == 0)
		location.setMaxBodySize(parser::toString(server.getMaxBodySize()));
	
	map<string, string> error_pages = location.getErrorPages();
	directive::mergeErrorPages(server.getErrorPages(), error_pages);
	location.setErrorPages(error_pages);
}
