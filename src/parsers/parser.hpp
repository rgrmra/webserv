#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>

class Http;
class Server;
class Location;

namespace parser {

enum { AUTOINDEX_NOT_SET = -1, AUTOINDEX_OFF = 0, AUTOINDEX_ON = 1 };

static const size_t BYTE = 1;
static const size_t KILOBYTE = (1024 * BYTE);
static const size_t MEGABYTE = (1024 * KILOBYTE);
static const size_t GIGABYTE = (1024 * MEGABYTE);

static const std::string DEFAULT_ROOT = "./var/www/html";
static const std::string DEFAULT_ACCESS_LOG = (DEFAULT_ROOT + "/access.log");
static const std::string DEFAULT_ERROR_LOG = (DEFAULT_ROOT + "/error.log");
static const std::string DEFAULT_MAX_BODY_SIZE = "10M";
static const std::string DEFAULT_INDEXES = "index.htm index.html";
static const std::string DEFAULT_HOST = "0.0.0.0";
static const std::string DEFAULT_PORT = "80";
static const std::string DEFAULT_ALLOW_METHODS = "GET POST DELETE";

static const std::string DEFAULT_404_ERROR =
    ("404 " + DEFAULT_ROOT + "/default/404.html");
static const std::string DEFAULT_50x_ERROR =
    ("500 502 503 504 " + DEFAULT_ROOT + "/default/50x.html");

size_t toSizeT(std::string value);
std::string toString(std::size_t value);
std::string toLower(std::string text);
std::string find(std::string key, std::string &configuration_file,
                 std::string delimiter);
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
} // namespace parser

#endif /* PARSER_HPP */
