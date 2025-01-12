/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:00:24 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/09 19:33:02 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Server.hpp"
#include <set>

Http::Http(void) {

}

Http::Http(string configuration_file) {

	// TODO:: parser file
}

Http::Http(const Http &src) {

	*this = src;
}

Http &Http::operator=(const Http &rhs) {
	
	if (this == &rhs)
		return *this;

	_max_body_size = rhs._max_body_size;
	_error_log = rhs._error_log;
	_access_log = rhs._access_log;
	_root = rhs._root;

	set<Server>::iterator it = rhs._servers.begin();

	for (; it != rhs._servers.end(); it++)
		_servers.insert(*it);

	return *this;

}

Http::~Http(void) {

}


void Http::setMaxBodySize(string max_body_size) {


}

size_t Http::getMaxBodySize(void) const {
		

}

void Http::setAcessLog(string access_log) {

	_access_log = access_log;
}

string Http::getAcessLog(void) const {

	return _access_log;
}

void Http::setErrorLog(string error_log) {

	_error_log = error_log;
}

string Http::getErrorLog(void) const {

	return _error_log;
}

void Http::setRoot(string root) {

	_root = root;
}

string Http::getRoot(void) const {

	return _root;
}


void Http::addServer(Server server) {

	_servers.insert(server);
}

Server Http::getServerByName(string name, string port) const {

	return Server();
}

Server Http::getServerByHost(string host, string port) const {

	return Server();
}

void Http::removeServerByName(string name, string port) {

}

void Http::removeServerByHost(string host, string port) {

}

ostream &operator<<(ostream &os, const Http &src) {

	return os;
}

