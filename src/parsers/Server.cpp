/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:24:18 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 15:53:58 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "directive.hpp"
#include "Http.hpp"
#include "Location.hpp"
#include "parser.hpp"
#include <iostream>
#include <list>

using namespace std;


Server::Server(void) {

}

Server::Server(string &configuration_file) 
	: _max_body_size(0) {

	parser::server(*this, configuration_file);
}

Server::Server(const Server &src) {

	*this = src;
}

Server &Server::operator=(const Server &rhs) {

	if (this == &rhs)
		return *this;

	_name = rhs._name;
	_host = rhs._host;
	_port = rhs._port;
	_root = rhs._root;
	_index = rhs._index;
	_max_body_size = rhs._max_body_size;
	_return_code = rhs._return_code;
	_return_path = rhs._return_path;
	_error_pages = rhs._error_pages;
	_locations = rhs._locations;

	return *this;
}

Server::~Server(void) {

}

bool Server::operator<(const Server &rhs) const {
	
	return (_port < rhs._port) && (_host < rhs._host);
}

void Server::setName(string name) {

	directive::setName(name, _name);
}

list<string> Server::getName(void) const {

	return _name;
}

void Server::setListen(string listen) {

	directive::setListen(listen, _host, _port);
}

string Server::getHost(void) const {

	return _host;
}

string Server::getPort(void) const {

	return _port;
}

void Server::setRoot(string root) {

	directive::setRoot(root, _root);
}

string Server::getRoot(void) const {

	return _root;
}

void Server::setIndex(string index) {

	directive::setIndex(index, _index);
}

set<string> Server::getIndex(void) const {

	return _index;
}

void Server::setMaxBodySize(string max_body_size) {

	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Server::getMaxBodySize(void) const {

	return _max_body_size;
}

void Server::setErrorPage(string error_page) {

	directive::addErrorPage(error_page, _error_pages);
}

string Server::getErrorPage(string code) const {

	if (_error_pages.find(code)->first.empty())
		return "";

	return _error_pages.find(code)->second;
}

map<string, string> Server::getErrorPages(void) const {

	return _error_pages;
}

void Server::addLocation(Location location) {

	_locations[location.getPath()] = location;
}

Location Server::getLocation(string code) const {
	
	(void) code;
	return Location();
}

map<string, Location> Server::getLocations(void) const {

	return _locations;
}

void Server::setReturn(string value) {

	directive::setReturn(value, _return_code, _return_path);
}

string Server::getReturnCode(void) const {

	return _return_code;
}

string Server::getReturnPath(void) const {

	return _return_path;
}

ostream &operator<<(ostream &os, const Server &src) {

	os << "\tserver {" << endl;
	os << "\t\tlisten " << src.getHost() << ":" << src.getPort() << ";" << endl;

	os << "\t\tserver_name";
	list<string> names = src.getName();
	for (list<string>::iterator it = names.begin(); it != names.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	os << "\t\troot " << src.getRoot() << ";" << endl;

	os << "\t\tindex";
	set<string> indexs = src.getIndex();
	for (set<string>::iterator it = indexs.begin(); it != indexs.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	os << "\t\tclient_max_body_size " << src.getMaxBodySize() << ";" << endl;

	map<string, string> error_pages = src.getErrorPages();
	for (map<string, string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		os << "\t\terror_page " << it->first << " " << (*it).second << ";" << endl;

	map<string, Location> locations = src.getLocations();
	for (map<string, Location>::iterator it = locations.begin(); it != locations.end(); it++)
		os << it->second << endl;

	os << "\t\treturn " << src.getReturnCode() + " " + src.getReturnPath() << ";" << endl;
	os << "\t}";

	return os;
}
