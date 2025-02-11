#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

class Connection;

namespace response {

	static const std::string PROTOCOL = "HTTP/1.1";
	static const std::map<std::string, std::string> EMPTY_HEADER;

	void pageBadRequest(Connection *connection);
	void pageUnauthorized(Connection *connection);
	void pageForbbiden(Connection *connection);
	void pageNotFound(Connection *connection);
	void pageMethodNotAllowed(Connection *connection);
	void pageLengthRequired(Connection *connection);
	void pagePayloadTooLarge(Connection *connection);
	void pageUnsupportedMediaType(Connection *connection);
	void pageUnprocessableContent(Connection *connection);
	void pageInternalServerError(Connection *connection);
	void pageGatewayTimeOut(Connection *connection);
	void pageHttpVersionNotSupported(Connection *connection);

}

#endif /* RESPONSE_HPP */
