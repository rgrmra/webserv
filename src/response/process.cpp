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
	
	if (uri->isDirectory()) {

		if (hasSlashAtEnd(path)) {

			cout << "checkpoint 2" << endl << "path: " << path << endl;
			if (location.getAutoIndex())
				return connection->setResource(new Directory(connection));

			return response::pageForbbiden(connection);
		}

		connection->addHeader(header::LOCATION, uri->getLocation() + "/");
		return response::pageMovedPermanently(connection);
	}

	if (location.getFastCgi() != "" && uri->isCgi())
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
	string path = connection->getLocation().getRoot() + connection->getPath();

	if (!process::isFile(path) && !isDirectory(path)) {

		connection->addHeader(header::LOCATION, uri->getLocation());
		return response::pageNotFound(connection);
	}
	
	if (isDirectory(path) && !hasSlashAtEnd(path)) {

		connection->addHeader(header::LOCATION, uri->getLocation());
		return response::pageConflict(connection);
	}

	// TODO: Alterar abordagem usando a implementacao da URI
	if (isDirectory(path) && isCGI(path) /* verificar se tem nao tem index*/) {

		connection->addHeader(header::LOCATION, uri->getLocation());
		return response::pageForbbiden(connection);
	}

	// TODO: pass to CGI handle file

	// TODO: Implementar logica
}

bool process::hasSlashAtEnd(const std::string &path) {

	return (path.at(path.size() - 1) == '/');
}
