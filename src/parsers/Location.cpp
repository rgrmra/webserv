/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:27 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/16 14:07:04 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Parser.hpp"

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

	_index = rhs._index;
	_root = rhs._root;

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

ostream &operator<<(ostream &os, const Location &src) {

	os << "LOCATION:" << endl;
	os << "path: " << src.getPath() << endl;
	os << "index: " << src.getIndex() << endl;
	os << "root: " << src.getRoot() << endl;

	set<string> methods = src.getMethod();

	set<string>::iterator it = methods.begin();

	os << "allow_methods: ";
	for (; it != methods.end(); it++)
		os << *it;
	os << endl;
	os << "autoindex: " << src.getAutoIndex();
	return os;
}
