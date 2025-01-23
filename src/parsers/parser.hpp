/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:50:23 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 12:37:33 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <string>
#include <list>

using namespace std;

namespace parser {

	string find(string key, string &configuration_file, string delimiter);
	bool compare(string key, string &configuration_file);
	list<string> split(string text, char delimiter);
	void erase(string &buffer, string text, size_t quantity);
	void replace(string &buffer, char from, char to);
	void trim(string &buffer, string set);
	string basename(string text);
	bool validadeHttpCode(string code);
	void http(Http &http, string &configuration_file);
	void server(Server &server, string &configuration_file);
	void location(Location &location, string &configuration_file);

}

#endif /* PARSER_HPP */
