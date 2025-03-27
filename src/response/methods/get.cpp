#include "parser.hpp"
#include "process.hpp"
#include "URL.hpp"
#include "Connection.hpp"
#include "header.hpp"
#include "response.hpp"
#include "code.hpp"
#include "Directory.hpp"
#include "File.hpp"
#include "Cgi.hpp"

void process::methodGet(Connection *connection) {

	URL *uri = connection->getUri();
	Location &location = connection->getLocation();

	connection->addHeader(header::LOCATION, uri->getLocation());
	
	if (uri->isDirectory()) {

		if (parser::lastCharacter(uri->getAbsolutePath()) == '/') {

			if (location.getAutoIndex())
				return connection->setResource(new Directory(connection));

			return response::builder(connection, code::FORBBIDEN);
		}

		connection->addHeader(header::LOCATION, uri->getLocation() + '/');
		return response::builder(connection, code::MOVED_PERMANENTLY);
	}

	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));

	if (uri->isFile())
		return connection->setResource(new File(connection));

	response::pageNotFound(connection);
}
