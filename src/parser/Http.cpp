#include "Http.hpp"
#include "Mime.hpp"
#include "WebServ.hpp"
#include "directive.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include <fstream>
#include <ostream>
#include <string>

using namespace std;

Http *Http::_instance = NULL;

Http::Http(void)
	: _autoindex(parser::AUTOINDEX_NOT_SET),
	  _max_body_size(0) {

}

Http::~Http(void) {

	delete WebServ::getInstance();
	delete Mime::getInstance();

}

Http *Http::getInstance(void) {

	if (_instance == NULL)
		_instance = new Http();

	return _instance;
}

void Http::configure(std::string filename) {

	if (parser::basename(filename) != ".conf")
		throw runtime_error("invalid .conf file format: " + filename);

	ifstream file(filename.c_str());
	if (not file)
		throw runtime_error("failed to open configuration file: " + filename);

	string buffer;

	for (string line; getline(file, line); buffer.append(line)) {
		
		if (line.find("#") != string::npos)
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

	addErrorPage(parser::DEFAULT_404_ERROR);
	addErrorPage(parser::DEFAULT_50x_ERROR);

	parser::http(*this, buffer);

	if (empty())
		throw runtime_error("no server avaliable");

	logger::info("configuration file parsed: " + filename);

	directive::setHttpDefaultValues(*this);
}

void Http::setAccessLog(string access_log) {

	directive::setAcessLog(access_log, _access_log);
}

string Http::getAccessLog(void) const {

	return _access_log;
}

void Http::setErrorLog(string error_log) {

	directive::setErrorLog(error_log, _error_log);
}

string Http::getErrorLog(void) const {

	return _error_log;
}

void Http::setRoot(string root) {

	directive::setRoot(root, _root);
}

string Http::getRoot(void) const {

	return _root;
}

void Http::setAutoIndex(string autoindex) {

	directive::setAutoIndex(autoindex, _autoindex);
}

void Http::setAutoIndex(bitset<2> autoindex) {

	_autoindex = autoindex;
}

bitset<2> Http::getAutoIndexBitSet(void) const {

	return _autoindex;
}

bool Http::getAutoIndex() const {

	return _autoindex == parser::AUTOINDEX_ON ? true : false;
}

void Http::setMaxBodySize(string max_body_size) {

	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Http::getMaxBodySize(void) const {
		
	return _max_body_size;
}

void Http::addIndex(string index) {

	directive::addIndex(index, _indexes);

}

void Http::setIndex(set<string> indexes) {

	_indexes = indexes;
}

set<string> Http::getIndexes(void) const {

	return _indexes;
}

void Http::addErrorPage(string error_page) {

	directive::addErrorPage(error_page, _error_pages);
}

void Http::setErrorPages(map<string, string> error_pages) {

	_error_pages = error_pages;
}

map<string, string> Http::getErrorPages(void) const {

	return _error_pages;
}

string Http::getErrorPageByCode(string code) const {

	if (_error_pages.find(code)->first.empty())
		return "";

	return _error_pages.find(code)->second;
}

void Http::addServer(Server server) {

	directive::addServer(server, _servers);
}

void Http::setServers(vector<Server> servers) {

	_servers = servers;
}

Server Http::getServerByListen(string listen) const {

	list<string> tmp = parser::split(listen, ':');

	if (!directive::validateHttpListen(listen) || tmp.size() != 2)
		return Server();

	vector<Server>::const_iterator it = _servers.begin();
	for (; it != _servers.end(); it++) {

		vector<string> listens = it->getListen();
		vector<string>::iterator listensIt = listens.begin();
		for(; listensIt != listens.end(); listensIt++) {
			list<string> tmp2 = parser::split(*listensIt, ':');
			if ((tmp2.front() == "0.0.0.0" || tmp2.front() == tmp.front()) && tmp2.back() == tmp.back())
				return *it;
		}
	}

	return Server();
}


Server Http::getServerByName(string name) const {

	list<string> listen = parser::split(name, ':');

	vector<Server>::const_iterator it = _servers.begin();
	for (; it != _servers.end(); it++) {

		set<string> ports;
		vector<string> listens = it->getListen();
		vector<string>::iterator listensIt = listens.begin();
		for (; listensIt != listens.end(); listensIt++) {

			list<string> tmp = parser::split(*listensIt, ':');
			ports.insert(tmp.back());
		}

		vector<string> names = it->getNames();
		vector<string>::iterator namesIt= names.begin();
		for(; namesIt!= names.end(); namesIt++) {

			if (listen.size() == 1 && listen.front() == *namesIt)
				return *it;

			if (listen.front() == *namesIt && ports.find(listen.back()) != ports.end())
				return *it;
		}
	}

	return Server();
}

vector<Server> Http::getServers(void) const {

	return _servers;
}

bool Http::empty(void) const {

	return _servers.empty() || _servers[0].empty();
}

void Http::start(void) {

	WebServ::getInstance()->run();
}

void Http::stop(void) {

	WebServ::getInstance()->stop();
}

ostream &operator<<(ostream &os, const Http &src) {

	os << "http {" << endl;
	os << "\tclient_max_body_size " << src.getMaxBodySize() << ";" << endl;
	os << "\troot " << src.getRoot() << ";" << endl;
	os << "\tautoindex " << (src.getAutoIndex() ? "on" : "off") << ";" << endl;
	os << "\taccess_log " << src.getAccessLog() << ";" << endl;
	os << "\terror_log " << src.getErrorLog() << ";" << endl;
	
	os << "\tindex";
	set<string> indexs = src.getIndexes();
	for (set<string>::iterator it = indexs.begin(); it != indexs.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	map<string, string> error_pages = src.getErrorPages();
	for (map<string, string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		os << "\terror_page " << it->first << " " << (*it).second << ";" << endl;

	vector<Server> servers = src.getServers();
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
		os << *it << endl;

	os << "}";

	return os;
}
