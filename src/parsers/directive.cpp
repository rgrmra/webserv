/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:18:42 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/25 14:34:52 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "directive.hpp"
#include "parser.hpp"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <string>

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

bool directive::validateName(string name) {

	if (name.at(0) == '-' || name.at(name.size() - 1) == '-')
		return false;

	if (name.at(0) == '.' || name.at(name.size() - 1) == '.')
		return false;

	if (name.find("..") != string::npos)
		return false;

	for (size_t i = 0; i < name.size(); i++)
		if (not isalnum(name.at(i)) && name.at(i) != '-' && name.at(i) != '.')
			return false;

	for (size_t i = 0; i < name.size(); i++) {
		if (name.at(i) != '.')
			continue;

		if (name.at(i - 1) == '-' || name.at(i + 1) == '-')
			return false;
	}

	return true;
}

void directive::setName(string name, list<string> &_name) {

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
		if (strtol(it->c_str(), NULL, 10) > 255)
			return false;

		it++;
	}

	return true;
}

bool directive::validateHttpPort(string port) {

	if (port.find_first_not_of("0123456789") != string::npos)
		return false;

	if (strtol(port.c_str(), NULL, 10) > 65535)
		return false;

	return true;
}

void directive::setListen(string listen, string &_host, string &_port) {

	if (not _host.empty() || not _host.empty())
		return;
	
	if (not directive::validateHttpListen(listen))
		throw runtime_error("invalid listen: " + listen);

	list<string> tmp = parser::split(listen, ':');

	if (not directive::validateHttpPort(tmp.back()))
		throw runtime_error("invalid port: " + tmp.back());

	_port = tmp.back();

	if (tmp.size() == 1)
		_host = "0.0.0.0";
	else if (not directive::validateHttpHost(tmp.front()))
		throw runtime_error("invalid host: " + tmp.front());
	else
		_host = tmp.front();
}

void directive::setURI(string uri, string &_uri) {
	
	if (uri.empty())
		return;

	if (uri.find_first_of(" ") != string::npos)
		throw runtime_error("invalid path: " + uri);

	_uri= uri;
}

void directive::setIndex(string index, set<string> &_index) {

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

void directive::setRoot(string root, string &_root) {

	if (root.empty())
		return;

	if (root.find_first_of(" ") != string::npos)
		throw runtime_error("invalid root: " + root);

	_root = root;
}

void directive::setMaxBodySize(string max_body_size, size_t &_max_body_size) {

	if (max_body_size.empty())
		return;

	size_t pos = max_body_size.find_first_not_of("0123456789");

	if (pos == string::npos)
		return;

	string format = max_body_size.substr(pos, max_body_size.size() - pos);

	_max_body_size = strtol(max_body_size.c_str(), NULL, 10);

	if (format == "KB" || format == "K")
		_max_body_size *= 1024;
	else if (format == "MB" || format == "M")
		_max_body_size *= pow(1024, 2);
	else if (format == "GB" || format == "G")
		_max_body_size *= pow(1024, 3);
	else
		throw runtime_error("invalid value to max_body_size: " + max_body_size);
}

void directive::setErrorPage(string error_page, map<string, string> &_error_pages) {

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

bool directive::validateHttpMethod(string method) {

	list<string> allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	allowed_methods.push_back("DELETE");

	list<string>::iterator it = allowed_methods.begin();
	while (it != allowed_methods.end()) {
		if (*it == method)
			return true;

		it++;
	}

	return false;
}

void directive::setMethods(string method, set<string> &_allow_methods) {

	if (method.empty())
		return;

	list<string> methods = parser::split(method, ' ');
	list<string> allowed_methods = parser::split("GET POST PUT PATCH DELETE", ' ');

	_allow_methods.clear();

	list<string>::iterator it = methods.begin();
	while (it != methods.end()) {
		if (not validateHttpMethod(*it))
			throw runtime_error("invalid method: " + *it);

		_allow_methods.insert(*it);

		it++;
	}
}

void directive::setAutoIndex(string autoindex, bool &_autoindex) {

	if (autoindex.empty())
		return;

	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw runtime_error("invalid autoindex: " + autoindex);
}

bool directive::validateHttpCode(string code) {

	if (code.find_first_not_of("0123456789") != string::npos)
		return false;

	size_t tmp = strtol(code.c_str(), NULL, 10);
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
