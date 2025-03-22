#include "request.hpp"
#include "Connection.hpp"
#include "IStream.hpp"
#include "directive.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "response.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>

using namespace std;

void request::parseRequest(Connection *connection, string &line) {

	if (connection->getStep() == IStream::NONE)
		return parseStartLine(connection, line);

	if (connection->getStep() == IStream::STARTLINE)
		return parseHeaders(connection, line);

	if (connection->getStep() == IStream::HEADERS)
		return parseBody(connection, line);
}

void request::parseStartLine(Connection *connection, string &line) {

	string method, target, protocol;

	if (line.at(line.size() - 1) != '\r')
		return response::pageBadRequest(connection);

	if (line.find_first_not_of(" \t\v\r") == string::npos)
		return;

	if (line.find_first_not_of(" \t\v") != 0)
		return response::pageBadRequest(connection);

	istringstream startline(line);
	if (!(startline >> method >> target >> protocol))
		return response::pageBadRequest(connection);

	if (!directive::validateHttpMethod(method))
		return response::pageMethodNotAllowed(connection);

	if (target.size() > 2 * parser::KILOBYTE)
		return response::pageURITooLong(connection);

	if (!directive::isValidRequestTarget(target))
		return response::pageBadRequest(connection);

	if (protocol != response::PROTOCOL)
		return response::pageHttpVersionNotSupported(connection);

	connection->setMethod(method);
	connection->setTarget(target);
	connection->setProtocol(protocol);
	connection->setStep(IStream::STARTLINE);

	return;
}

void request::parseHeaders(Connection *connection, std::string &line) {

	if (line.at(line.size() - 1) != '\r')
		return response::pageBadRequest(connection);

	if (line == "\r") {
		connection->setStep(IStream::HEADERS);

		connection->setUri(new URL(connection));

		if (!connection->getHeadersSize())
			return response::pageBadRequest(connection);

		if (connection->getMethod() == "POST"
			&& !(*connection == header::CONTENT_LENGTH)
			&& !(*connection == header::TRANSFER_ENCONDING))
			return response::pageBadRequest(connection);

		if (*connection == header::TRANSFER_ENCONDING)
			return;

		if (parser::toSizeT((*connection)[header::CONTENT_LENGTH]) > 0)
			return;

		return response::pageOK(connection);
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

	if (*connection == header::TRANSFER_ENCONDING)
		return parseTransferEncoding(connection, line);

	size_t body_size = line.size() ;
	size_t content_length = parser::toSizeT((*connection)[header::CONTENT_LENGTH]);

	if (body_size < content_length)
		return;

	if (body_size > content_length)
		return response::pagePayloadTooLarge(connection);

	connection->addBody(line);
	line.clear();

	if (connection->getBody().size() > connection->getLocation().getMaxBodySize())
		return response::pagePayloadTooLarge(connection);

	response::pageOK(connection);
}

void request::checkTransferEncodingEnd(Connection *connection, string &buffer) {

	if (buffer.size() < 4)
		return;

	if (!parser::compare("0\r\n\r\n", buffer))
		return response::pageBadRequest(connection);
	buffer.clear();

	connection->setStep(IStream::BODY);
	return response::pageOK(connection);
}

void request::parseTransferEncoding(Connection *connection, string &buffer) {

	if (buffer.empty())
		return;

	string chunk_size_value = buffer.substr(0, buffer.find("\r\n"));
	size_t chunk_size_length = chunk_size_value.size() + 2;

	if (chunk_size_value.find_first_not_of("0123456789ABCDEF") != string::npos)
		return response::pageBadRequest(connection);

	if (chunk_size_value == "0")
		return checkTransferEncodingEnd(connection, buffer);

	size_t chunk_line_length;
	convertToHex(connection, chunk_size_value, chunk_line_length);
	if (connection->getCode() != "")
		return;

	string chunk_line_value = buffer.substr(chunk_size_length, chunk_line_length);

	if (chunk_line_value.size() != chunk_line_length)
		return;
	buffer.erase(0, chunk_size_length + chunk_line_length);

	connection->addBody(chunk_line_value);
	if (connection->getBody().size() > connection->getLocation().getMaxBodySize())
		return response::pagePayloadTooLarge(connection);

	if (!parser::compare("\r\n", buffer))
		return response::pageBadRequest(connection);
	buffer.erase(0, 2);

	return parseTransferEncoding(connection, buffer);
}

void request::convertToHex(Connection *connection, string &line, size_t &chunck_size) {

	if (line.find_first_not_of("0123456789ABCDEF") != string::npos)
		return response::pageBadRequest(connection);

	char *rest;
	chunck_size = strtoul(line.c_str(), &rest, 16);
	if (rest[0] != '\0')
		return response::pageBadRequest(connection);
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

	if (value != "chunked")
		return response::pageNotImplemented(connection);

	if (*connection == header::CONTENT_LENGTH)
		return response::pageBadRequest(connection);
}
