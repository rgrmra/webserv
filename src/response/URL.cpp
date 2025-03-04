#include "URL.hpp"
#include "Connection.hpp"
#include "parser.hpp"
#include "process.hpp"
#include <cctype>
#include <csetjmp>
#include <iostream>
#include <sstream>

using namespace std;

URL::URL(Connection *connection)
	: _connection(connection) {

	string uri = connection->getPath();

	_scheme = "http";

	list<string> tmp = parser::split(connection->getHost(), ':');
	if (tmp.size())
		_host = tmp.front();

	if (tmp.size() > 1)
		_port = tmp.back();

	size_t pos = uri.find_first_of("?");
	if (pos != string::npos) {
		_path = uri.substr(0, pos);
		_query = uri.substr(pos + 1);
	} else {
		_path = uri;
		_query = "";
	}

	pos = _path.find_last_of("/");
	if (pos != string::npos)
		_file = _path.substr(pos + 1, _path.size());

	processPath(_path);

	pos = _file.find_last_of(".");
	if (pos != string::npos)
		_extension = _file.substr(pos, _file.size());
}

URL::URL(const URL &src) {

	*this = src;
}

URL &URL::operator=(const URL &rhs) {

	if (this == &rhs)
		return *this;

	_connection = rhs._connection;
	_scheme = rhs._scheme;
	_host = rhs._host;
	_port = rhs._port;
	_path = rhs._path;
	_query = rhs._query;
	_file = rhs._file;
	_extension = rhs._extension;

	return *this;
}

URL::~URL(void) {

}

void URL::convertCharacters(string &path) {

	std::string output;

	for (size_t i = 0; i < path.length(); ++i) {

		if (path[i] == '%' && i + 2 < path.length()) {

			string tmp = path.substr(i, i + 2);

			if (tmp.find_first_of("0123456789ABCDEFG") != string::npos) {
				output += '%';
				continue;
			}

			std::istringstream iss(tmp);
			int value;

			iss >> std::hex >> value;
			output += static_cast<char>(value);

			i += 2;
		}

		output += path[i];
	}
	path = output;
}

void URL::formatPath(std::string path) {

	list<string> new_path;
	list<string> splited_path = parser::split(path, '/');

	list<string>::iterator it = splited_path.begin();
	for (; it != splited_path.end(); it ++) {

		if (*it == ".")
			continue;

		if (*it == "..") {
			if (new_path.size())
				new_path.pop_back();

			continue;
		}

		convertCharacters(*it);

		new_path.push_back(*it);
	}

	string tmp;

	for (it = new_path.begin(); it != new_path.end(); it++)
		tmp += "/" + *it;

	_path = tmp + (path.at(path.size() -1) == '/' ? "/" : "");
}

void URL::processPath(string path) {

	Server server = _connection->getServer();
	formatPath(path);

	list<string> paths;
	while (path.size()) {
		paths.push_back(path);

		size_t pos = path.find_first_of("/");
		if (pos == 0) {
			paths.push_back("/");
			break;
		}

		path = path.substr(0, pos);
	}

	Location location;

	for (list<string>::iterator it = paths.begin(); it != paths.end(); it++) {
		location = server.getLocationByURI(*it);
		if (!location.empty())
			break;
	}

	if (process::isDirectory(location.getRoot() + _path))
		_file = process::checkIndex(location, _path);

	_connection->setLocation(location);
}

string URL::getScheme(void) const {

	return _scheme;
}

string URL::getHost(void) const {

	return _host;
}

string URL::getPort(void) const {

	return _port;
}

string URL::getPath(void) const {

	return _path;
}

string URL::getQuery(void) const {

	return _query;
}

string URL::getFile(void) const {

	return _file;
}

string URL::getExtension(void) const {

	return _extension;
}

std::string URL::getAbsolutePath(void) const {

	return _connection->getLocation().getRoot() + _path;
}

std::string URL::getLocation(void) const {

	return (_scheme.size() ? _scheme : "http") + "://"
		+ _host + (_port.size() ? ":" + _port : "")
		+ _path.substr(0, _path.size() - _file.size());
}

ostream &operator<<(ostream &os, const URL &src) {

	os << (src.getScheme().size() ? src.getScheme() + "://" : "http://")
		+ src.getHost() + (src.getPort().size() ? ":" + src.getPort() : "")
		+ src.getPath() + (src.getQuery().size() ? "?" + src.getQuery() : "") << endl;

	os << "file: " << src.getFile() << ", extension: " << src.getExtension() << endl;
	os << "location: " << src.getLocation() << endl;

	return os << "absolute path: " << src.getAbsolutePath() << endl;
}
