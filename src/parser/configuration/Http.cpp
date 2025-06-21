#include "Http.hpp"
#include "Mime.hpp"
#include "WebServ.hpp"
#include "directive.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include <bitset>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

Http *Http::_instance = NULL;

Http::Http(void)
	: _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0),
	  _signal(0) {}

Http::~Http(void)
{
	delete WebServ::getInstance();
	delete Mime::getInstance();
}

Http *Http::getInstance(void)
{
	if (_instance == NULL)
		_instance = new Http();

	return _instance;
}

void Http::configure(std::string filename)
{
	if (parser::basename(filename) != ".conf")
		throw std::runtime_error("invalid .conf file format: " + filename);

	std::ifstream file(filename.c_str());
	if (not file)
		throw std::runtime_error("failed to open configuration file: " + filename);

	std::string buffer;

	for (std::string line; getline(file, line); buffer.append(line)) {
		
		if (line.find("#") != std::string::npos)
			line = line.substr(0, line.find_first_of("#"));

		parser::trim(line, " \n\t\r\v\f");
	}

	file.close();

	parser::replace(buffer, '\t', ' ');

	parser::erase(buffer, "  ", 1);
	parser::erase(buffer, " {", 1);
	parser::rerase(buffer, "{ ", 1);
	parser::erase(buffer, " }", 1);
	parser::rerase(buffer, "} ", 1);
	parser::erase(buffer, " ;", 1);
	parser::rerase(buffer, "; ", 1);

	parser::http(*this, buffer);

	if (empty())
		throw std::runtime_error("no server avaliable");

	logger::info("configuration file parsed: " + filename);

	directive::setHttpDefaultValues(*this);
}

void Http::setRoot(std::string root)
{
	directive::setRoot(root, _root);
}

std::string Http::getRoot(void) const
{
	return _root;
}

void Http::setAutoIndex(std::string autoindex)
{
	directive::setAutoIndex(autoindex, _autoindex);
}

void Http::setAutoIndex(std::bitset<2> autoindex)
{
	_autoindex = autoindex;
}

std::bitset<2> Http::getAutoIndexBitSet(void) const
{
	return _autoindex;
}

bool Http::getAutoIndex() const
{
	return _autoindex == parser::AUTOINDEX_ON ? true : false;
}

void Http::setWebDav(std::string webdav)
{
	directive::setWebDav(webdav, _webdav);
}

void Http::setWebDav(std::bitset<2> webdav)
{
	_webdav = webdav;
}

std::bitset<2> Http::getWebDavBitSet(void) const
{
	return _webdav;
}

bool Http::getWebDav() const
{
	return _webdav == parser::WEB_DAV_ON ? true : false;
}

void Http::setMaxBodySize(std::string max_body_size)
{
	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Http::getMaxBodySize(void) const
{
	return _max_body_size;
}

void Http::addIndex(std::string index)
{
	directive::addIndex(index, _indexes);

}

void Http::setIndex(std::set<std::string> indexes)
{
	_indexes = indexes;
}

std::set<std::string> Http::getIndexes(void) const
{
	return _indexes;
}

void Http::addErrorPage(std::string error_page)
{
	directive::addErrorPage(error_page, _error_pages);
}

void Http::setErrorPages(std::map<std::string, std::string> error_pages)
{
	_error_pages = error_pages;
}

std::map<std::string, std::string> Http::getErrorPages(void) const
{
	return _error_pages;
}

std::string Http::getErrorPageByCode(std::string code) const
{
	std::map<std::string, std::string>::const_iterator error_page = _error_pages.find(code);
	if (error_page ==_error_pages.end())
		return "";

	return error_page->second;
}

void Http::addServer(Server server)
{
	directive::addServer(server, _servers);
}

void Http::setServers(std::vector<Server> servers)
{
	_servers = servers;
}

Server Http::getServerByListen(std::string listen) const
{
	std::list<std::string> host = parser::split(listen, ':');
	if (!directive::validateHttpListen(listen) || host.size() != 2)
		return Server();

	std::vector<Server>::const_iterator server = _servers.begin();
	for (; server != _servers.end(); server++)
	{
		std::vector<std::string> listens = server->getListen();
		std::vector<std::string>::iterator listen = listens.begin();
		for(; listen != listens.end(); listen++)
		{
			std::list<std::string> server_host = parser::split(*listen, ':');
			if (server_host.back() != host.back())
				continue;
			
			if (server_host.front() == "0.0.0.0")
				return *server;

			if (server_host.front() == host.front())
				return *server;
		}
	}

	return Server();
}

Server Http::getServerByName(std::string name) const
{
	std::list<std::string> host = parser::split(name, ':');
	std::vector<Server>::const_iterator server  = _servers.begin();
	for (; server != _servers.end(); server++)
	{
		std::set<std::string> ports;
		std::vector<std::string> listens = server->getListen();
		std::vector<std::string>::iterator listen = listens.begin();
		for (; listen != listens.end(); listen++) {

			std::list<std::string> tmp = parser::split(*listen, ':');
			ports.insert(tmp.back());
		}

		std::vector<std::string> names = server->getNames();
		std::vector<std::string>::iterator name= names.begin();
		for(; name!= names.end(); name++)
		{
			if (host.front() != *name)
				continue;

			if (ports.find(host.back()) == ports.end())
				continue;

			return *server;
		}
	}

	return Server();
}

std::vector<Server> Http::getServers(void) const
{
	return _servers;
}

bool Http::empty(void) const
{
	return _servers.empty() || _servers[0].empty();
}

void Http::start(void)
{
	WebServ::getInstance()->run();
}

void Http::stop(int signal)
{
	_signal = signal;

	WebServ::getInstance()->stop();
}

int Http::getSignal(void)
{
	return _signal;
}
