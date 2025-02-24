#include "File.hpp"
#include "Page.hpp"
#include "Text.hpp"
#include "code.hpp"
#include "Connection.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "process.hpp"
#include "response.hpp"
#include "status.hpp"
#include <iostream>
#include <string>
#include <sys/stat.h>

using namespace std;

static void buildHeaderAndBody(Connection *connection) {

	string header_connection = (*connection)[header::CONNECTION];
	string header_location = (*connection)[header::LOCATION];

	connection->setProtocol(response::PROTOCOL);
	connection->setHeaders(response::EMPTY_HEADER);

	connection->addHeader(header::CONNECTION, header_connection);
	connection->addHeader(header::LOCATION, header_location);

	connection->buildResponse();
	connection->setSend(true);
}

void response::pageOK(Connection *connection) {

	connection->setCode(code::OK);
	connection->setStatus(status::OK);

	process::request(connection);
	if (connection->getCode() != code::OK)
		return;

	logger::info(connection->getHost() + " "
			+ connection->getMethod() + " "
			+ connection->getPath() + " "
			+ connection->getProtocol() + " "
			+ connection->getCode() + " - "
			+ connection->getHeaderByKey(header::USER_AGENT));

	buildHeaderAndBody(connection);
}

void response::pageMovedPermanently(Connection *connection) {

	connection->setCode(code::MOVED_PERMANENTLY);
	connection->setStatus(status::MOVED_PERMANENTLY);
	// TODO: check if getReturnURI is a valid path or a text
	connection->addHeader(header::LOCATION, connection->getPath() + string("/"));
	connection->setFile(new Page(code::MOVED_PERMANENTLY, status::MOVED_PERMANENTLY));
	//connection->setFile(new Text(connection->getLocation().getReturnURI()));
	buildHeaderAndBody(connection);
}

void response::pageBadRequest(Connection *connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	connection->setFile(new Page(code::BAD_REQUEST, status::BAD_REQUEST));
	buildHeaderAndBody(connection);
}

void response::pageUnauthorized(Connection *connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	connection->setFile(new Page(code::UNAUTHORIZED, status::UNAUTHORIZED));
	buildHeaderAndBody(connection);
}

void response::pageForbbiden(Connection *connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	connection->setFile(new Page(code::FORBBIDEN, status::FORBBIDEN));
	buildHeaderAndBody(connection);
}

void response::pageNotFound(Connection *connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	connection->setFile(new Page(code::NOT_FOUND, status::NOT_FOUND));
	buildHeaderAndBody(connection);
}

void response::pageNotAllowed(Connection *connection) {

	connection->setCode(code::NOT_ALLOWED);
	connection->setStatus(status::NOT_ALLOWED);
	connection->setFile(new Page(code::NOT_ALLOWED, status::NOT_ALLOWED));
	buildHeaderAndBody(connection);
}

void response::pageLengthRequired(Connection *connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	connection->setFile(new Page(code::LENGTH_REQUIRED, status::LENGTH_REQUIRED));
	buildHeaderAndBody(connection);
}

void response::pagePayloadTooLarge(Connection *connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	connection->setFile(new Page(code::PAYLOAD_TOO_LARGE, status::PAYLOAD_TOO_LARGE));
	buildHeaderAndBody(connection);
}

void response::pageURITooLong(Connection *connection) {

	connection->setCode(code::URI_TOO_LONG);
	connection->setStatus(status::URI_TOO_LONG);
	connection->setFile(new Page(code::URI_TOO_LONG, status::URI_TOO_LONG));
	buildHeaderAndBody(connection);
}

void response::pageUnsupportedMediaType(Connection *connection) {

	connection->setCode(code::UNSUPPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDIA_TYPE);
	connection->setFile(new Page(code::UNSUPPORTED_MEDIA_TYPE, status::UNSUPPORTED_MEDIA_TYPE));
	buildHeaderAndBody(connection);
}

void response::pageUnprocessableContent(Connection *connection) {

	connection->setCode(code::UNPROCESSABLE_CONTENT);
	connection->setStatus(status::UNPROCESSABLE_CONTENT);
	connection->setFile(new Page(code::UNPROCESSABLE_CONTENT, status::UNPROCESSABLE_CONTENT));
	buildHeaderAndBody(connection);
}

void response::pageInternalServerError(Connection *connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	connection->setFile(new Page(code::INTERNAL_SERVER_ERROR, status::INTERNAL_SERVER_ERROR));
	buildHeaderAndBody(connection);
}

void response::pageNotImplemented(Connection *connection) {

	connection->setCode(code::NOT_IMPLEMENTED);
	connection->setStatus(status::NOT_IMPLEMENTED);
	connection->setFile(new Page(code::NOT_IMPLEMENTED, status::NOT_IMPLEMENTED));
	buildHeaderAndBody(connection);
}

void response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	connection->setFile(new Page(code::GATEWAY_TIMEOUT, status::GATEWAY_TIMEOUT));
	buildHeaderAndBody(connection);
}

void response::pageHttpVersionNotSupported(Connection *connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPORTED);
	connection->setFile(new Page(code::HTTP_VERSION_NOT_SUPPORTED, status::HTTP_VERSION_NOT_SUPPORTED));
	buildHeaderAndBody(connection);
}
