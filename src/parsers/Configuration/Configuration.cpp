/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:00:39 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/04 20:49:03 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"
#include "Location.hpp"
#include <cstdio>

Configuration::Configuration(void) {

}

Configuration::Configuration(const Configuration &src) {

	*this = src;
}

Configuration &Configuration::operator=(const Configuration &rhs) {

	if (this == &rhs)
		return *this;

	_name = rhs._name;
	_port = rhs._port;
	_host = rhs._host;
	_max_body_size = rhs._max_body_size;
	_root = rhs._root;
	_default_file = rhs._default_file;

	map<string, string> configuration = rhs._error_page;

	map<string, string>::iterator it = configuration.begin();

	for (; it != configuration.end(); it++)
		_error_page[it->first] = it->second;

	map<string, Location> location = rhs._location;

	map<string, Location>::iterator itl = location.begin();

	for (; itl != location.end(); itl++)
		_location[itl->first] = itl->second;

	return *this;
}

Configuration::~Configuration(void) {

}

bool Configuration::operator<(const Configuration &rhs) const {

	return _port < rhs._port;
}

void Configuration::setName(string name) {

	_name = name;
}

string Configuration::getName(void) const {

	return _name;
}

void Configuration::setPort(string port) {

	_port = port;
}

string Configuration::getPort(void) const {

	return _port;
}

void Configuration::setHost(string host) {

	_host = host;
}

string Configuration::getHost(void) const {

	return _host;
}

void Configuration::setMaxBodySize(string max_body_size) {

	_max_body_size = max_body_size;
}

string Configuration::getMaxBodySize(void) const {
	
	return _max_body_size;
}

void Configuration::setRoot(string root) {

	_root = root;
}

string Configuration::getRoot(void) const {

	return _root;
}

void Configuration::setDefaultFile(string default_file) {
	
	_default_file = default_file;
}

string Configuration::getDefaultFile(void) const {

	return _default_file;
}

void Configuration::addErrorPage(string code, string path) {

	_error_page[code] = path;
}

string Configuration::getErrorPage(string code) const {

	if (not _error_page.find(code)->first.empty())
		return _error_page.find(code)->second;
	return NULL;
}

map<string, string> Configuration::getErrorPage(void) const {

	return _error_page;
}

void Configuration::addLocation(Location location) {

	_location[location.getPath()] = location;
}

Location Configuration::getLocation(string path) const {

	if (not _location.find(path)->first.empty())
		return _location.find(path)->second;

	return Location();
}

map<string, Location> Configuration::getLocation(void) const {

	return _location;
}

std::ostream &operator<<(std::ostream &os, const Configuration &src) {

	os << "Name: " << src.getName() << endl;
	os << "Port: " << src.getPort() << endl;
	os << "Host: " << src.getHost() << endl;
	os << "Max Body Size: " << src.getMaxBodySize() << endl;
	os << "Root: " << src.getRoot() << endl;
	os << "Default File: " << src.getDefaultFile() << endl;
	os << "Error Pages: " << endl;
	
	map<string, string> error_page = src.getErrorPage();

	map<string, string>::iterator it = error_page.begin();

	for (; it != error_page.end(); it++)
		os << (*it).first << " " << (*it).second << endl;

	os << "Locations: " << endl;

	map<string, Location> location = src.getLocation();

	map<string, Location>::iterator itl = location.begin();

	for (; itl != location.end(); itl++)
		os << (*itl).second << endl;

	return os;
}
