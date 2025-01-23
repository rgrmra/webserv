/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:03:48 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 14:25:00 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include "Location.hpp"

using namespace std;

void parser::replace(string &buffer, char from, char to) {

	for (size_t i = 0; i < buffer.length(); i++)
		if (buffer.at(i) == from)
			buffer.at(i) = to;
}

void parser::erase(string &buffer, string text, size_t quantity) {

	size_t pos = buffer.find(text);

	while (pos != string::npos) {
		buffer.erase(pos, quantity);

		pos = buffer.find(text);
	}
}

void parser::trim(string &buffer, string set) {

	if (buffer.find_first_not_of(set) == string::npos) {
		buffer = "";
		return;
	}

	buffer = buffer.substr(buffer.find_first_not_of(set), buffer.size());
	buffer = buffer.substr(0, buffer.find_last_not_of(set) + 1);
}

bool parser::compare(string key, string &buffer) {

	if (buffer.empty())
		return false;

	if (buffer.compare(0, key.size(), key) == 0)
		return true;

	return false;
}

string parser::find(string key, string &buffer, string delimiter) {

	if (buffer.empty())
		return "";

	if (not parser::compare(key, buffer))
		return "";
	
	buffer.erase(0, key.size());

	size_t pos = buffer.find(delimiter);

	if (pos == string::npos)
		return "";

	string tmp = buffer.substr(0, pos);
	buffer.erase(0, pos + 1);
		
	return tmp;
}

list<string> parser::split(std::string text, char delimiter) {

	list<string> tmp;

	while (text.size()) {
		
		parser::trim(text, string(1, delimiter));
		size_t pos = text.find_first_of(string(1, delimiter));

		if (not text.empty() && pos == string::npos) {
			tmp.push_back(text.substr(0, text.size()));
			text.erase(0, text.size());
		}
		else if (not text.empty() && pos != string::npos) {
			tmp.push_back(text.substr(0, pos));
			text.erase(0, pos);
		}
	}

	return tmp;
}

string parser::basename(string text) {

	if (text.empty())
		return "";

	size_t pos = text.find_last_of(".");

	if (pos == string::npos)
		return "";

	return text.substr(pos, text.size() - pos);
}

bool parser::validadeHttpCode(string code) {

	if (code.find_first_not_of("0123456789") != string::npos)
		return false;

	if (code < "100" || code > "599")
		return false;

	return true;
}

void parser::http(Http &http, string &buffer) {

	if (parser::compare("http{", buffer)) {
		buffer.erase(0, 5);

		if (buffer.size() > 1 && string(&buffer[buffer.size() - 2]) != "}}")
			throw runtime_error("failed to parse not enclosed http");

		buffer.erase(buffer.size() - 1, buffer.size());
	}
	
	for (size_t i = 0; i < buffer.size(); i++) {

		http.setMaxBodySize(find("client_max_body_size ", buffer, ";"));
		http.setAcessLog(find("access_log ", buffer, ";"));
		http.setErrorLog(find("error_log ", buffer, ";"));
		http.setRoot(find("root ", buffer, ";"));

		if (parser::compare("server{", buffer))
			http.addServer(Server(buffer));

		if (buffer.empty()) {
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parse http at: " + buffer);
}

void parser::server(Server &server, string &buffer) {

	buffer.erase(0, 7);
	
	string tmp;

	for (size_t i = 0; i < buffer.size(); i++) {

		tmp = parser::find("listen ", buffer, ";");
		if (not tmp.empty())
			server.setListen(tmp);

		tmp = parser::find("server_name ", buffer, ";");
		if (not tmp.empty())
			server.setName(tmp);
			
		tmp = parser::find("root ", buffer, ";");
		if (not tmp.empty())
			server.setRoot(tmp);

		tmp = parser::find("error_page ", buffer, ";");
		if (not tmp.empty())
			server.setErrorPage(tmp);

		tmp = parser::find("index ", buffer, ";");
		if (not tmp.empty())
			server.setIndex(tmp);

		tmp = parser::find("client_max_body_size ", buffer, ";");
		if (not tmp.empty())
			server.setMaxBodySize(tmp);

		tmp = parser::find("return ", buffer, ";");
		if (not tmp.empty())
			server.setReturn(tmp);

		if (parser::compare("location ", buffer))
			server.addLocation(Location(buffer));

		if (parser::compare("}", buffer)) {
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parser server at: " + buffer);
}

void parser::location(Location &location, string &buffer) {

	string tmp;

	tmp = parser::find("location ", buffer, "{");
	if (not tmp.empty())
		location.setPath(tmp);

	for (size_t i = 0; i < buffer.size(); i++) {

		location.setIndex(find("index ", buffer, ";"));
		location.setRoot(find("root ", buffer, ";"));
		location.addMethod(find("allow_methods ", buffer, ";"));
		location.setAutoIndex(find("autoindex ", buffer, ";"));
		location.setMaxBodySize(find("client_max_body_size ", buffer, ";"));

		tmp = parser::find("return ", buffer, ";");
		if (not tmp.empty())
			location.setReturn(tmp);
			
		if (parser::compare("}", buffer)) {
			buffer.erase(0, 1);
			return ;
		}
	}

	throw runtime_error("failed to parse location at: " + buffer);
}
