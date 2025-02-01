#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>

#define BYTE 1
#define KILOBYTE (1024 * BYTE)
#define MEGABYTE (1024 * KILOBYTE)
#define GIGABYTE (1024 * MEGABYTE)

#define DEFAULT_ROOT "./var/www/html"
#define DEFAULT_ACCESS_LOG (DEFAULT_ROOT "/access.log")
#define DEFAULT_ERROR_LOG (DEFAULT_ROOT "/error.log")
#define DEFAULT_MAX_BODY_SIZE "10M"
#define DEFAULT_INDEXES "index.htm index.html"
#define DEFAULT_HOST "0.0.0.0"
#define DEFAULT_PORT "80"
#define DEFAULT_ALLOW_METHODS "GET POST PUT PATCH DELETE"

#define DEFAULT_404_ERROR ("404 " DEFAULT_ROOT "/default/404.html")
#define DEFAULT_50x_ERROR ("500 502 503 504 " DEFAULT_ROOT "/default/50x.html")

enum {
	AUTOINDEX_NOT_SET = -1,
	AUTOINDEX_OFF = 0,
	AUTOINDEX_ON = 1
};

class Http;
class Server;
class Location;

namespace parser {

	size_t toSizeT(std::string value);
	std::string toString(std::size_t value);
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
