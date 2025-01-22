/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:00:24 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/22 15:01:54 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "Logger.hpp"
#include <cstdlib>
#include <fstream>
#include <set>
#include <string>
#include <cmath>

Http::Http(void) {

}

Http::Http(string &filename) {

	if (Parser::basename(filename) != ".conf")
		throw runtime_error("invalid .conf file format: " + filename);

	ifstream file(filename.c_str());
	if (not file)
		throw runtime_error("failed to open configuration file: " + filename);

	string buffer;

	for (string line; getline(file, line); buffer.append(line)) {
		
		if (line.find("#") != string::npos)
			line = line.substr(0, line.find_first_of("#"));

		Parser::trim(line, " \n\t\r\v\f");
	}

	file.close();

	Parser::replace(buffer, '\t', ' ');

	Parser::erase(buffer, "  ", 1);
	Parser::erase(buffer, " {", 1);
	Parser::erase(buffer, " }", 1);
	Parser::erase(buffer, " ;", 1);

	Parser::http(*this, buffer);

	Logger::info("configuration file parsed: " + filename);
}

Http::Http(const Http &src) {

	*this = src;
}

Http &Http::operator=(const Http &rhs) {
	
	if (this == &rhs)
		return *this;

	_max_body_size = rhs._max_body_size;
	_error_log = rhs._error_log;
	_access_log = rhs._access_log;
	_root = rhs._root;
	_servers = rhs._servers;

	return *this;
}

Http::~Http(void) {

}

void Http::setMaxBodySize(string max_body_size) {

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

size_t Http::getMaxBodySize(void) const {
		
	return _max_body_size;
}

void Http::setAcessLog(string access_log) {

	if (access_log.empty() || not _access_log.empty())
		return;

	_access_log = access_log;
}

string Http::getAcessLog(void) const {

	return _access_log;
}

void Http::setErrorLog(string error_log) {

	if (error_log.empty() || not _error_log.empty())
		return;

	_error_log = error_log;
}

string Http::getErrorLog(void) const {

	return _error_log;
}

void Http::setRoot(string root) {

	if (root.empty())
		return;

	_root = root;
}

string Http::getRoot(void) const {

	return _root;
}


void Http::addServer(Server server) {

	_servers.insert(server);
}

Server Http::getServerByName(string name, string port) const {

	(void)name;
	(void)port;
	return Server();
}

Server Http::getServerByHost(string host, string port) const {

	(void)host;
	(void)port;
	return Server();
}

set<Server> Http::getServers(void) const {

	return _servers;
}

ostream &operator<<(ostream &os, const Http &src) {

	os << "HTTP:" << endl;
	os << "\tclient_max_body_size: " << src.getMaxBodySize() << endl;
	os << "\taccess_log: " << src.getAcessLog() << endl;
	os << "\terror_log: " << src.getErrorLog() << endl;
	os << "\troot: " << src.getRoot() << endl;

	set<Server> servers = src.getServers();

	set<Server>::iterator it = servers.begin();

	for (; it != servers.end(); it++) {
		os << *it << endl;
	}

	return os;
}
