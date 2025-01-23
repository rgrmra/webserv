/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:35:27 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 18:24:28 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "directive.hpp"
#include "parser.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
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
	
	directive::setPath(path, _path);
}

string Location::getPath(void) const {

	return _path;
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

void Location::addMethod(string method) {

	directive::addMethod(method, _allow_methods);
}

set<string> Location::getMethod(void) const {

	return _allow_methods;
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

	directive::setReturn(value, _return_code, _return_path);
}

string Location::getReturnCode() const {

	return _return_code;
}

string Location::getReturnPath() const {

	return _return_path;
}

ostream &operator<<(ostream &os, const Location &src) {

	os << "\t\tlocation " << src.getPath() << " {" << endl;
	
	os << "\t\t\tindex";
	set<string> indexs = src.getIndex();
	for (set<string>::iterator it = indexs.begin(); it != indexs.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	os << "\t\t\troot " << src.getRoot() << ";" << endl;

	os << "\t\t\tallow_methods";
	set<string> methods = src.getMethod();
	for (set<string>::iterator it = methods.begin(); it != methods.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	os << "\t\t\tclient_max_body_size " << src.getMaxBodySize() << ";" << endl;
	os << "\t\t\tautoindex " << (src.getAutoIndex() ? "on" : "off") << endl;
	os << "\t\t\treturn " << src.getReturnCode() << " " << src.getReturnPath() << ";" << endl;
	os << "\t\t}";

	return os;
}
