#include "response.hpp"
#include "Connection.hpp"
#include "Server.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "response/code.hpp"
#include "response/status.hpp"
#include <map>
#include <sstream>
#include <string>

using namespace std;


static void buildHeaderAndBody(Connection *connection) {

	string tmp;

	if (tmp.empty()) {
		ostringstream oss;
		oss << "<html><head><title>" + connection->getCode() + " " + connection->getStatus() + "</title>"
			"</head><body><center><h1>" + connection->getCode() + " " + connection->getStatus() + "</h1>"
			"</center><hr><center>webserv</center></body></html>";
		tmp = oss.str();
	}

	connection->setProtocol(response::PROTOCOL);
	connection->setHeaders(response::EMPTY_HEADER);

	connection->addHeader("Content-Type", "text/html");
	connection->addHeader("Content-Lenght", tmp.size());
	connection->addHeader("Connection", "closed");

	connection->setBody(tmp);
	connection->buildResponse();
}

string response::pageBadRequest(Connection * connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageUnauthorized(Connection * connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageForbbiden(Connection * connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageNotFound(Connection * connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageMethodNotAllowed(Connection * connection) {

	connection->setCode(code::METHOD_NOT_ALLOWED);
	connection->setStatus(status::METHOD_NOT_ALLOWED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageLengthRequired(Connection * connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pagePayloadTooLarge(Connection * connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageUnsupportedMediaType(Connection * connection) {

	connection->setCode(code::UNSUPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDI_TYPE);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string response::pageInternalServerError(Connection * connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	buildHeaderAndBody(connection);
	
	logger::warning(connection->getIp() + " timed out");

	return connection->getResponse();
}

string response::pageHttpVersionNotSupported(Connection * connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPPORTED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}
