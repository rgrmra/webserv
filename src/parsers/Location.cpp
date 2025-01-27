/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:27 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/26 20:21:50 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "directive.hpp"
#include "parser.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

Location::Location(void)
	: _deny_methods(false),
	  _max_body_size(0) {

}

Location::Location(string &configuration_file) 
	: _deny_methods(false),
	  _max_body_size(0) {

	parser::location(*this, configuration_file);
}

Location::Location(const Location &src) {
	
	*this = src;
}

Location &Location::operator=(const Location &rhs) {

	if (this == &rhs)
		return *this;

	_uri= rhs._uri;
	_index = rhs._index;
	_root = rhs._root;
	_return_code = rhs._return_code;
	_return_uri= rhs._return_uri;
	_allow_methods = rhs._allow_methods;
	_deny_methods = rhs._deny_methods;
	_autoindex = rhs._autoindex;
	_max_body_size = rhs._max_body_size;

	return *this;
}

Location::~Location(void) {

}

void Location::setURI(string uri) {
	
	directive::setURI(uri, _uri);
}

string Location::getURI(void) const {

	return _uri;
}

void Location::setIndex(string index) {

	directive::setIndex(index, _index);
}

set<string> Location::getIndex(void) const {

	return _index;
}

void Location::setRoot(string root) {

	directive::setRoot(root, _root);
}

string Location::getRoot(void) const {

	return _root;
}

void Location::setMethods(string method) {

	directive::setMethods(method, _allow_methods);
}

set<string> Location::getMethods(void) const {

	return _allow_methods;
}
void Location::setDenyMethods(string deny_methods) {

	directive::setDenyMethods(deny_methods, _deny_methods);
}
bool Location::getDenyMethods(void) const {

	return _deny_methods;
}

void Location::setAutoIndex(string autoindex) {

	directive::setAutoIndex(autoindex, _autoindex);
}

bool Location::getAutoIndex(void) const {

	return _autoindex;
}

void Location::setMaxBodySize(string max_body_size) {

	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Location::getMaxBodySize(void) const {

	return _max_body_size;
}

void Location::setReturn(string value) {

	directive::setReturn(value, _return_code, _return_uri);
}

string Location::getReturnCode() const {

	return _return_code;
}

string Location::getReturnURI() const {

	return _return_uri;
}

ostream &operator<<(ostream &os, const Location &src) {

	os << "\t\tlocation " << src.getURI() << " {" << endl;
	
	os << "\t\t\tindex";
	set<string> indexs = src.getIndex();
	for (set<string>::iterator it = indexs.begin(); it != indexs.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	os << "\t\t\troot " << src.getRoot() << ";" << endl;

	os << "\t\t\tlimit_except";
	set<string> methods = src.getMethods();
	for (set<string>::iterator it = methods.begin(); it != methods.end(); it++)
		os << " " << *it;
	os << " {";
	if (src.getDenyMethods())
		os << "\n\t\t\t\tdeny all;\n\t\t\t";
	os << "}" << endl;

	os << "\t\t\tclient_max_body_size " << src.getMaxBodySize() << ";" << endl;
	os << "\t\t\tautoindex " << (src.getAutoIndex() ? "on" : "off") << endl;
	os << "\t\t\treturn " << src.getReturnCode() << " " << src.getReturnURI() << ";" << endl;
	os << "\t\t}";

	return os;
}
