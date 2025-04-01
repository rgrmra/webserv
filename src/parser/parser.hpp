#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>

class Http;
class Server;
class Location;

namespace parser {

	enum {
		AUTOINDEX_NOT_SET = -1,
		AUTOINDEX_OFF = 0,
		AUTOINDEX_ON = 1
	};

	const char &lastCharacter(const std::string &text);
	size_t toSizeT(std::string value);
	std::string toString(std::size_t value);
	std::string toUpper(std::string text);
	std::string toLower(std::string text);
	std::string find(std::string key, std::string &configuration_file, std::string delimiter);
	bool compare(std::string key, std::string &configuration_file);
	std::list<std::string> split(std::string text, char delimiter);
	void erase(std::string &buffer, std::string text, std::size_t quantity);
	void rerase(std::string &buffer, std::string text, std::size_t quantity);
	void replace(std::string &buffer, char from, char to);
	void trim(std::string &buffer, std::string set);
	std::string basename(std::string text);
	void http(Http &http, std::string &configuration_file);
	void server(Server &server, std::string &configuration_file);
	void location(Location &location, std::string &configuration_file);
	void limit_except(Location &location, std::string &configuration_file);

}

#endif /* PARSER_HPP */
