/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:27 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/21 18:31:09 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Logger.hpp"
#include "Parser.hpp"
#include <algorithm>

Location::Location(void) {

}

Location::Location(string &configuration_file) {

	Parser::location(*this, configuration_file);
}

Location::Location(const Location &src) {
	
	*this = src;
}

Location &Location::operator=(const Location &rhs) {

	if (this == &rhs)
		return *this;

	_path = rhs._path;
	_index = rhs._index;
	_root = rhs._root;
	_return_code = rhs._return_code;
	_return_path = rhs._return_path;

	set<string> allow_methods= rhs._allow_methods;

	set<string>::iterator it = allow_methods.begin();

	for (; it != allow_methods.end(); it++)
		_allow_methods.insert(*it);

	_autoindex = rhs._autoindex;

	return *this;
}

Location::~Location(void) {

}

void Location::setPath(string path) {

	_path = path;
}

string Location::getPath(void) const {

	return _path;
}

void Location::setIndex(string index) {

	_index = index;
}

string Location::getIndex(void) const {

	return _index;
}

void Location::setRoot(string root) {

	_root = root;
}

string Location::getRoot(void) const {

	return _root;
}

void Location::addMethod(string method) {

	_allow_methods.insert(method);
}

set<string> Location::getMethod(void) const {

	return _allow_methods;
}

void Location::setAutoIndex(string autoindex) {

	(void) autoindex;
	_autoindex = true;
}

bool Location::getAutoIndex(void) const {

	return _autoindex;
}
void Location::setMaxBodySize(string max_body_size) {

	(void) max_body_size;
	_max_body_size = 1000;
}

size_t Location::getMaxBodySize(void) const {

	return _max_body_size;
}

void Location::setReturn(string value) {

	_return_code = value;
	_return_path = value;
}

string Location::getReturnCode() const {

	return _return_code;
}

string Location::getReturnPath() const {

	return _return_path;
}

ostream &operator<<(ostream &os, const Location &src) {

	os << "LOCATION:" << endl;
	os << "\tpath: " << src.getPath() << endl;
	os << "\tindex: " << src.getIndex() << endl;
	os << "\troot: " << src.getRoot() << endl;

	set<string> methods = src.getMethod();

	set<string>::iterator it = methods.begin();

	os << "\tallow_methods: ";
	for (; it != methods.end(); it++)
		os << *it;
	os << endl;
	os << "\tautoindex: " << src.getAutoIndex() << endl;
	os << "\treturn: " + src.getReturnCode() + " " + src.getReturnPath() << endl;
	return os;
}
