#include <Connection.hpp>
#include <IStream.hpp>
#include <Page.hpp>
#include "File.hpp"
#include "URL.hpp"
#include "code.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "process.hpp"
#include "response.hpp"
#include "standard.hpp"
#include "status.hpp"
#include <map>
#include <string>

using namespace std;

static void buildHeaderAndBody(Connection *connection) {

	connection->setStep(IStream::BODY);

	string tmp = connection->getHost() + " "
				+ connection->getMethod() + " "
				+ connection->getTarget() + " "
				+ connection->getProtocol() + " "
				+ connection->getCode() + " - "
				+ connection->getHeaderByKey(header::USER_AGENT);

	if (connection->getCode() == code::OK)
		logger::info(tmp);
	else
		logger::warning(tmp);

	string header_connection = (*connection)[header::CONNECTION];
	string header_location = (*connection)[header::LOCATION];

	connection->setProtocol(standard::PROTOCOL);
	connection->setHeaders(standard::EMPTY_HEADER);

	connection->addHeader(header::CONNECTION, header_connection);
	connection->addHeader(header::LOCATION, header_location);
	connection->setTime();
	connection->buildResponse();
}

static bool checkErrorPages(Connection *connection) {

	string page = connection->getLocation().getErrorPageByCode(connection->getCode());
	if (page.empty())
		return false;

	string path = connection->getTarget();

	connection->setTarget(page);
	URL *uri = new URL(connection);

	connection->setTarget(path);

	if (!uri->isFile()) {
		delete uri;
		return false;
	}
	
	URL *old_uri = connection->getUri();
	connection->setUri(uri);

	connection->setResource(new File(connection));
	connection->setUri(old_uri);

	delete uri;

	return true;
}

void response::builder(Connection *connection, string code) {

	if (responses.empty()) {
		responses[code::OK] = status::OK;
		responses[code::CREATED] = status::CREATED;
		responses[code::ACCEPTED] = status::ACCEPTED;
		responses[code::NO_CONTENT] = status::NO_CONTENT;
		responses[code::MOVED_PERMANENTLY] = status::MOVED_PERMANENTLY;
		responses[code::BAD_REQUEST] = status::BAD_REQUEST;
		responses[code::UNAUTHORIZED] = status::UNAUTHORIZED;
		responses[code::FORBBIDEN] = status::FORBBIDEN;
		responses[code::NOT_FOUND] = status::NOT_FOUND;
		responses[code::NOT_ALLOWED] = status::NOT_ALLOWED;
		responses[code::CONFLICT] = status::CONFLICT;
		responses[code::LENGTH_REQUIRED] = status::LENGTH_REQUIRED;
		responses[code::PAYLOAD_TOO_LARGE] = status::PAYLOAD_TOO_LARGE;
		responses[code::URI_TOO_LONG] = status::URI_TOO_LONG;
		responses[code::UNSUPPORTED_MEDIA_TYPE] = status::UNSUPPORTED_MEDIA_TYPE;
		responses[code::UNPROCESSABLE_CONTENT] = status::UNPROCESSABLE_CONTENT;
		responses[code::INTERNAL_SERVER_ERROR] = status::INTERNAL_SERVER_ERROR;
		responses[code::NOT_IMPLEMENTED] = status::NOT_IMPLEMENTED;
		responses[code::BAD_GATEWAY] = status::BAD_GATEWAY;
		responses[code::GATEWAY_TIMEOUT] = status::GATEWAY_TIMEOUT;
		responses[code::HTTP_VERSION_NOT_SUPPORTED] = status::HTTP_VERSION_NOT_SUPPORTED;
	}

	map<string, string>::iterator it = responses.find(code);
	if (it == responses.end())
		return builder(connection, code::INTERNAL_SERVER_ERROR);

	connection->setCode(code);
	connection->setStatus(it->second);

	if (code == code::OK)
		process::request(connection);
	else if (!checkErrorPages(connection))
		connection->setResource(new Page(connection));

	if (code == connection->getCode())
		buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::OK)
void response::pageOK(Connection *connection) {

	connection->setCode(code::OK);
	connection->setStatus(status::OK);
	process::request(connection);
	if (connection->getCode() == code::OK)
		buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::NO_CONTENT)
void response::pageNoContent(Connection *connection) {

	connection->setCode(code::NO_CONTENT);
	connection->setStatus(status::NO_CONTENT);
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::MOVED_PERMANENTLY)
void response::pageMovedPermanently(Connection *connection) {

	connection->setCode(code::MOVED_PERMANENTLY);
	connection->setStatus(status::MOVED_PERMANENTLY);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::BAD_REQUEST)
void response::pageBadRequest(Connection *connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::UNAUTHORIZED)
void response::pageUnauthorized(Connection *connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::FORBBIDEN)
void response::pageForbbiden(Connection *connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::NOT_FOUND)
void response::pageNotFound(Connection *connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::NOT_ALLOWED)
void response::pageMethodNotAllowed(Connection *connection) {

	connection->setCode(code::NOT_ALLOWED);
	connection->setStatus(status::NOT_ALLOWED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::LENGTH_REQUIRED)
void response::pageLengthRequired(Connection *connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::PAYLOAD_TOO_LARGE)
void response::pagePayloadTooLarge(Connection *connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::URI_TOO_LONG)
void response::pageURITooLong(Connection *connection) {

	connection->setCode(code::URI_TOO_LONG);
	connection->setStatus(status::URI_TOO_LONG);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::UNSUPPORTED_MEDIA_TYPE)
void response::pageUnsupportedMediaType(Connection *connection) {

	connection->setCode(code::UNSUPPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDIA_TYPE);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::UNPROCESSABLE_CONTENT)
void response::pageUnprocessableContent(Connection *connection) {

	connection->setCode(code::UNPROCESSABLE_CONTENT);
	connection->setStatus(status::UNPROCESSABLE_CONTENT);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::INTERNAL_SERVER_ERROR)
void response::pageInternalServerError(Connection *connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::NOT_IMPLEMENTED)
void response::pageNotImplemented(Connection *connection) {

	connection->setCode(code::NOT_IMPLEMENTED);
	connection->setStatus(status::NOT_IMPLEMENTED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::BAD_GATEWAY)
void response::pageBadGateway(Connection *connection) {

	connection->setCode(code::BAD_GATEWAY);
	connection->setStatus(status::BAD_GATEWAY);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::GATEWAY_TIMEOUT)
void response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::HTTP_VERSION_NOT_SUPPORTED)
void response::pageHttpVersionNotSupported(Connection *connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPORTED);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}

// deprecated: use response::builder(connection, code::CONFLICT)
void response::pageConflict(Connection *connection) {

	connection->setCode(code::CONFLICT);
	connection->setStatus(status::CONFLICT);
	connection->setResource(new Page(connection));
	buildHeaderAndBody(connection);
}
