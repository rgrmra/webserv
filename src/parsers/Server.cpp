/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:24:18 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/29 15:31:26 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "directive.hpp"
#include "Http.hpp"
#include "Location.hpp"
#include "parser.hpp"
#include <iostream>
#include <list>
#include <stdexcept>

using namespace std;


Server::Server(void)
	: _max_body_size(0) {

}

Server::Server(string &configuration_file) 
	: _max_body_size(0) {

	parser::server(*this, configuration_file);

	if (!_listen.size())
		throw runtime_error("listen not setted");
}

Server::Server(const Server &src) {

	*this = src;
}

Server &Server::operator=(const Server &rhs) {

	if (this == &rhs)
		return *this;

	_name = rhs._name;
	_listen = rhs._listen;
	_root = rhs._root;
	_index = rhs._index;
	_max_body_size = rhs._max_body_size;
	_return_code = rhs._return_code;
	_return_uri = rhs._return_uri;
	_error_pages = rhs._error_pages;
	_locations = rhs._locations;

	return *this;
}

Server::~Server(void) {

}

bool Server::operator<(const Server &rhs) const {

//	if (_port != rhs._port)
//		return _port < rhs._port;
//
//	if (_host == "0.0.0.0" || rhs._host == "0.0.0.0")
//		return _port < rhs._port;
//
//	string server1 = _host + ":" +_port;
//	string server2 = rhs._host + ":" + rhs._port;
//
//	if (server1 == server2)
//		return server1 < server2;
//
//	list<string>::const_iterator i1 = _name.begin();
//	while (i1 != _name.end()) {
//
//		list<string>::const_iterator i2 = rhs._name.begin();
//		while (i2 != rhs._name.end()) {
//			if (*i1 == *i2)
//				return *i1 < *i2;
//
//			i2++;
//		}
//
//		i1++;
//	}
//
//	return server1 < server2;
	return _root < rhs._root;
}

void Server::setName(string name) {

	directive::setName(name, _name);
}

list<string> Server::getName(void) const {

	return _name;
}

void Server::setListen(string listen) {

	directive::setListen(listen, _listen);
}

list<string> Server::getListen(void) const {

	return _listen;
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

	directive::setErrorPage(error_page, _error_pages);
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

	map<string, Location>::iterator it = _locations.begin();
	for (; it != _locations.end(); it++)
		if (it->first == location.getURI())
			throw runtime_error("duplicated location: " + location.getURI());

	_locations[location.getURI()] = location;
}

Location Server::getLocation(string uri) const {
	
	map<string, Location>::const_iterator it = _locations.begin();
	for (; it != _locations.end(); it++)
		if (it->first == uri)
			return it->second;

	return Location();
}

map<string, Location> Server::getLocations(void) const {

	return _locations;
}

void Server::setReturn(string value) {

	directive::setReturn(value, _return_code, _return_uri);
}

string Server::getReturnCode(void) const {

	return _return_code;
}

string Server::getReturnURI(void) const {

	return _return_uri;
}

ostream &operator<<(ostream &os, const Server &src) {

	os << "\tserver {" << endl;

	list<string> listens = src.getListen();
	for (list<string>::iterator it = listens.begin(); it != listens.end(); it++)
		os << "\t\tlisten " << *it << ";" << endl;

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

	os << "\t\treturn " << src.getReturnCode() + " " + src.getReturnURI() << ";" << endl;
	os << "\t}";

	return os;
}
