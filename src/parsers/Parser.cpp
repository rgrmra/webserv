/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:03:48 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/15 20:21:14 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <cstring>
#include <stdexcept>

using namespace std;

Parser::Parser(void) {

}

Parser::Parser(const Parser &src) {

	*this = src;
}

Parser &Parser::operator=(const Parser &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

Parser::~Parser(void) {

}

void Parser::http(Http &http, string &configuration_file) {

	if (strncmp("http{", configuration_file.c_str(), 5) == 0) {
		configuration_file.erase(0, 5);
	} else if (strncmp("server{", configuration_file.c_str(), 7) == 0) {
		configuration_file.erase(0, 7);
	} else {
		throw runtime_error("invalid!");
	}
	(void) http;
}

void Parser::server(Server &server, string &configuration_file) {

	(void)server;
	(void)configuration_file;
}

void Parser::location(Location &location, string &configuration_file) {

	(void)location;
	(void)configuration_file;
}

