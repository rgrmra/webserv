#include "request.hpp"
#include "Connection.hpp"
#include "IStream.hpp"
#include "color.hpp"
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

	if (connection->getStep() == IStream::NONE)
		return parseStartLine(connection, line);

	if (connection->getStep() == IStream::STARTLINE)
		return parseHeaders(connection, line);

	if (connection->getStep() == IStream::HEADERS)
		return parseBody(connection, line);
}

void request::parseStartLine(Connection *connection, string &line) {

	string method, path, protocol;

	if (line.at(line.size() -1) != '\r')
		return response::pageBadRequest(connection);

	if (line.find_first_not_of(" \t\v\r") == string::npos)
		return;

	if (line.find_first_not_of(" \t\v") != 0)
		return response::pageBadRequest(connection);

	istringstream startline(line);
	if (!(startline >> method >> path>> protocol))
		return response::pageBadRequest(connection);

	if (!directive::validateHttpMethod(method))
		return response::pageMethodNotAllowed(connection);

	if (path.size() > 2 * parser::KILOBYTE)
		return response::pageURITooLong(connection);

	if (!directive::validateURI(path))
		return response::pageBadRequest(connection);

	if (protocol != response::PROTOCOL)
		return response::pageHttpVersionNotSupported(connection);

	connection->setMethod(method);
	connection->setPath(path);
	connection->setProtocol(protocol);
	connection->setStep(IStream::STARTLINE);

	return;
}

void request::parseHeaders(Connection *connection, std::string &line) {

	if (line.at(line.size() -1) != '\r')
		return response::pageBadRequest(connection);

	if (line == "\r") {
		connection->setStep(IStream::HEADERS);

		if (!connection->getHeaders().size())
			return response::pageBadRequest(connection);

		if (connection->getMethod() == "POST"
			&& !(*connection == header::CONTENT_LENGTH)
			&& !(*connection == header::TRANSFER_ENCONDING))
			return response::pageBadRequest(connection);

		if (parser::toSizeT((*connection)[header::CONTENT_LENGTH]) == 0)
			return response::pageOK(connection);

		return;
	}

	size_t separator = line.find_first_of(":");
	if (separator == string::npos)
		return response::pageBadRequest(connection);

	string key = line.substr(0, separator);
	string value = line.substr(separator + 1);

	parser::trim(value, " \t\v\r");

	validateHeader(connection, key, value);

	connection->addHeader(key, value);

	return;
}

void request::parseBody(Connection *connection, string &line) {

	// TODO: transfer enconding parser
	
	size_t body_size = line.size();
	size_t content_length = parser::toSizeT((*connection)[header::CONTENT_LENGTH]);

	if (body_size == content_length) {
		connection->setBody(line);
		return response::pageOK(connection);
	}

	if (body_size > content_length)
		return response::pagePayloadTooLarge(connection);
}

void request::validateHeader(Connection *connection, string &key, string &value) {

	if (key == header::CONTENT_LENGTH)
		return validateContentLength(connection, value);

	if (key == header::HOST)
		return validateHost(connection, value);

	if (key == header::TRANSFER_ENCONDING)
		return validateTransferEncoding(connection, value);
}

void request::validateContentLength(Connection *connection, string &value) {

	if (connection->getMethod() != "POST")
		return response::pageBadRequest(connection);

	if (value.find_first_not_of("0123456789") != string::npos)
		return response::pageBadRequest(connection);

	if (*connection == header::TRANSFER_ENCONDING)
		return response::pageBadRequest(connection);
}

void request::validateHost(Connection *connection, string &value) {

	Http *http = Http::getInstance();
	Server server = http->getServerByListen(value);
	if (server.empty())
		server = http->getServerByName(value);

	if (server.empty())
		server = http->getServerByListen(connection->getId());

	if (server.empty())
		return response::pageBadRequest(connection);

	connection->setHost(value);
	connection->setServer(server);
}

void request::validateTransferEncoding(Connection *connection, string &value) {

	if (connection->getMethod() != "POST")
		return response::pageBadRequest(connection);

	if (value != "chuncked")
		return response::pageNotImplemented(connection);

	if (*connection == header::CONTENT_LENGTH)
		return response::pageBadRequest(connection);
}
