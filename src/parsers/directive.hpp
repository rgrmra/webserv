#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "Http.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <bitset>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace directive {

	void setAcessLog(std::string access_log, std::string &_access_log);
	void setErrorLog(std::string error_log, std::string &_error_log);
	bool validateHttpListen(std::string listen);
	bool validateHttpHost(std::string host);
	bool validateHttpPort(std::string port);
	void addListen(std::string listen, std::vector<std::string> &_listen);
	bool validateName(std::string name);
	void addName(std::string name, std::vector<std::string> &_name);
	void setURI(std::string uri, std::string &_uri);
	bool validateHttpMethod(std::string method);
	void addMethod(std::string method, std::set<std::string> &_allow_methods);
	void setDenyMethods(std::string deny_methods, bool &_deny_methods);
	void setRoot(std::string root, std::string &_root);
	void setAutoIndex(std::string autoindex, std::bitset<2> &_autoindex);
	void setMaxBodySize(std::string max_body_size, std::size_t &_max_body_size);
	void addIndex(std::string index, std::set<std::string> &_index);
	void addErrorPage(std::string error_page, std::map<std::string, std::string> &_error_pages);
	void mergeErrorPages(std::map<std::string, std::string> error_pages, std::map<std::string, std::string> &_error_pages);
	void addServer(Server server, std::vector<Server> &_servers);
	bool validateHttpCode(std::string code);
	void setReturn(std::string value, std::string &_code, std::string &_uri);
	void setHttpDefaultValues(Http &http);
	void setServerDefaultValues(Http &http, Server &server);
	void setLocationDefaultValues(Server &server, Location &location);
}

#endif /* DIRECTIVE_HPP */