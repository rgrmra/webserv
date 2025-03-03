#include "IStream.hpp"
#include "Page.hpp"
#include "code.hpp"
#include "Connection.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "process.hpp"
#include "response.hpp"
#include "status.hpp"
#include <iostream>
#include <string>
#include <sys/stat.h>

using namespace std;

static void buildHeaderAndBody(Connection *connection) {
	connection->setStep(IStream::BODY);

	if (connection->getCode() == code::OK) {
		logger::info(connection->getHost() + " "
				+ connection->getMethod() + " "
				+ connection->getPath() + " "
				+ connection->getProtocol() + " "
				+ connection->getCode() + " - "
				+ connection->getHeaderByKey(header::USER_AGENT));
	} else {
		logger::warning(connection->getId() + " "
			+ connection->getCode() + " "
			+ connection->getStatus());
	}

	string header_connection = (*connection)[header::CONNECTION];
	string header_location = (*connection)[header::LOCATION];

	connection->setProtocol(response::PROTOCOL);
	connection->setHeaders(response::EMPTY_HEADER);

	connection->addHeader(header::CONNECTION, header_connection);
	connection->addHeader(header::LOCATION, header_location);
	connection->buildResponse();
}

void response::pageOK(Connection *connection) {

	connection->setCode(code::OK);
	connection->setStatus(status::OK);
	process::request(connection);
	if (connection->getCode() == code::OK)
		buildHeaderAndBody(connection);
}

void response::pageMovedPermanently(Connection *connection) {

	connection->setCode(code::MOVED_PERMANENTLY);
	connection->setStatus(status::MOVED_PERMANENTLY);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageBadRequest(Connection *connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageUnauthorized(Connection *connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageForbbiden(Connection *connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageNotFound(Connection *connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageMethodNotAllowed(Connection *connection) {

	connection->setCode(code::NOT_ALLOWED);
	connection->setStatus(status::NOT_ALLOWED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageLengthRequired(Connection *connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pagePayloadTooLarge(Connection *connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageURITooLong(Connection *connection) {

	connection->setCode(code::URI_TOO_LONG);
	connection->setStatus(status::URI_TOO_LONG);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageUnsupportedMediaType(Connection *connection) {

	connection->setCode(code::UNSUPPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDIA_TYPE);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageUnprocessableContent(Connection *connection) {

	connection->setCode(code::UNPROCESSABLE_CONTENT);
	connection->setStatus(status::UNPROCESSABLE_CONTENT);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageInternalServerError(Connection *connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageNotImplemented(Connection *connection) {

	connection->setCode(code::NOT_IMPLEMENTED);
	connection->setStatus(status::NOT_IMPLEMENTED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageBadGateway(Connection *connection) {

	connection->setCode(code::BAD_GATEWAY);
	connection->setStatus(status::BAD_GATEWAY);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

void response::pageHttpVersionNotSupported(Connection *connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPORTED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}
