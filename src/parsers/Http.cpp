#include "Http.hpp"
#include "WebServ.hpp"
#include "directive.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include <fstream>
#include <ostream>
#include <string>

using namespace std;

WebServ *webserv;

Http::Http(string filename)
	: _autoindex(AUTOINDEX_NOT_SET),
	  _max_body_size(0) {

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

	addErrorPage(DEFAULT_404_ERROR);
	addErrorPage(DEFAULT_50x_ERROR);

	parser::http(*this, buffer);

	if (empty())
		throw runtime_error("no server avaliable");

	logger::info("configuration file parsed: " + filename);

	directive::setHttpDefaultValues(*this);
}

Http::Http(const Http &src) {

	*this = src;
}

Http &Http::operator=(const Http &rhs) {
	
	if (this == &rhs)
		return *this;

	_access_log = rhs._access_log;
	_error_log = rhs._error_log;
	_root = rhs._root;
	_autoindex = rhs._autoindex;
	_max_body_size = rhs._max_body_size;
	_indexes = rhs._indexes;
	_error_pages = rhs._error_pages;
	_servers = rhs._servers;

	return *this;
}

Http::~Http(void) {

	delete webserv;

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

	return _autoindex == AUTOINDEX_ON ? true : false;
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

	vector<Server>::const_iterator it = _servers.begin();
	for (; it != _servers.end(); it++) {

		vector<string> names = it->getNames();
		vector<string>::iterator namesIt= names.begin();
		for(; namesIt!= names.end(); namesIt++) {
			if (name == *namesIt)
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

	webserv = new WebServ(this);

	webserv->run();
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
