#include "Http.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "directive.hpp"
#include "logger.hpp"
#include "method.hpp"
#include "parser.hpp"
#include "standard.hpp"
#include <bitset>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

bool directive::validateHttpListen(std::string listen)
{
	if (listen.empty())
		return false;

	if (listen.find_first_not_of(".:0123456789") != std::string::npos)
		return false;

	if (listen.at(0) == ':' || parser::lastCharacter(listen) == ':')
		return false;

	if (listen.find("::") != std::string::npos)
		return false;

	std::list<std::string> tmp = parser::split(listen, ':');
	if (tmp.size() > 2)
		return false;

	return true;
}

bool directive::validateHttpHost(std::string host)
{
	if (host.empty())
		return false;

	if (host.find_first_not_of(".0123456789") != std::string::npos)
		return false;

	if (host.find("..") != std::string::npos)
		return false;

	if (host.at(0) == '.' || parser::lastCharacter(host) == '.')
		return false;

	std::list<std::string> octets = parser::split(host, '.');

	if (octets.size() != 4)
		return false;

	std::list<std::string>::iterator it = octets.begin();
	for (; it != octets.end(); ++it)
		if (parser::toSizeT(*it) > 255)
			return false;

	return true;
}

bool directive::validateHttpPort(std::string port)
{
	if (port.empty())
		return false;

	if (port.find_first_not_of("0123456789") != std::string::npos)
		return false;

	size_t port_value = parser::toSizeT(port);

	if (port_value < 1024 || port_value > 49151)
		return false;

	return true;
}

void directive::addListen(std::string listen, std::vector<std::string> &_listen)
{
	if (listen.empty())
		return;

	std::string host = standard::HOST;
	std::string port = standard::PORT;

	if (not directive::validateHttpListen(listen))
		throw std::runtime_error("invalid listen: " + listen);

	std::list<std::string> tmp = parser::split(listen, ':');

	if (tmp.size() >= 1 && directive::validateHttpPort(tmp.back()))
		port = tmp.back();
	else if (tmp.size() == 1 && directive::validateHttpHost(tmp.back()))
		host = tmp.back();
	else
		throw std::runtime_error("invalid listen: " + listen);

	if (tmp.size() == 2 && directive::validateHttpHost(tmp.front()))
		host = tmp.front();
	else if (tmp.size() == 2)
		throw std::runtime_error("invalid host: " + tmp.front());

	listen = host + ":" + port;

	std::vector<std::string>::iterator it = _listen.begin();
	for (; it != _listen.end(); ++it)
		if (*it == listen)
			throw std::runtime_error("duplicated listen: " + listen);

	_listen.push_back(listen);
}

bool directive::validateName(std::string name)
{
	if (name.empty())
		return false;

	if (name.at(0) == '-' || parser::lastCharacter(name) == '-')
		return false;

	if (name.at(0) == '.' || parser::lastCharacter(name) == '.')
		return false;

	for (std::string::iterator it = name.begin(); it != name.end(); it++)
	{
		if (!isalnum(*it) && *it != '-' && *it != '.')
			return false;

		if (*it == '-' && *(it + 1) == '-')
			return false;

		if (*it == '.' && *(it + 1) == '.')
			return false;
	}

	return true;
}

void directive::addName(std::string name, std::vector<std::string> &_name)
{
	if (name.empty())
		return;

	std::list<std::string> names = parser::split(name, ' ');

	for (std::list<std::string>::iterator it = names.begin(); it != names.end(); it++) {
		if (!directive::validateName(*it))
			throw std::runtime_error("invalid server name: " + *it);

		_name.push_back(parser::toLower(*it));
	}
}

bool directive::isValidRequestTarget(const std::string& target)
{
	return isValidAbsolutePath(target) || isValidAbsoluteURI(target);
}

bool directive::isValidAbsolutePath(const std::string& target)
{
	if (target.empty() || target[0] != '/')
		return false;

	if (target.find_first_not_of(standard::ALLOWED_CHARACTERS) != std::string::npos)
		return false;

	return true;
}

bool directive::isValidAbsoluteURI(const std::string& target)
{
	size_t schemeEnd = target.find("://");
	if (schemeEnd == std::string::npos)
		return false;

	for (size_t i = 0; i < schemeEnd; ++i)
	{
		char c = target[i];
		if (!isalpha(c))
			return false;
	}

	size_t pathStart = schemeEnd + 3;
	if (pathStart >= target.size())
		return false;

	return isValidAbsolutePath(target.substr(pathStart));
}

void directive::setURI(std::string uri, std::string &_uri)
{
	if (uri.empty())
		return;

	if (!directive::isValidAbsolutePath(uri))
		throw std::runtime_error("invalid path: " + uri);

	_uri = parser::formatPath(uri);
}

bool directive::validateHttpMethod(std::string method)
{
	std::set<std::string> &allowed_methods = method::getAllowedMethods();

	std::set<std::string>::iterator it = allowed_methods.begin();
	for (; it != allowed_methods.end(); it++)
		if (*it == method)
			return true;

	return false;
}

void directive::addMethod(std::string method, std::set<std::string> &_allow_methods)
{
	if (method.empty())
		return;

	std::list<std::string> methods = parser::split(method, ' ');

	_allow_methods.clear();

	std::list<std::string>::iterator it = methods.begin();
	for (; it != methods.end(); it++)
	{
		if (not validateHttpMethod(*it))
			throw std::runtime_error("invalid method: " + *it);

		_allow_methods.insert(*it);
	}
}

void directive::setDenyMethods(std::string deny_methods, bool &_deny_methods)
{
	if (deny_methods.empty())
		return;

	if (deny_methods == "all")
		_deny_methods = true;
	else
		throw std::runtime_error("invalid deny: " + deny_methods);
}

void directive::setRoot(std::string root, std::string &_root)
{
	if (root.empty())
		return;

	if (root.find_first_of(" ") != std::string::npos)
		throw std::runtime_error("invalid root: " + root);

	_root = root;
}

void directive::setAutoIndex(std::string autoindex, std::bitset<2> &_autoindex)
{
	if (autoindex.empty())
		return;

	if (autoindex == "on")
		_autoindex = parser::AUTOINDEX_ON;
	else if (autoindex == "off")
		_autoindex = parser::AUTOINDEX_OFF;
	else
		throw std::runtime_error("invalid autoindex: " + autoindex);
}

void directive::setWebDav(std::string webdav, std::bitset<2> &_webdav)
{
	if (webdav.empty())
		return;

	if (webdav== "on")
		_webdav= parser::WEB_DAV_ON;
	else if (webdav == "off")
		_webdav= parser::WEB_DAV_OFF;
	else
		throw std::runtime_error("invalid webdav: " + webdav);
}

void directive::setMaxBodySize(std::string max_body_size, size_t &_max_body_size)
{
	if (max_body_size.empty())
		return;

	size_t pos = max_body_size.find_first_not_of("0123456789");

	std::string format;
	if (pos != std::string::npos)
		format = max_body_size.substr(pos, max_body_size.size() - pos);

	size_t tmp = parser::toSizeT(max_body_size);
	if (tmp == 0)
		_max_body_size = std::numeric_limits<size_t>::max();
	else
		_max_body_size = tmp;

	if (format.empty() || format == "B")
		_max_body_size *= size::BYTE;
	else if (format == "K")
		_max_body_size *= size::KILOBYTE;
	else if (format == "M")
		_max_body_size *= size::MEGABYTE;
	else if (format == "G")
		_max_body_size *= size::GIGABYTE;
	else
		throw std::runtime_error("invalid value to max_body_size: " + max_body_size);
}

void directive::addIndex(std::string index, std::set<std::string> &_index)
{
	if (index.empty())
		return;

	std::list<std::string> indexes = parser::split(index, ' ');

	_index.clear();

	std::list<std::string>::iterator it = indexes.begin();
	for (; it != indexes.end(); it++)
		_index.insert(*it);
}

void directive::setFastCgi(std::string fastcgi, std::string &_fastcgi)
{
	if (fastcgi.empty())
		return;

	_fastcgi = fastcgi;
}

void directive::setFastCgiExtension(std::string extensions, std::set<std::string> &_extensions)
{
	if (extensions.empty())
		return;

	_extensions.clear();

	std::list<std::string> tmp = parser::split(extensions, ' ');

	for (std::list<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		_extensions.insert((it->at(0) == '.' ? *it : "." + *it));
}

void directive::addErrorPage(std::string error_page, std::map<std::string, std::string> &_error_pages)
{
	if (error_page.empty())
		return;

	std::list<std::string> tmp = parser::split(error_page, ' ');

	if (tmp.size() < 2)
		throw std::runtime_error("invalid error page: " + error_page);

	if (tmp.back().at(0) != '.' && tmp.back().at(0) != '/')
		tmp.back() = "./" + tmp.back();

	std::string path = tmp.back();
	tmp.pop_back();

	for (std::list<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		if (not directive::validateHttpCode(*it))
			throw std::runtime_error("invalid error code: " + *it);

		_error_pages[*it] = path;
	}
}

void directive::mergeErrorPages(std::map<std::string, std::string> error_pages, std::map<std::string, std::string> &_error_pages)
{
	std::map<std::string, std::string>::iterator it = error_pages.begin();
	for (; it != error_pages.end(); it++)
		if (_error_pages[it->first].empty())
			_error_pages[it->first] = it->second;
}

bool directive::validateHttpCode(std::string code)
{
	if (code.find_first_not_of("0123456789") != std::string::npos)
		return false;

	size_t tmp = parser::toSizeT(code);
	if (tmp < 200 || tmp > 599)
		return false;

	return true;
}

static bool validateReturn(std::string &tmp)
{
	if (tmp.empty())
		return true;

	std::string quote = std::string(1, tmp.at(0));
	if (quote.at(0) != '\'' && quote.at(0) != '\"')
		return true;

	size_t first = tmp.find_first_of(quote);
	size_t last = tmp.find_last_of(quote);
	if (first == std::string::npos && last == std::string::npos)
		return true;

	if (first == std::string::npos || last == std::string::npos)
		return false;

	first++;

	std::string text = tmp.substr(first, last - first);
	if (text.size() != tmp.size() - 2)
		return false;

	if (text.find_first_of(quote) != std::string::npos)
		return false;

	tmp = text;

	return true;
}

void directive::setReturn(std::string value, std::string &_code, std::string &_uri)
{
	if (value.empty())
		return;

	std::list<std::string> tmp = parser::split(value, ' ');

	if (tmp.size() < 1 || tmp.size() > 2)
		throw std::runtime_error("invalid return: " + value);

	if (not directive::validateHttpCode(tmp.front()))
		throw std::runtime_error("invalid return code: " + tmp.front());

	_code = tmp.front();

	if (tmp.size() == 1)
		return _uri.clear();

	if (!validateReturn(tmp.back()))
		throw std::runtime_error("invalid return uri/text: " + tmp.back());

	_uri = tmp.back();
}

void directive::addServer(Server server, std::vector<Server> &_servers)
{
	std::vector<Server>::iterator serverIt = _servers.begin();
	for (; serverIt != _servers.end(); serverIt++)
	{
		std::vector<std::string> exitenListen = serverIt->getListen();
		std::vector<std::string>::iterator exitenListenIt = exitenListen.begin();
		for (; exitenListenIt != exitenListen.end(); exitenListenIt++)
		{
			std::list<std::string> tmp = parser::split(*exitenListenIt, ':');

			std::vector<std::string> newListen = server.getListen();
			std::vector<std::string>::iterator newListenIt = newListen.begin();
			for (; newListenIt != newListen.end(); newListenIt++)
			{
				std::list<std::string> tmp2 = parser::split(*newListenIt, ':');
				if (tmp2.front() != standard::HOST
					&& tmp.front() == tmp2.front()
					&& tmp.back() == tmp2.back())
				{
					logger::warning("conflicting server name \""
						+ (server.getNames().size() ? server.getNames().at(0) : "")
						+ "\" on " + *newListenIt + ", ignored");

					newListen.erase(newListenIt);
					server.setListen(newListen);

					return directive::addServer(server, _servers);
				}
			}
		}
	}

	if (server.getLocations().empty())
	{
		Location location;
		location.setURI("/");
		server.addLocation(location);
	}

	_servers.push_back(server);
}

void directive::setHttpDefaultValues(Http &http)
{
	if (http.getMaxBodySize() == 0)
		http.setMaxBodySize(standard::MAX_BODY_SIZE);

	if (http.getIndexes().size() == 0)
		http.addIndex(standard::DEFAULT_INDEXES);

	if (http.getAutoIndexBitSet() == parser::AUTOINDEX_NOT_SET)
		http.setAutoIndex(parser::AUTOINDEX_OFF);

	if (http.getWebDavBitSet() == parser::WEB_DAV_NOT_SET)
		http.setWebDav(parser::WEB_DAV_OFF);

	if (http.getRoot().empty())
		http.setRoot(standard::ROOT_DIR);

	std::vector<Server> servers = http.getServers();
	std::vector<Server>::iterator it = servers.begin();
	for (; it != servers.end(); it++)
		directive::setServerDefaultValues(http, *it);

	http.setServers(servers);
}

void directive::setServerDefaultValues(Http &http, Server &server)
{
	if (server.getMaxBodySize() == 0)
		server.setMaxBodySize(parser::toString(http.getMaxBodySize()));

	if (server.getRoot().empty())
		server.setRoot(http.getRoot());

	if (server.getIndexes().empty())
		server.setIndexes(http.getIndexes());

	if (server.getAutoIndexBitSet() == parser::AUTOINDEX_NOT_SET)
		server.setAutoIndex(http.getAutoIndexBitSet());

	if (server.getWebDavBitSet() == parser::WEB_DAV_NOT_SET)
		server.setWebDav(http.getWebDavBitSet());

	std::map<std::string, std::string> error_pages = server.getErrorPages();
	directive::mergeErrorPages(http.getErrorPages(), error_pages);
	server.setErrorPages(error_pages);

	std::map<std::string, Location> locations = server.getLocations();
	std::map<std::string, Location>::iterator it = locations.begin();
	for (; it != locations.end(); it++)
		directive::setLocationDefaultValues(server, it->second);

	server.setLocations(locations);
}

void directive::setLocationDefaultValues(Server &server, Location &location)
{
	if (location.getRoot().empty())
		location.setRoot(server.getRoot());

	if (location.getIndexes().empty())
		location.setIndexes(server.getIndexes());

	if (location.getDenyMethods() == false)
		location.addMethod(method::GET);

	if (location.getAutoIndexBitSet() == parser::AUTOINDEX_NOT_SET)
		location.setAutoIndex(server.getAutoIndexBitSet());

	if (location.getWebDavBitSet() == parser::WEB_DAV_NOT_SET)
		location.setWebDav(server.getWebDavBitSet());

	if (location.getMaxBodySize() == 0)
		location.setMaxBodySize(parser::toString(server.getMaxBodySize()));

	std::map<std::string, std::string> error_pages = location.getErrorPages();
	directive::mergeErrorPages(server.getErrorPages(), error_pages);
	location.setErrorPages(error_pages);

	if (server.getReturnCode().size())
		location.setReturn(server.getReturnCode() + " " + server.getReturnURI());
}
