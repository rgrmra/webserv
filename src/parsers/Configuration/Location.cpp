/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:35:12 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/04 19:05:08 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void) {

}

Location::Location(const Location &src) {

	*this = src;
}

Location &Location::operator=(const Location &rhs) {

	if (this == &rhs)
		return *this;

	_path = rhs._path;

	set<string>::iterator it = rhs._methods.begin();
	for (; it != rhs._methods.end(); it++)
		_methods.insert(*it);

	_redirect = rhs._redirect;

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

void Location::addMethod(string methods) {
	
	_methods.insert(methods);
}

set<string> Location::getMethods(void) const {

	return _methods;
}

void Location::setRedirect(string redirect) {
	
	_redirect = redirect;
}

string Location::getRedirect(void) const {

	return _redirect;
}

ostream &operator<<(ostream &os, const Location &src) {

	os << "Path: " << src.getPath() << endl;
	os << "Methods: ";

	set<string> methods = src.getMethods();
	for (set<string>::iterator it = methods.begin(); it != methods.end(); it++)
		os << *it << " ";

	return os << "\nRedirect: " << src.getRedirect() << std::endl;
}
