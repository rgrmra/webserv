/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:03:48 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/16 15:10:22 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <cstring>
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

string Parser::find(string key, string &configuration_file, string delimiter) {
	
	if (strncmp(key.c_str(), configuration_file.c_str(), key.size()) == 0) {
		configuration_file.erase(0, key.size());

		size_t pos = configuration_file.find(delimiter);

		if (pos != string::npos) {
			string tmp = configuration_file.substr(0, pos);
			configuration_file.erase(0, pos + 1);
			
			return tmp;
		}
	}

	return "";
}

void Parser::http(Http &http, string &configuration_file) {

	if (strncmp("http{", configuration_file.c_str(), 5) != 0)
		return ;

	// remove http
	configuration_file.erase(0, 5);
	
	static string keys[5] = {
		"client_max_body_size ",
		"access_log ",
		"error_log ",
		"root ",
		"server{"
	};

	string tmp;

	for (size_t i = 0; i < configuration_file.size(); i++) {

		tmp = Parser::find(keys[0], configuration_file, ";");
		if (not tmp.empty())
			http.setMaxBodySize(tmp);

		tmp = Parser::find(keys[1], configuration_file, ";");
		if (not tmp.empty())
			http.setAcessLog(tmp);

		tmp = Parser::find(keys[2], configuration_file, ";");
		if (not tmp.empty())
			http.setErrorLog(tmp);

		tmp = Parser::find(keys[3], configuration_file, ";");
		if (not tmp.empty())
			http.setRoot(tmp);

		if (strncmp(keys[4].c_str(), configuration_file.c_str(), keys[4].size()) == 0)
			http.addServer(Server(configuration_file));

		if (configuration_file.at(0) == '}') {
			configuration_file.erase(0, 1);
			break;
		}
	}

	if (not configuration_file.empty())
		cout << "invalid configuration file" << endl;
}

void Parser::server(Server &server, string &configuration_file) {

	configuration_file.erase(0, 7);
	
	static string keys[5] = {
		"listen ",
		"server_name ",
		"root ",
		"error_page",
		"location "
	};

	string tmp;

	for (size_t i = 0; i < configuration_file.size(); i++) {

		tmp = Parser::find(keys[0], configuration_file, ";");
		if (not tmp.empty()) {
			server.setHost(tmp);
			server.setPort(tmp);
		}

		tmp = Parser::find(keys[1], configuration_file, ";");
		if (not tmp.empty())
			server.setName(tmp);
			
		tmp = Parser::find(keys[2], configuration_file, ";");
		if (not tmp.empty())
			server.setRoot(tmp);

		tmp = Parser::find(keys[3], configuration_file, ";");
		if (not tmp.empty())
			server.addErrorPage(tmp, tmp);

		if (strncmp(keys[4].c_str(), configuration_file.c_str(), keys[4].size()) == 0)
			server.addLocation(Location(configuration_file));

		if (configuration_file.at(0) == '}') {
			configuration_file.erase(0, 1);
			break;
		}
	}
}

void Parser::location(Location &location, string &configuration_file) {

	//configuration_file.erase(0, 8);

	static string keys[4] = {
		"index ",
		"root ",
		"allow_methods",
		"autoindex "
	};

	string tmp;

	tmp = Parser::find("location ", configuration_file, "{");

	cout << "\033[091m" << tmp << "\033[0m" << endl;
	if (not tmp.empty())
		location.setPath(tmp);

	for (size_t i = 0; i < configuration_file.size(); i++) {

		tmp = Parser::find(keys[0], configuration_file, ";");
		if (not tmp.empty())
			location.setIndex(tmp);

		tmp = Parser::find(keys[1], configuration_file, ";");
		if (not tmp.empty())
			location.setRoot(tmp);

		tmp = Parser::find(keys[2], configuration_file, ";");
		if (not tmp.empty())
			location.addMethod(tmp);

		tmp = Parser::find(keys[3], configuration_file, ";");
		if (not tmp.empty())
			location.setAutoIndex(tmp);
			
		if (configuration_file.at(0) == '}') {
			configuration_file.erase(0, 1);
			break;
		}
	}
}

