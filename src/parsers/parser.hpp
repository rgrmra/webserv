/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:50:23 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/26 22:44:04 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <string>
#include <list>

#define BYTE 1
#define KILOBYTE (1024 * BYTE)
#define MEGABYTE (1024 * KILOBYTE)
#define GIGABYTE (1024 * MEGABYTE)

#define ROOT "./"
#define ACCESS_LOG "./var/log/access.log"
#define ERROR_LOG "./var/log/error.log"

using namespace std;

namespace parser {

	string toLower(string text);
	string find(string key, string &configuration_file, string delimiter);
	bool compare(string key, string &configuration_file);
	list<string> split(string text, char delimiter);
	void erase(string &buffer, string text, size_t quantity);
	void rerase(string &buffer, string text, size_t quantity);
	void replace(string &buffer, char from, char to);
	void trim(string &buffer, string set);
	string basename(string text);
	void http(Http &http, string &configuration_file);
	void server(Server &server, string &configuration_file);
	void location(Location &location, string &configuration_file);
	void limit_except(Location &location, string &configuration_file);

}

#endif /* PARSER_HPP */
