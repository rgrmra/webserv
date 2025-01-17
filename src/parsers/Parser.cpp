/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:03:48 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/17 10:06:53 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <functional>
#include <iostream>
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

bool Parser::compare(string key, string &configuration_file) {

	if (configuration_file.empty())
		return false;

	if (configuration_file.compare(0, key.size(), key) == 0)
		return true;

	return false;
}

string Parser::find(string key, string &configuration_file, string delimiter) {

	if (configuration_file.empty())
		return "";

	if (not Parser::compare(key, configuration_file))
		return "";
	
	configuration_file.erase(0, key.size());

	size_t pos = configuration_file.find(delimiter);

	if (pos == string::npos)
		return "";

	string tmp = configuration_file.substr(0, pos);
	configuration_file.erase(0, pos + 1);
		
	return tmp;
}

void Parser::http(Http &http, string &configuration_file) {

	if (Parser::compare("http{", configuration_file))
		configuration_file.erase(0, 5);
	
	string tmp;

	for (size_t i = 0; i < configuration_file.size(); i++) {

		tmp = Parser::find("client_max_body_size ", configuration_file, ";");
		if (not tmp.empty())
			http.setMaxBodySize(tmp);

		tmp = Parser::find("access_log ", configuration_file, ";");
		if (not tmp.empty())
			http.setAcessLog(tmp);

		tmp = Parser::find("error_log ", configuration_file, ";");
		if (not tmp.empty())
			http.setErrorLog(tmp);

		tmp = Parser::find("root ", configuration_file, ";");
		if (not tmp.empty())
			http.setRoot(tmp);

		if (Parser::compare("server{", configuration_file))
			http.addServer(Server(configuration_file));

		if (Parser::compare("}", configuration_file)) {
			configuration_file.erase(0, 1);
			break;
		}
	}

	if (not configuration_file.empty())
		cout << "invalid configuration file" << endl;
}

void Parser::server(Server &server, string &configuration_file) {

	configuration_file.erase(0, 7);
	
	string tmp;

	for (size_t i = 0; i < configuration_file.size(); i++) {

		tmp = Parser::find("listen ", configuration_file, ";");
		if (not tmp.empty()) {
			server.setHost(tmp);
			server.setPort(tmp);
		}

		tmp = Parser::find("server_name ", configuration_file, ";");
		if (not tmp.empty())
			server.setName(tmp);
			
		tmp = Parser::find("root ", configuration_file, ";");
		if (not tmp.empty())
			server.setRoot(tmp);

		tmp = Parser::find("error_page ", configuration_file, ";");
		if (not tmp.empty())
			server.addErrorPage(tmp, tmp);

		tmp = Parser::find("index ", configuration_file, ";");
		if (not tmp.empty())
			server.setIndex(tmp);

		tmp = Parser::find("client_max_body_size ", configuration_file, ";");
		if (not tmp.empty())
			server.setMaxBodySize(tmp);

		tmp = Parser::find("return ", configuration_file, ";");
		if (not tmp.empty())
			server.setReturn(tmp);

		if (Parser::compare("location ", configuration_file))
			server.addLocation(Location(configuration_file));

		if (Parser::compare("}", configuration_file)) {
			configuration_file.erase(0, 1);
			break;
		}
	}
}

void Parser::location(Location &location, string &configuration_file) {

	string tmp;

	tmp = Parser::find("location ", configuration_file, "{");
	if (not tmp.empty())
		location.setPath(tmp);

	for (size_t i = 0; i < configuration_file.size(); i++) {

		tmp = Parser::find("index ", configuration_file, ";");
		if (not tmp.empty())
			location.setIndex(tmp);

		tmp = Parser::find("root ", configuration_file, ";");
		if (not tmp.empty())
			location.setRoot(tmp);

		tmp = Parser::find("allow_methods ", configuration_file, ";");
		if (not tmp.empty())
			location.addMethod(tmp);

		tmp = Parser::find("autoindex ", configuration_file, ";");
		if (not tmp.empty())
			location.setAutoIndex(tmp);

		tmp = Parser::find("return ", configuration_file, ";");
		if (not tmp.empty())
			location.setReturn(tmp);
			
		if (Parser::compare("}", configuration_file)) {
			configuration_file.erase(0, 1);
			break;
		}
	}
}

