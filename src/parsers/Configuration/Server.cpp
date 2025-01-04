/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:00:35 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/04 20:49:52 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Server.hpp"
#include <ostream>
#include <set>

Server::Server(void) {

}

Server::Server(string configuration_file) {

	// TODO: PARSER
	(void) configuration_file;

	Location l1 = Location();
	
	l1.setPath("/");
	l1.addMethod("GET");
	l1.addMethod("POST");
	l1.setRedirect("/index.html");
	
	Location l2 = Location();
	
	l2.setPath("/home");
	l2.addMethod("GET");
	l2.addMethod("POST");
	l2.setRedirect("/index.html");
	
	Configuration c = Configuration();
	
	c.setName("First");
	c.setPort("8888");
	c.setHost("127.0.0.1");
	c.setMaxBodySize("23854644");
	c.setRoot("websites/01");
	c.setDefaultFile("index.html");
	c.addErrorPage("403", "/error_webpages/403.html");
	c.addErrorPage("404", "/error_webpages/404.html");
	c.addErrorPage("405", "/error_webpages/405.html");
	c.addErrorPage("413", "/error_webpages/413.html");
	c.addErrorPage("501", "/error_webpages/501.html");
	c.addLocation(l1);
	c.addLocation(l2);

	_servers.insert(c);
	_servers.insert(c);
	_servers.insert(c);

}

Server::Server(const Server &src) {

	*this = src;
}

Server &Server::operator=(const Server &src) {

	if (this == &src)
		return *this;

	return *this;
}

Server::~Server(void) {

}

Configuration Server::getServer(string host, string port) {

	set<Configuration>::iterator it = _servers.begin();

	for (; it != _servers.end(); it++)
		if ((*it).getHost() == host && (*it).getPort() == port)
			return *it;

	return Configuration();
}

set<Configuration> Server::getServers(void) const {

	return _servers;
}

ostream &operator<<(ostream &os, const Server &src) {

	set<Configuration> servers = src.getServers();

	set<Configuration>::iterator it = servers.begin();

	for (; it != servers.end(); it++)
		os << *it << endl;

	return os;
}
