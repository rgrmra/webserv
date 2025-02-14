#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

class Connection;
class Location;

namespace response {

	static const std::string PROTOCOL = "HTTP/1.1";
	static const std::map<std::string, std::string> EMPTY_HEADER;

	void pageBadRequest(Connection *connection);
	void pageUnauthorized(Connection *connection);
	void pageForbbiden(Connection *connection);
	void pageNotFound(Connection *connection);
	void pageNotAllowed(Connection *connection);
	void pageLengthRequired(Connection *connection);
	void pagePayloadTooLarge(Connection *connection);
	void pageUnsupportedMediaType(Connection *connection);
	void pageUnprocessableContent(Connection *connection);
	void pageInternalServerError(Connection *connection);
	void pageGatewayTimeOut(Connection *connection);
	void pageHttpVersionNotSupported(Connection *connection);
	//std::string setPageWithPath(Connection * connection);

	void pageOK(Connection *connection);

	void		setContentTypes(Connection * connection);
	//void		setHeader(Connection * connection);
	//void		setBody(Connection * connection);
	void		buildResponseBody(Connection *connection);
	void		setPathAndMethod(Connection *connection);
	Location	isPathValid(Connection * connection);


	bool		checkIndex(const Location &location, Connection * connection);
	bool		isDirectory(const std::string &path);
	bool		isFile(const std::string &path);
	//bool		isValidMethod(const std::string &method);
	bool		isCGI(const std::string &path);
}

#endif /* RESPONSE_HPP */
