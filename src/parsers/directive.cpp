/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:18:42 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 20:36:54 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "directive.hpp"
#include "parser.hpp"
#include <cmath>
#include <cstdlib>
#include <map>
#include <stdexcept>

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

void directive::setName(string name, list<string> &_name) {

	if (name.empty())
		return;

	list<string> names = parser::split(name, ' ');

	for (list<string>::iterator it = names.begin(); it != names.end(); it++)
		_name.push_back(*it);
}

void directive::setListen(string listen, string &_host, string &_port) {

	if (listen.empty() || not _host.empty() || not _host.empty())
		return;

	list<string> tmp = parser::split(listen, ':');

	if (tmp.size() > 2 || listen.at(0) == ':' || listen.find("::") != string::npos)
		throw runtime_error("invalid listen: " + listen);

	if (tmp.back().find_first_not_of("0123456789") != string::npos)
		throw runtime_error("invalid listen port: " + tmp.back());

	if (strtol(tmp.back().c_str(), NULL, 10) > 65535)
		throw runtime_error("invalid listen port range: " + tmp.back());
	
	_port = tmp.back();

	if (tmp.size() == 1) {
		_host = "0.0.0.0";
		return;
	}

	list<string> host = parser::split(tmp.front(), '.');

	if (host.size() != 4)
		throw runtime_error("invalid list host: " + tmp.front());

	for (list<string>::iterator it = host.begin(); it != host.end(); it++) {
		if ((*it).find_first_not_of("0123456789") != string::npos || *it > "255")
			throw runtime_error("invalid listen host: " + tmp.front());
	}

	_host = tmp.front();
}

void directive::setPath(string path, string &_path) {
	
	if (path.empty())
		return;

	if (path.find_first_of(" ") != string::npos)
		throw runtime_error("invalid path: " + path);

	_path = path;
}

void directive::setIndex(string index, set<string> &_index) {

	if (index.empty())
		return;

	list<string> indexes = parser::split(index, ' ');

	_index.clear();

	for (list<string>::iterator it = indexes.begin(); it != indexes.end(); it++)
		_index.insert(*it);
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

void directive::addErrorPage(string error_page, map<string, string> &_error_pages) {

	if (error_page.empty())
		return;

	list<string> tmp = parser::split(error_page, ' ');

	if (tmp.size() < 2)
		throw runtime_error("invalid error page: " + error_page);

	string path = tmp.back();
	tmp.pop_back();

	for (list<string>::iterator it = tmp.begin(); it != tmp.end(); it++) {
		if ((*it).find_first_not_of("0123456789") != string::npos)
			throw runtime_error("invalid error code: " + *it);

		size_t code = strtol((*it).c_str(), NULL, 10);
		if (code < 100 || code > 599)
			throw runtime_error("invalid error code: " + *it);

		_error_pages[*it] = path;
	}
}

void directive::addMethod(string method, set<string> &_allow_methods) {

	if (method.empty())
		return;

	list<string> methods = parser::split(method, ' ');

	_allow_methods.clear();

	for (list<string>::iterator it = methods.begin(); it != methods.end(); it++) {
		if (*it != "GET" && *it != "POST" && *it != "DELETE")
			throw runtime_error("invalid method: " + *it);
		
		_allow_methods.insert(*it);
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

void directive::setReturn(string value, string &_code, string &_path) {

	list<string> tmp = parser::split(value, ' ');

	if (tmp.size() < 1 || tmp.size() > 2)
		throw runtime_error("invalid return: " + value);

	string code = tmp.front();
	string path = tmp.back();

	if (not parser::validadeHttpCode(code))
		throw runtime_error("invalid return code: " + code);

	_code = code;

	if (tmp.size() == 2)
		_path = path;
}
