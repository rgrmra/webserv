/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:50:23 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/22 14:26:08 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <functional>
#include <string>
#include <list>

using namespace std;

class Parser {
	private:
		Parser(void);
		Parser(const Parser &src);
		Parser &operator=(const Parser &rhs);
		~Parser(void);

	protected:
		static string find(string key, string &configuration_file, string delimiter);
		static bool compare(string key, string &configuration_file);

	public:
		static list<string> split(string text);
		static void erase(string &buffer, string text, size_t quantity);
		static void replace(string &buffer, char from, char to);
		static void trim(string &buffer, string set);
		static string basename(string text);
		static size_t stringToSizeT(string text);
		static void http(Http &http, string &configuration_file);
		static void server(Server &server, string &configuration_file);
		static void location(Location &location, string &configuration_file);
};

#endif /* PARSER_HPP */
