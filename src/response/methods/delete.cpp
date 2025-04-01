#include "parser.hpp"
#include "process.hpp"
#include "URL.hpp"
#include "Connection.hpp"
#include "response.hpp"
#include "code.hpp"
#include "Cgi.hpp"
#include "header.hpp"
#include <cstdio>
#include <stdlib.h>
#include <iostream>

using namespace std;

void process::methodDelete(Connection *connection) {

	URL *uri = connection->getUri();
	connection->addHeader(header::LOCATION, uri->getLocation());

	if (!uri->isFile() && !uri->isDirectory())
		return response::builder(connection, code::NOT_FOUND);

	if (uri->isDirectory() && parser::lastCharacter(uri->getAbsolutePath()) != '/')
		return response::builder(connection, code::CONFLICT);

	if (uri->isCgi())
		return methodDeleteCgi(connection);

	if (uri->isFile())
		return deleteFile(connection, uri);

	if (uri->isDirectory())
		return deleteDirectory(connection, uri);
	
	return response::builder(connection, code::FORBIDDEN);
}

void process::deleteDirectory(Connection *connection, URL *uri) {

	if (uri->isDirectory() && uri->isDeletable()) {

		if (remove(uri->getAbsolutePath().c_str()))
			return response::builder(connection, code::INTERNAL_SERVER_ERROR);

		return response::builder(connection, code::NO_CONTENT);
	}

	return response::builder(connection, code::FORBIDDEN);
}

void process::deleteFile(Connection *connection, URL *uri) {

	if (uri->isFile() && uri->isDeletable()) {

		if (remove(uri->getAbsolutePath().c_str()))
			return response::builder(connection, code::INTERNAL_SERVER_ERROR);

		return response::builder(connection, code::NO_CONTENT);
	}

	return response::builder(connection, code::FORBIDDEN);
}

void process::methodDeleteCgi(Connection *connection) {

	URL *uri = connection->getUri();
	connection->addHeader(header::LOCATION, uri->getLocation());

	if (uri->isDirectory() && !uri->isDeletable())
		return connection->setResource(new Cgi(connection));

	if (uri->isFile())
		return connection->setResource(new Cgi(connection));
	
	return response::builder(connection, code::FORBIDDEN);
}
