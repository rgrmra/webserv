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

	if (!connection->getStartLineParsed())
		return parseStartLine(connection, line);

	if (!connection->getHeadersParsed())
		return parseHeaders(connection, line);

	if (connection->getHeadersParsed()) {

		size_t body_size = connection->getBuffer().size();
		size_t content_length = parser::toSizeT((*connection)[header::CONTENT_LENGTH]);

		if (body_size == content_length) {
			connection->setBody(connection->getBuffer());
			connection->setSend(true);
		} else if (body_size > content_length)
			return response::pagePayloadTooLarge(connection);
	}
}

void request::parseStartLine(Connection *connection, string line) {

	string method, uri, protocol;

	if (line.at(line.size() -1) != '\r')
		return response::pageBadRequest(connection);

	if (line.find_first_not_of(" \t\v\r") == string::npos)
		return;

	if (line.find_first_not_of(" \t\v") != 0)
		return response::pageBadRequest(connection);

	istringstream startline(line);
	if (!(startline >> method >> uri >> protocol))
		return response::pageBadRequest(connection);

	if (!directive::validateHttpMethod(method))
		return response::pageNotAllowed(connection);

	if (uri.size() > parser::KILOBYTE * 2)
		return response::pageURITooLong(connection);

	if (protocol != response::PROTOCOL)
		return response::pageHttpVersionNotSupported(connection);

	connection->setMethod(method);
	connection->setUri(uri);
	connection->setProtocol(protocol);
	splitPathQuery(connection);
	connection->setStartLineParsed(true);

	return;
}

void request::splitPathQuery(Connection *connection) {

	size_t pos = connection->getUri().find("?");
	if (pos == string::npos)
		return;

	connection->setPath(connection->getUri().substr(0, pos));
	connection->setQueryString(connection->getUri().substr(pos + 1));

	return;
}

void request::parseHeaders(Connection *connection, std::string line) {

	if (line.at(line.size() -1) != '\r')
		return response::pageBadRequest(connection);

	if (line == "\r") {
		connection->setHeadersParsed(true);

		if (!connection->getHeaders().size())
			return response::pageBadRequest(connection);

		if (!connection->hasContentLenght() && !connection->hasTransferEnconding() && (connection->getMethod() == "POST"))
			return response::pageBadRequest(connection);

		if (connection->hasTransferEnconding())
			if ((*connection)[header::TRANSFER_ENCONDING] != "chuncked")
				return response::pageNotImplemented(connection);

		if (parser::toSizeT((*connection)[header::CONTENT_LENGTH]) == 0)
			return response::pageOK(connection);

		return;
	}

	size_t separator = line.find(":");
	if (request::validateHeaders(connection, separator))
		return response::pageBadRequest(connection);

	string key = line.substr(0, separator);
	string value = line.substr(separator + 1);

	parser::trim(value, " \t\v\r");

	connection->addHeader(key, value);

	return;
}

bool request::validateHeaders(Connection *connection, size_t separator) {

	if (separator == string::npos)
		return true;

	if (connection->hasContentLenght() && connection->hasTransferEnconding())
		return true;

	return false;
}
