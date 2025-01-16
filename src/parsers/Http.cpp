/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:00:24 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/16 10:58:13 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sys/types.h>

Http::Http(void) {

}

Http::Http(string &configuration_file) {

	ifstream file(configuration_file.c_str());

	if (not file)
		throw runtime_error(strerror(errno));

	string buffer;
	string line;
	while(getline(file, line)) {
		
		// ignore empty line
		if (not line.size())
			continue;

		// remove comment
		if (static_cast<ssize_t>(line.find_first_of("#")) > -1)
			line = line.substr(0, line.find_first_of("#"));

		// ignore blank line
		if (static_cast<ssize_t>(line.find_first_not_of(" \n\t\r\v\f")) == -1)
			continue;

		// remove blank line at start
		line = line.substr(line.find_first_not_of(" \n\t\r\v\f"), line.size());

		// remove black line at end
		line = line.substr(0, line.find_last_not_of(" \n\t\r\v\f") + 1);

		// concat string
		buffer.append(line);
	}

	// replace tab by space
	for (size_t i = 0; i < buffer.length(); i++)
		if (buffer.at(i) == '\t')
			buffer.at(i) = ' ';

	// erase double spaces
	for (size_t pos = buffer.find("  "); pos != string::npos; pos = buffer.find("  "))
		buffer.erase(pos, 1);

	for (size_t pos = buffer.find(" {"); pos != string::npos; pos = buffer.find(" {"))
		buffer.erase(pos, 1);

	for (size_t pos = buffer.find(" }"); pos != string::npos; pos = buffer.find(" }"))
		buffer.erase(pos, 1);

	for (size_t pos = buffer.find(" ;"); pos != string::npos; pos = buffer.find(" ;"))
		buffer.erase(pos, 1);

	// close file
	file.close();

	Parser::http(*this, buffer);
	// print formated configuration
	cout << buffer << endl;
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

	set<Server>::iterator it = rhs._servers.begin();

	for (; it != rhs._servers.end(); it++)
		_servers.insert(*it);

	return *this;

}

Http::~Http(void) {

}


void Http::setMaxBodySize(string max_body_size) {

	(void) max_body_size;
	_max_body_size = 1000;
}

size_t Http::getMaxBodySize(void) const {
		
	return _max_body_size;
}

void Http::setAcessLog(string access_log) {

	_access_log = access_log;
}

string Http::getAcessLog(void) const {

	return _access_log;
}

void Http::setErrorLog(string error_log) {

	_error_log = error_log;
}

string Http::getErrorLog(void) const {

	return _error_log;
}

void Http::setRoot(string root) {

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

void Http::removeServerByName(string name, string port) {

	(void)name;
	(void)port;
}

void Http::removeServerByHost(string host, string port) {

	(void)host;
	(void)port;
}

set<Server> Http::getServers(void) const {

	return _servers;
}

ostream &operator<<(ostream &os, const Http &src) {

	os << "HTTP:" << endl;
	os << "client_max_body_size: " << src.getMaxBodySize() << endl;
	os << "access_log: " << src.getAcessLog() << endl;
	os << "error_log: " << src.getErrorLog() << endl;
	os << "root: " << src.getRoot() << endl;

	set<Server> servers = src.getServers();

	set<Server>::iterator it = servers.begin();

	for (; it != servers.end(); it++) {
		os << *it << endl;
	}

	return os;
}

