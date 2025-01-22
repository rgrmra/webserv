/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:03:48 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/22 15:05:38 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include "Location.hpp"

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

void Parser::replace(string &buffer, char from, char to) {

	for (size_t i = 0; i < buffer.length(); i++)
		if (buffer.at(i) == from)
			buffer.at(i) = to;
}

void Parser::erase(string &buffer, string text, size_t quantity) {

	size_t pos = buffer.find(text);

	while (pos != string::npos) {
		buffer.erase(pos, quantity);

		pos = buffer.find(text);
	}
}

void Parser::trim(string &buffer, string set) {

	if (buffer.find_first_not_of(set) == string::npos) {
		buffer = "";
		return;
	}

	buffer = buffer.substr(buffer.find_first_not_of(set), buffer.size());
	buffer = buffer.substr(0, buffer.find_last_not_of(set) + 1);
}

bool Parser::compare(string key, string &buffer) {

	if (buffer.empty())
		return false;

	if (buffer.compare(0, key.size(), key) == 0)
		return true;

	return false;
}

string Parser::find(string key, string &buffer, string delimiter) {

	if (buffer.empty())
		return "";

	if (not Parser::compare(key, buffer))
		return "";
	
	buffer.erase(0, key.size());

	size_t pos = buffer.find(delimiter);

	if (pos == string::npos)
		return "";

	string tmp = buffer.substr(0, pos);
	buffer.erase(0, pos + 1);
		
	return tmp;
}

list<string> Parser::split(std::string text) {

	list<string> tmp;

	for (size_t i = 0; i < text.size(); i++) {
		
		Parser::trim(text, " ");
		size_t pos = text.find_first_of(" ");

		if (not text.empty() && pos == string::npos) {
			tmp.push_back(text.substr(0, text.size()));
			text.erase(0, text.size());
		}
		else if (not text.empty() && pos != string::npos) {
			tmp.push_back(text.substr(0, pos));
			text.erase(0, pos);
		}
		cout << text << endl;
	}

	return tmp;
}

string Parser::basename(string text) {

	if (text.empty())
		return "";

	size_t pos = text.find_last_of(".");

	if (pos == string::npos)
		return "";

	return text.substr(pos, text.size() - pos);
}

size_t	Parser::stringToSizeT(string text) {
	
	size_t	tmp = 0;

	for (size_t i = 0; i < text.size(); i++)
		tmp += text.at(i);

	return tmp;
}

void Parser::http(Http &http, string &buffer) {

	if (Parser::compare("http{", buffer)) {
		buffer.erase(0, 5);
		buffer.erase(buffer.size() - 1, buffer.size());
	}
	
	for (size_t i = 0; i < buffer.size(); i++) {

		http.setMaxBodySize(find("client_max_body_size ", buffer, ";"));
		http.setAcessLog(find("access_log ", buffer, ";"));
		http.setErrorLog(find("error_log ", buffer, ";"));
		http.setRoot(find("root ", buffer, ";"));

		if (Parser::compare("server{", buffer))
			http.addServer(Server(buffer));

		if (Parser::compare("}", buffer) || buffer.empty()) {
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parse http at: " + buffer);
}

void Parser::server(Server &server, string &buffer) {

	buffer.erase(0, 7);
	
	string tmp;

	for (size_t i = 0; i < buffer.size(); i++) {

		tmp = Parser::find("listen ", buffer, ";");
		if (not tmp.empty()) {
			server.setHost(tmp);
			server.setPort(tmp);
		}

		tmp = Parser::find("server_name ", buffer, ";");
		if (not tmp.empty())
			server.setName(tmp);
			
		tmp = Parser::find("root ", buffer, ";");
		if (not tmp.empty())
			server.setRoot(tmp);

		tmp = Parser::find("error_page ", buffer, ";");
		if (not tmp.empty())
			server.addErrorPage(tmp, tmp);

		tmp = Parser::find("index ", buffer, ";");
		if (not tmp.empty())
			server.setIndex(tmp);

		tmp = Parser::find("client_max_body_size ", buffer, ";");
		if (not tmp.empty())
			server.setMaxBodySize(tmp);

		tmp = Parser::find("return ", buffer, ";");
		if (not tmp.empty())
			server.setReturn(tmp);

		if (Parser::compare("location ", buffer))
			server.addLocation(Location(buffer));

		if (Parser::compare("}", buffer)) {
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parser server at: " + buffer);
}

void Parser::location(Location &location, string &buffer) {

	string tmp;

	tmp = Parser::find("location ", buffer, "{");
	if (not tmp.empty())
		location.setPath(tmp);

	for (size_t i = 0; i < buffer.size(); i++) {

		tmp = Parser::find("index ", buffer, ";");
		if (not tmp.empty())
			location.setIndex(tmp);

		tmp = Parser::find("root ", buffer, ";");
		if (not tmp.empty())
			location.setRoot(tmp);

		tmp = Parser::find("allow_methods ", buffer, ";");
		if (not tmp.empty())
			location.addMethod(tmp);

		tmp = Parser::find("autoindex ", buffer, ";");
		if (not tmp.empty())
			location.setAutoIndex(tmp);

		tmp = Parser::find("client_max_body_size ", buffer, ";");
		if (not tmp.empty())
			location.setMaxBodySize(tmp);

		tmp = Parser::find("return ", buffer, ";");
		if (not tmp.empty())
			location.setReturn(tmp);
			
		if (Parser::compare("}", buffer)) {
			buffer.erase(0, 1);
			return ;
		}
	}

	throw runtime_error("failed to parse location at: " + buffer);
}
