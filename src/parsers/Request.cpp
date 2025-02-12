#include "Request.hpp"
#include "Connection.hpp"
#include "directive.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "response.hpp"
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>

using namespace std;

void request::parseRequest(Connection *connection, string line) {

	string method, path, protocol;

	if (!connection->getStartLineParsed()) {
		
		// IGNORE EMPTY FIRST LINES?
		if (line.find_first_not_of(" \t\v\r") == string::npos)
			return;

		if (line.find_first_not_of(" \t\v") != 0)
			return response::pageBadRequest(connection);

		istringstream startline(line);
		if (!(startline >> method >> path >> protocol))
			return response::pageBadRequest(connection);

		if (!directive::validateHttpMethod(method))
			return response::pageBadRequest(connection);

		if (protocol != response::PROTOCOL)
			return response::pageHttpVersionNotSupported(connection);

		connection->setMethod(method);
		connection->setPath(path);
		connection->setProtocol(protocol);
		connection->setStartLineParsed(true);

		return;
	}

	if (!connection->getHeadersParsed()) {

		if (line == "\r") {
			connection->setHeadersParsed(true);
			if (!parser::toSizeT(connection->getHeaderByKey(header::CONTENT_LENGTH)))
				connection->setSend(true);

			return;
		}

		size_t separator = line.find(":");
		if (separator == string::npos)
			return response::pageBadRequest(connection);

		string key = line.substr(0, separator);
		string value = line.substr(separator + 1);

		parser::trim(value, " \t\v\r");

		connection->addHeader(key, value);

		return;
	}

	if (connection->getHeadersParsed()) {

		size_t body_size = connection->getBuffer().size();
		size_t content_length = parser::toSizeT(connection->getHeaderByKey(header::CONTENT_LENGTH));

		if (body_size == content_length) {
			connection->setBody(connection->getBuffer());
			connection->setSend(true);
		} else if (body_size > content_length)
			return response::pageBadRequest(connection);
	}
}
