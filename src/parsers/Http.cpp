/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:00:24 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 18:08:04 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Server.hpp"
#include "directive.hpp"
#include "parser.hpp"
#include "logger.hpp"
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
#include <cmath>

Http::Http(void) {

}

Http::Http(string &filename) {

	if (parser::basename(filename) != ".conf")
		throw runtime_error("invalid .conf file format: " + filename);

	ifstream file(filename.c_str());
	if (not file)
		throw runtime_error("failed to open configuration file: " + filename);

	string buffer;

	for (string line; getline(file, line); buffer.append(line)) {
		
		if (line.find("#") != string::npos)
			line = line.substr(0, line.find_first_of("#"));

		parser::trim(line, " \n\t\r\v\f");
	}

	file.close();

	parser::replace(buffer, '\t', ' ');

	parser::erase(buffer, "  ", 1);
	parser::erase(buffer, " {", 1);
	parser::erase(buffer, " }", 1);
	parser::erase(buffer, " ;", 1);

	parser::http(*this, buffer);

	logger::info("configuration file parsed: " + filename);
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

	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Http::getMaxBodySize(void) const {
		
	return _max_body_size;
}

void Http::setAcessLog(string access_log) {

	directive::setAcessLog(access_log, _access_log);
}

string Http::getAcessLog(void) const {

	return _access_log;
}

void Http::setErrorLog(string error_log) {

	directive::setErrorLog(error_log, _error_log);
}

string Http::getErrorLog(void) const {

	return _error_log;
}

void Http::setRoot(string root) {

	directive::setRoot(root, _root);
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

	os << "http {" << endl;
	os << "\tclient_max_body_size " << src.getMaxBodySize() << ";" << endl;
	os << "\taccess_log " << src.getAcessLog() << ";" << endl;
	os << "\terror_log " << src.getErrorLog() << ";" << endl;
	os << "\troot " << src.getRoot() << ";" << endl;
	
	set<Server> servers = src.getServers();
	for (set<Server>::iterator it = servers.begin(); it != servers.end(); it++)
		os << *it << endl;

	os << "}";

	return os;
}
