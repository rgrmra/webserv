#include "code.hpp"
#include "Connection.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "response.hpp"
#include "status.hpp"
#include <sstream>
#include <string>

using namespace std;

static void buildHeaderAndBody(Connection *connection) {

	string tmp;

	if (tmp.empty()) {

		tmp = connection->getCode() + " " + connection->getStatus();

		ostringstream oss;
		oss << "<html>\n"
			"<head><title>" + tmp + "</title></head>\n"
			"<body>\n"
			"<center><h1>" + tmp + "</h1></center>\n"
			"<hr><center>webserv</center>\n"
			"</body>\n"
			"</html>\n";
		tmp = oss.str();
	}

	connection->setProtocol(response::PROTOCOL);
	connection->setHeaders(response::EMPTY_HEADER);

	connection->addHeader(header::CONTENT_TYPE, "text/html");
	connection->addHeader(header::CONTENT_LENGTH, tmp.size());
	connection->addHeader(header::CONNECTION, "close");

	connection->setBody(tmp);
	connection->buildResponse();
}

void response::pageBadRequest(Connection *connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	buildHeaderAndBody(connection);
}

void response::pageUnauthorized(Connection *connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	buildHeaderAndBody(connection);
}

void response::pageForbbiden(Connection *connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	buildHeaderAndBody(connection);
}

void response::pageNotFound(Connection *connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	buildHeaderAndBody(connection);
}

void response::pageMethodNotAllowed(Connection *connection) {

	connection->setCode(code::METHOD_NOT_ALLOWED);
	connection->setStatus(status::METHOD_NOT_ALLOWED);
	buildHeaderAndBody(connection);
}

void response::pageLengthRequired(Connection *connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	buildHeaderAndBody(connection);
}

void response::pagePayloadTooLarge(Connection *connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	buildHeaderAndBody(connection);
}

void response::pageUnsupportedMediaType(Connection *connection) {

	connection->setCode(code::UNSUPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDIA_TYPE);
	buildHeaderAndBody(connection);
}

void response::pageUnprocessableContent(Connection *connection) {

	connection->setCode(code::UNPROCESSABLE_CONTENT);
	connection->setStatus(status::UNPROCESSABLE_CONTENT);
	buildHeaderAndBody(connection);
}

void response::pageInternalServerError(Connection *connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	buildHeaderAndBody(connection);
}

void response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	buildHeaderAndBody(connection);
	
	logger::warning(connection->getIp() + " timed out");
}

void response::pageHttpVersionNotSupported(Connection * connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPPORTED);
	buildHeaderAndBody(connection);
}
