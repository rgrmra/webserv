/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:24:18 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/29 19:07:35 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "directive.hpp"
#include "Http.hpp"
#include "Location.hpp"
#include "parser.hpp"
#include <iostream>
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

	_names = rhs._names;
	_listen = rhs._listen;
	_root = rhs._root;
	_indexes = rhs._indexes;
	_max_body_size = rhs._max_body_size;
	_return_code = rhs._return_code;
	_return_uri = rhs._return_uri;
	_error_pages = rhs._error_pages;
	_locations = rhs._locations;

	return *this;
}

Server::~Server(void) {

}

void Server::addName(string name) {

	directive::addName(name, _names);
}

void Server::setNames(vector<string> names) {

	_names = names;
}

vector<string> Server::getNames(void) const {

	return _names;
}

void Server::addListen(string listen) {

	directive::addListen(listen, _listen);
}

void Server::setListen(vector<string> listen) {

	_listen = listen;
}

vector<string> Server::getListen(void) const {

	return _listen;
}

void Server::setRoot(string root) {

	directive::setRoot(root, _root);
}

string Server::getRoot(void) const {

	return _root;
}

void Server::addIndex(string index) {

	directive::addIndex(index, _indexes);
}

void Server::setIndexes(set<string> indexes) {

	_indexes = indexes;
}

set<string> Server::getIndexes(void) const {

	return _indexes;
}

void Server::setMaxBodySize(string max_body_size) {

	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Server::getMaxBodySize(void) const {

	return _max_body_size;
}

void Server::addErrorPage(string error_page) {

	directive::addErrorPage(error_page, _error_pages);
}

void Server::setErrorPages(map<string, string> error_pages) {

	_error_pages = error_pages;
}

string Server::getErrorPageByCode(string code) const {

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

void Server::setLocations(map<string, Location> locations) {

	_locations = locations;
}

Location Server::getLocationByURI(string uri) const {
	
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

	vector<string> listens = src.getListen();
	for (vector<string>::iterator it = listens.begin(); it != listens.end(); it++)
		os << "\t\tlisten " << *it << ";" << endl;

	os << "\t\tserver_name";
	vector<string> names = src.getNames();
	for (vector<string>::iterator it = names.begin(); it != names.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	os << "\t\troot " << src.getRoot() << ";" << endl;

	os << "\t\tindex";
	set<string> indexs = src.getIndexes();
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
