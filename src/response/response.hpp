#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

class Connection;
class Location;

namespace response {

	static std::map<std::string, std::string> responses;

	std::string getStatusByCode(const std::string &code);
	void builder(Connection * connection, std::string code);

	void pageOK(Connection *connection);
	void pageNoContent(Connection *connection);
	void pageMovedPermanently(Connection *connection);
	void pageBadRequest(Connection *connection);
	void pageUnauthorized(Connection *connection);
	void pageFORBIDDEN(Connection *connection);
	void pageNotFound(Connection *connection);
	void pageConflict(Connection *connection);
	void pageMethodNotAllowed(Connection *connection);
	void pageLengthRequired(Connection *connection);
	void pagePayloadTooLarge(Connection *connection);
	void pageURITooLong(Connection *connection);
	void pageUnsupportedMediaType(Connection *connection);
	void pageUnprocessableContent(Connection *connection);
	void pageInternalServerError(Connection *connection);
	void pageNotImplemented(Connection *connection);
	void pageBadGateway(Connection *connection);
	void pageGatewayTimeOut(Connection *connection);
	void pageHttpVersionNotSupported(Connection *connection);

}

#endif /* RESPONSE_HPP */
