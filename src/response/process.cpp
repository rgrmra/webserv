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
#include "code.hpp"
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void process::request(Connection *connection) {

	Location &location = connection->getLocation();
	if (location.empty())
		return response::builder(connection, code::NOT_FOUND);

	if (location.getReturnCode().size())
		return response::builder(connection, code::MOVED_PERMANENTLY);

	const string &method = connection->getMethod();
	if (location.getMethod(method) == "")
		return response::builder(connection, code::NOT_ALLOWED);

	if (method == "GET")
		return methodGet(connection);

	if (method == "POST")
		return methodPost(connection);

	if (method == "DELETE")
		return methodDelete(connection);
}
	
void process::methodGet(Connection *connection) {

	URL *uri = connection->getUri();
	Location &location = connection->getLocation();

	connection->addHeader(header::LOCATION, uri->getLocation());
	
	if (uri->isDirectory()) {

		if (hasSlashAtEnd(uri->getAbsolutePath() + "/")) {

			if (location.getAutoIndex())
				return connection->setResource(new Directory(connection));

			return response::builder(connection, code::FORBBIDEN);
		}

		connection->addHeader(header::LOCATION, uri->getLocation() + "/");
		return response::builder(connection, code::MOVED_PERMANENTLY);
	}

	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));

	if (uri->isFile())
		return connection->setResource(new File(connection));

	response::pageNotFound(connection);
}

void process::methodPost(Connection *connection) {

	cout << connection->getBody() << endl;

	response::pageNotFound(connection);
}

void process::methodDelete(Connection *connection) {

	URL *uri = connection->getUri();
	connection->addHeader(header::LOCATION, uri->getLocation());

	if (!uri->isFile() && !uri->isDirectory())
		return response::builder(connection, code::NOT_FOUND);

	if (uri->isDirectory() && !hasSlashAtEnd(uri->getAbsolutePath()))
		return response::builder(connection, code::CONFLICT);

	if (uri->isCgi())
		return methodDeleteCgi(connection);

	if (uri->isFile())
		return deleteFile(connection, uri);

	if (uri->isDirectory())
		return deleteDirectory(connection, uri);
	
	return response::builder(connection, code::FORBBIDEN);
}

void process::deleteDirectory(Connection *connection, URL *uri) {

	if (uri->isDirectory() && uri->isDeletable()) {

		std::string command = "rmdir " + uri->getAbsolutePath();
		
		if (system(command.c_str()))
			return response::builder(connection, code::INTERNAL_SERVER_ERROR);
		

		return response::builder(connection, code::NO_CONTENT);
	}

	return response::builder(connection, code::FORBBIDEN);
}

void process::deleteFile(Connection *connection, URL *uri) {

	if (uri->isFile() && uri->isDeletable()) {

		std::string command = "rm " + uri->getAbsolutePath();

		if (system(command.c_str()))
			return response::builder(connection, code::INTERNAL_SERVER_ERROR);

		return response::builder(connection, code::NO_CONTENT);
	}

	return response::builder(connection, code::FORBBIDEN);
}

void process::methodDeleteCgi(Connection *connection) {

	URL *uri = connection->getUri();
	connection->addHeader(header::LOCATION, uri->getLocation());

	if (uri->isDirectory() && !uri->isDeletable())
		return connection->setResource(new Cgi(connection));

	if (uri->isFile())
		return connection->setResource(new Cgi(connection));
	
	return response::builder(connection, code::FORBBIDEN);
}

bool process::hasSlashAtEnd(const std::string &path) {

	return (path.at(path.size() - 1) == '/');
}
