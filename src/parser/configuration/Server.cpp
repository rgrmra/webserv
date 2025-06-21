#include "Server.hpp"
#include "Location.hpp"
#include "directive.hpp"
#include "parser.hpp"
#include <bitset>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

Server::Server(void)
	: _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0) {}

Server::Server(std::string &configuration_file)
	: _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0)
{
	parser::server(*this, configuration_file);

	if (this->empty())
		throw std::runtime_error("no listen defined");
}

Server::Server(const Server &src)
{
	*this = src;
}

Server &Server::operator=(const Server &rhs)
{
	if (this == &rhs)
		return *this;

	_listen = rhs._listen;
	_names = rhs._names;
	_root = rhs._root;
	_autoindex = rhs._autoindex;
	_webdav = rhs._webdav;
	_max_body_size = rhs._max_body_size;
	_indexes = rhs._indexes;
	_error_pages = rhs._error_pages;
	_locations = rhs._locations;
	_return_code = rhs._return_code;
	_return_uri = rhs._return_uri;

	return *this;
}

Server::~Server(void) {}

void Server::addListen(std::string listen)
{
	directive::addListen(listen, _listen);
}

void Server::setListen(std::vector<std::string> listen)
{
	if (listen.empty())
	{
		std::stringstream ss;
		ss << "no listen avaliable to server_name \"";
		ss << (_names.size() ? _names[0] : "") + "\"";
		throw std::runtime_error(ss.str());
	}

	_listen = listen;
}

std::vector<std::string> Server::getListen(void) const
{
	return _listen;
}

void Server::addName(std::string name)
{
	directive::addName(name, _names);
}

void Server::setNames(std::vector<std::string> names)
{
	_names = names;
}

std::vector<std::string> Server::getNames(void) const
{
	return _names;
}

void Server::setRoot(std::string root)
{
	directive::setRoot(root, _root);
}

std::string Server::getRoot(void) const
{
	return _root;
}

void Server::setAutoIndex(std::string autoindex)
{
	directive::setAutoIndex(autoindex, _autoindex);
}

void Server::setAutoIndex(std::bitset<2> autoindex)
{
	_autoindex = autoindex;
}

std::bitset<2> Server::getAutoIndexBitSet(void) const
{
	return _autoindex;
}

bool Server::getAutoIndex(void) const
{
	return _autoindex == parser::AUTOINDEX_ON ? true : false;
}

void Server::setWebDav(std::string webdav)
{
	directive::setWebDav(webdav, _webdav);
}

void Server::setWebDav(std::bitset<2> webdav)
{
	_webdav = webdav;
}

std::bitset<2> Server::getWebDavBitSet(void) const
{
	return _webdav;
}

bool Server::getWebDav() const
{
	return _webdav == parser::WEB_DAV_ON ? true : false;
}

void Server::setMaxBodySize(std::string max_body_size)
{
	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Server::getMaxBodySize(void) const
{
	return _max_body_size;
}

void Server::addIndex(std::string index)
{
	directive::addIndex(index, _indexes);
}

void Server::setIndexes(std::set<std::string> indexes)
{
	_indexes = indexes;
}

std::set<std::string> Server::getIndexes(void) const
{
	return _indexes;
}

void Server::addErrorPage(std::string error_page)
{
	directive::addErrorPage(error_page, _error_pages);
}

void Server::setErrorPages(std::map<std::string, std::string> error_pages)
{
	_error_pages = error_pages;
}

std::map<std::string, std::string> Server::getErrorPages(void) const
{
	return _error_pages;
}

std::string Server::getErrorPageByCode(std::string code) const
{
	std::map<std::string, std::string>::const_iterator error_page = _error_pages.find(code);
	if (error_page == _error_pages.end())
		return "";

	return error_page->second;
}

void Server::addLocation(Location location)
{
	if (_locations.find(location.getURI()) != _locations.end())
		throw std::runtime_error("duplicated location: " + location.getURI());

	_locations[location.getURI()] = location;
}

void Server::setLocations(std::map<std::string, Location> locations)
{
	_locations = locations;
}

Location Server::getLocationByURI(std::string uri) const
{
	std::map<std::string, Location>::const_iterator location = _locations.find(uri);
	if (location == _locations.end())
		return Location();

	return location->second;
}

std::map<std::string, Location> Server::getLocations(void) const
{
	return _locations;
}

void Server::setReturn(std::string value)
{
	directive::setReturn(value, _return_code, _return_uri);
}

std::string Server::getReturnCode(void) const
{
	return _return_code;
}

std::string Server::getReturnURI(void) const
{
	return _return_uri;
}

bool Server::empty(void) const
{
	return _listen.empty();
}
