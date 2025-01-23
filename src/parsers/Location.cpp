/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:27 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/22 21:32:21 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "directive.hpp"
#include "parser.hpp"
#include <cmath>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>

Location::Location(void) {

}

Location::Location(string &configuration_file) 
	: _max_body_size(0) {

	parser::location(*this, configuration_file);
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
	_allow_methods = rhs._allow_methods;
	_autoindex = rhs._autoindex;

	return *this;
}

Location::~Location(void) {

}

void Location::setPath(string path) {
	
	if (path.empty())
		return;

	if (path.find_first_of(" ") != string::npos)
		throw runtime_error("invalid location path: " + path);

	_path = path;
}

string Location::getPath(void) const {

	return _path;
}

void Location::setIndex(string index) {

	if (index.empty())
		return;

	list<string> indexes = parser::split(index);

	for (list<string>::iterator it = indexes.begin(); it != indexes.end(); it++)
		_index.insert(*it);
}

set<string> Location::getIndex(void) const {

	return _index;
}

void Location::setRoot(string root) {

	if (root.empty())
		return;

	_root = root;
}

string Location::getRoot(void) const {

	return _root;
}

void Location::addMethod(string method) {

	if (method.empty())
		return;

	list<string> methods = parser::split(method);

	for (list<string>::iterator it = methods.begin(); it != methods.end(); it++) {
		if (*it != "GET" && *it != "POST" && *it != "DELETE")
			throw runtime_error("invalid method: " + *it);

		_allow_methods.insert(*it);
	}
}

set<string> Location::getMethod(void) const {

	return _allow_methods;
}

void Location::setAutoIndex(string autoindex) {

	if (autoindex.empty())
		return;

	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw runtime_error("invalid autoindex: " + autoindex);
}

bool Location::getAutoIndex(void) const {

	return _autoindex;
}

void Location::setMaxBodySize(string max_body_size) {

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

size_t Location::getMaxBodySize(void) const {

	return _max_body_size;
}

void Location::setReturn(string value) {

	list<string> tmp = directive::setReturn(value);

	_return_code = tmp.front();

	if (tmp.size() != 2)
		return;

	_return_path = tmp.back();
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
	os << "\tindex:";

	set<string> indexes = src.getIndex();

	set<string>::iterator it = indexes.begin();

	for (;it != indexes.end(); it++)
		os << " " << *it;
	os << endl;

	os << "\troot: " << src.getRoot() << endl;

	set<string> methods = src.getMethod();

	set<string>::iterator itl = methods.begin();

	os << "\tallow_methods:";
	for (; itl != methods.end(); itl++)
		os << " " << *itl;
	os << endl;
	os << "\tautoindex: " << boolalpha <<src.getAutoIndex() << endl;
	os << "\treturn: " + src.getReturnCode() + " " + src.getReturnPath() << endl;
	return os;
}
