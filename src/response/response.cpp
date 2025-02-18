#include "File.hpp"
#include "Page.hpp"
#include "AutoIndex.hpp"
#include "code.hpp"
#include "Connection.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "response.hpp"
#include "status.hpp"
#include <iostream>
#include <string>
#include <sys/stat.h>

using namespace std;

bool response::isDirectory(const std::string &path) {

	struct stat info;

	if (stat(path.c_str(), &info) == 0)
		return (info.st_mode & S_IFDIR) != 0;

	return false;
}

bool response::isFile(const std::string &path) {

	struct stat info;

	if (stat(path.c_str(), &info) == 0)
		return (info.st_mode & S_IFREG) != 0;

	return false;
}

// TODO: refactor
bool response::isCGI(const std::string &path) {
	size_t pos = path.find_last_of(".");
	if (pos == std::string::npos) {
		return false;
	}
	std::string extension = path.substr(pos);
	return extension == ".php" || extension == ".py" || extension == ".go";
}

string response::getFileExtension(string path) {

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

Location response::isPathValid(Connection *connection) {
	string path = connection->getPath();
	if (path.find("..") != string::npos || path.find('/') == string::npos) {
		return Location();
	}

	while (!path.empty()) {
		Location temp = connection->getServer().getLocationByURI(path);
		if (not temp.getURI().empty())
		{
			return temp;
		}

		size_t lastSlash = path.find_last_of('/');
		if (lastSlash == 0)
			path = "/";
		else if (lastSlash != string::npos)
			path = path.substr(0, lastSlash);
		else
			path.clear();
	}
	return Location();
}

Location isValidPath(Connection *connection) {

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

	cout << "location: " << tmp << endl;

	return locations.back();
}

static void buildHeaderAndBody(Connection *connection) {

	connection->setFile(new Page(connection->getCode(), connection->getStatus()));

	string header_connection = (*connection)[header::CONNECTION];

	connection->setProtocol(response::PROTOCOL);
	connection->setHeaders(response::EMPTY_HEADER);

	connection->addHeader(header::CONNECTION, header_connection);
	connection->buildResponse();
	connection->setSend(true);
}

bool response::checkIndex(const Location &location, Connection *connection) {
	const std::set<std::string> &indexes = location.getIndexes();
	const string &path = connection->getPath();
	const string &bar = path.find_last_of('/') == path.size() - 1 ? "" : "/";

	typedef std::set<std::string>::const_iterator set_iterator;
	for (set_iterator it = indexes.begin(); it != indexes.end(); ++it) {
        std::string indexPath = path + bar + *it;
        if (isFile(indexPath)) {
            connection->setPath(indexPath);
            return true;
        }
    }
    return false;
}

string getPathFromURL(string url) {

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

void response::pageOK(Connection *connection) {

	connection->setCode(code::OK);
	connection->setStatus(status::OK);

	cout << "URL: " << (*connection)[header::REFERER] << endl;
	cout << "URI Path: " << connection->getPath() << endl;
	string url = (*connection)[header::REFERER];
	// if (url.size()) // BUG: essa condição está duplicando o path
	// 	connection->setPath(getPathFromURL(url) + connection->getPath());

	Location location = isValidPath(connection);
	cout << location << endl;
	if (location.empty())
		return response::pageNotFound(connection);

	string queryString = connection->getPath().find('?') != string::npos ? connection->getPath().substr(connection->getPath().find('?')) : "";
	if (not queryString.empty())
		connection->setQueryString(queryString);

	string path = connection->getPath();
	string uri;
	
	path = not queryString.empty() ? path.substr(0, path.find('?')) : path;
	uri = path;
	path = location.getRoot() + connection->getPath();
	logger::info("Path: " + path);
	connection->setPath(path);
	if (isDirectory(path) && not checkIndex(location, connection) 
		&& not location.getAutoIndex())
		return response::pageForbbiden(connection);

	cout << path << " " << getFileExtension(connection->getPath()) << endl;
	cout << "URI: " << uri << endl;
	if (isCGI(path))
	{
		// cout << "CGI Path:::::: " << path << endl;
		// function to handle CGI
	}

	logger::info(connection->getHost() + " "
			+ connection->getMethod() + " "
			+ connection->getPath() + " "
			+ connection->getProtocol() + " "
			+ connection->getCode() + " - "
			+ connection->getHeaderByKey(header::USER_AGENT));

	//buildHeaderAndBody(connection);
	string header_connection = (*connection)[header::CONNECTION];
	connection->setHeaders(response::EMPTY_HEADER);
	connection->addHeader(header::CONNECTION, header_connection);

	if (isDirectory(connection->getPath()))
		connection->setFile(new AutoIndex(connection->getPath(), uri));
	else
		connection->setFile(new File(connection->getPath()));
	connection->buildResponse();
	connection->setSend(true);
}

void response::pageBadRequest(Connection *connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	buildHeaderAndBody(connection);
}

void response::pageUnauthorized(Connection *connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	buildHeaderAndBody(connection);
}

void response::pageForbbiden(Connection *connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	buildHeaderAndBody(connection);
}

void response::pageNotFound(Connection *connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	buildHeaderAndBody(connection);
}

void response::pageNotAllowed(Connection *connection) {

	connection->setCode(code::NOT_ALLOWED);
	connection->setStatus(status::NOT_ALLOWED);
	buildHeaderAndBody(connection);
}

void response::pageLengthRequired(Connection *connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	buildHeaderAndBody(connection);
}

void response::pagePayloadTooLarge(Connection *connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	buildHeaderAndBody(connection);
}

void response::pageURITooLong(Connection *connection) {

	connection->setCode(code::URI_TOO_LONG);
	connection->setStatus(status::URI_TOO_LONG);
	buildHeaderAndBody(connection);
}

void response::pageUnsupportedMediaType(Connection *connection) {

	connection->setCode(code::UNSUPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDIA_TYPE);
	buildHeaderAndBody(connection);
}

void response::pageUnprocessableContent(Connection *connection) {

	connection->setCode(code::UNPROCESSABLE_CONTENT);
	connection->setStatus(status::UNPROCESSABLE_CONTENT);
	buildHeaderAndBody(connection);
}

void response::pageInternalServerError(Connection *connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	buildHeaderAndBody(connection);
}

void response::pageNotImplemented(Connection *connection) {

	connection->setCode(code::NOT_IMPLEMENTED);
	connection->setStatus(status::NOT_IMPLEMENTED);
	buildHeaderAndBody(connection);
}

void response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	buildHeaderAndBody(connection);

	logger::warning(connection->getIp() + " timed out");
}

void response::pageHttpVersionNotSupported(Connection *connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPPORTED);

	buildHeaderAndBody(connection);
}
