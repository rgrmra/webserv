#include "Connection.hpp"
#include "File.hpp"
#include "AutoIndex.hpp"
#include "Location.hpp"
#include "header.hpp"
#include "../CGI/Cgi.hpp"
#include "parser.hpp"
#include "process.hpp"
#include "response.hpp"
#include <iostream>
#include <list>
#include <string>
#include <sys/stat.h>

using namespace std;

void process::request(Connection *connection) {
    string url = (*connection)[header::REFERER];
    // if (url.size()) #FIXME: this condition is duplicating the path in some cases (e.g. links in html) 
    //  connection->setPath(process::getPathFromReferer(url) + connection->getPath());

    Location location = process::isValidPath(connection);
    connection->setLocation(location);
    if (location.empty())
        return response::pageNotFound(connection);
    if (location.getReturnCode().size())
	{
        return response::pageMovedPermanently(connection);
	}

    string queryString = connection->getPath().find('?') != string::npos ? connection->getPath().substr(connection->getPath().find('?')) : "";
    if (not queryString.empty())
        connection->setQueryString(queryString);

    string path = connection->getPath();
    path = not queryString.empty() ? path.substr(0, path.find('?')) : path;

	string uri = path;
	path = location.getRoot() + connection->getPath();

	
    if (process::isDirectory(path) && path[path.size() - 1] != '/') {

        connection->addHeader(header::LOCATION, connection->getPath() + string("/"));
        return response::pageMovedPermanently(connection);
    }

	if (process::isCGI(path))
	{
		Cgi cgi(*connection);

		cout << "CGI Output:::::: " << cgi.getCgiOutput() << endl;
		cout << "CGI Exit Status:::::: " << cgi.getExitStatus() << endl;

	}

    // connection->setPath(path);
    if (process::isDirectory(path) && not process::checkIndex(location, path))
        return response::pageForbbiden(connection);

    if (process::isCGI(path))
    {
        // cout << "CGI Path:::::: " << path << endl;
        // function to handle CGI
    }
    if (process::isDirectory(path) && location.getAutoIndex())
        return connection->setFile(new AutoIndex(path, uri));
    connection->setFile(new File(path));
}

bool process::isDirectory(const std::string &path) {

	struct stat info;

	if (stat(path.c_str(), &info) == 0)
		return (info.st_mode & S_IFDIR) != 0;

	return false;
}

bool process::isFile(const std::string &path) {

	struct stat info;

	if (stat(path.c_str(), &info) == 0)
		return (info.st_mode & S_IFREG) != 0;

	return false;
}

// TODO: refactor
bool process::isCGI(const std::string &path) {
	size_t pos = path.find_last_of(".");
	if (pos == std::string::npos) {
		return false;
	}
	std::string extension = path.substr(pos);
	return extension == ".php" || extension == ".py" || extension == ".go";
}

string process::getFileExtension(string path) {

	list<string> splited_path = parser::split(path, '/');
	if (splited_path.empty())
		return "";

	size_t pos;
	list<string>::iterator it = splited_path.begin();
	for (; it != splited_path.end(); it++) {

		if (*it == "." || *it == "..")
			continue;

		pos = it->find_first_of(".");
		if (pos == string::npos)
			continue;

		return it->substr(pos + 1);
	}

	return "";
}

Location process::isValidPath(Connection *connection) {

	list<Location> locations;
	locations.push_back(connection->getServer().getLocationByURI("/"));

	string path = connection->getPath();

	size_t pos = path.find_first_of("?");
	if (pos != string::npos)
		path.erase(pos);

	list<string> splited_path = parser::split(path, '/');
	if (splited_path.empty())
		return locations.back();

	string tmp;

	list<string>::iterator it = splited_path.begin();
	for (; it != splited_path.end(); it ++) {

		cout << *it << endl;

		if (*it == ".")
			continue;

		if (*it == "..") {
			if (locations.size() > 1)
				locations.pop_back();

			continue;
		}

		if (it->find_first_of(".?") != string::npos) {

			return locations.back();
		}

		tmp += "/" + *it;

		Location location = connection->getServer().getLocationByURI(tmp);
		if (!location.empty())
			locations.push_back(location);
	}
	return locations.back();
}

bool process::checkIndex(const Location &location, std::string &path) {
	const std::set<std::string> &indexes = location.getIndexes();
	const string &bar = path.find_last_of('/') == path.size() - 1 ? "" : "/";

	typedef std::set<std::string>::const_iterator set_iterator;
	for (set_iterator it = indexes.begin(); it != indexes.end(); ++it) {
        std::string indexPath = path + bar + *it;
        if (isFile(indexPath)) {
            path = indexPath;
            return true;
        }
    }
    return location.getAutoIndex();
}

string process::getPathFromReferer(string url) {

	size_t pos = url.find_first_of("://");
	if (pos != string::npos)
		url.erase(0, pos + 3);

	pos = url.find_first_of("/");
	if (pos != string::npos)
		url.erase(0, pos);

	pos = url.find_first_of("?");
	if (pos != string::npos)
		url.erase(pos);

	return url;
}
