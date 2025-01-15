/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:24:18 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/15 18:18:36 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"
#include <cstring>
#include <iostream>

using namespace std;


Server::Server(void) {

}

Server::Server(string configuration_file) {

	(void) configuration_file;
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

	map<string, string> error_pages = rhs._error_pages;

	map<string, string>::iterator it = error_pages.begin();

	for (; it != error_pages.end(); it++)
		_error_pages[it->first] = it->second;

	map<string, Location> locations = rhs._locations;

	map<string, Location>::iterator itl = locations.begin();

	for (; itl != locations.end(); itl++)
		_locations[itl->first] = itl->second;
	
	return *this;
}

Server::~Server(void) {

}

bool Server::operator<(const Server &rhs) const {
	
	return _port < rhs._port;
}

void Server::setName(string name) {

	_name = name;
}

string Server::getName(void) const {

	return _name;
}

void Server::setHost(string host) {

	_host = host;
}

string Server::getHost(void) const {

	return _host;
}

void Server::setPort(string port) {
	
	_port = port;
}

string Server::getPort(void) const {

	return _port;
}

void Server::setRoot(string root) {

	_root = root;
}

string Server::getRoot(void) const {

	return _root;
}

void Server::setIndex(string index) {

	_index = index;
}

string Server::getIndex(void) const {

	return _index;
}

void Server::setMaxBodySize(string size) {

	(void) size;
	_max_body_size = 0;
}

size_t Server::getMaxBodySize(void) const {

	return _max_body_size;
}

void Server::addErrorPage(string code, string path) {

	_error_pages[code] = path;
}

string Server::getErrorPage(string code) const {

	if (_error_pages.find(code)->first.empty())
		return "";

	return _error_pages.find(code)->second;
}

void Server::addLocation(string path, Location location) {

	_locations[path] = location;
}

Location Server::getLocation(string code) const {
	
	(void) code;
	return Location();
}

ostream &operator<<(ostream &os, const Server &src) {

	(void) src;
	return os;
}

