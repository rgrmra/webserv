#include "Directory.hpp"
#include "File.hpp"
#include "Connection.hpp"
#include "Location.hpp"
#include "Cgi.hpp"
#include "URL.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "process.hpp"
#include "response.hpp"
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

void process::request(Connection *connection) {

	Location &location = connection->getLocation();
	if (location.empty())
		return response::pageNotFound(connection);

	if (location.getReturnCode().size())
		return response::pageMovedPermanently(connection);

	const string &method = connection->getMethod();
	if (location.getMethod(method) == "")
		return response::pageMethodNotAllowed(connection);

	if (method == "GET")
		return methodGet(connection);

	if (method == "POST")
		return methodPost(connection);

	if (method == "DELETE")
		return methodDelete(connection);
}
	
void process::methodGet(Connection *connection) {

	URL *uri = connection->getUri();
	string path = uri->getAbsolutePath();
	Location &location = connection->getLocation();

	connection->addHeader(header::LOCATION, uri->getLocation());
	
	if (isDirectory(path)) {

		if (path.at(path.size() - 1) == '/') {

			if (location.getAutoIndex())
				return connection->setResource(new Directory(connection));

			return response::pageForbbiden(connection);
		}

		connection->addHeader(header::LOCATION, uri->getLocation() + "/");
		return response::pageMovedPermanently(connection);
	}

	if (location.getFastCgi() != "" && process::isCGI(path))
		return connection->setResource(new Cgi(connection));

	if (process::isFile(path))
		return connection->setResource(new File(connection));

	response::pageNotFound(connection);
}

void process::methodPost(Connection *connection) {

	cout << connection->getBody() << endl;

	response::pageNotFound(connection);
}

void process::methodDelete(Connection *connection) {

	response::pageNotFound(connection);
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

	if (!isFile(path))
		return false;

	if (access(path.c_str(), F_OK) == -1)
		return false;

	if (access(path.c_str(), X_OK) == -1)
		return false;

	return true;
}

string process::checkIndex(const Location &location, std::string &path) {

	const set<string> &indexes = location.getIndexes();

	set<string>::const_iterator it = indexes.begin();
	for (; it != indexes.end(); it++) {

		cout << location.getRoot() + path + *it << endl;

		if (!isFile(location.getRoot() + path + *it))
			continue;

		path.append(*it);
		return *it;
	}
	return "";
}
