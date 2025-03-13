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

		if (hasSlashAtEnd(path)) {

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

	string path = connection->getLocation().getRoot() + connection->getPath();

	if (!process::isFile(path) && !isDirectory(path))
		return response::pageNotFound(connection);
	
	if (isDirectory(path) && !hasSlashAtEnd(path))
		return response::pageConflict(connection);

	// FIXME: Arrumar check index ou criar uma nova para checar se no diretorio
	// tem a presenca de index
	string indexFile = process::checkIndex(connection->getLocation(), path);
	cout << indexFile.empty() << std::endl;
	if (isDirectory(path) && isCGI(path) && indexFile.empty())
		return response::pageForbbiden(connection);

	// TODO: pass to CGI handle file

	// TODO: Implementar logica
}

bool process::hasSlashAtEnd(const std::string &path) {

	return (path.at(path.size() - 1) == '/');
}

bool process::isDirectory(const std::string &path) {

	struct stat info;

	if (!stat(path.c_str(), &info))
		return S_ISDIR(info.st_mode);

	return false;
}

bool process::isFile(const std::string &path) {

	struct stat info;

	if (!stat(path.c_str(), &info))
		return S_ISREG(info.st_mode);

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

	// BUG: Essa funcao deve considerar o path que foi passado na requisicao
	// e caso ele exista deve chegar se tem algum arquivo de index
	// dentro do diretorio passado.
	// Dessa maneira, ela esta retornando appendando o index mesmo 
	// quando eh passado apenas um diretoro.
	const set<string> &indexes = location.getIndexes();

	set<string>::const_iterator it = indexes.begin();
	for (; it != indexes.end(); it++) {

		cout << location.getRoot() + "/" + *it << endl;

		if (!isFile(location.getRoot() + "/" + *it))
			continue;

		path.append(*it);
		return *it;
	}
	return "";
}
