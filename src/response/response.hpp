#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

class Connection;

namespace response {

	static const std::string PROTOCOL = "HTTP/1.1";
	static const std::map<std::string, std::string> EMPTY_HEADER;
	
	std::string pageBadRequest(Connection * connection);
	std::string pageUnauthorized(Connection * connection);
	std::string pageForbbiden(Connection * connection);
	std::string pageNotFound(Connection * connection);
	std::string pageMethodNotAllowed(Connection * connection);
	std::string pageLengthRequired(Connection * connection);
	std::string pagePayloadTooLarge(Connection * connection);
	std::string pageUnsupportedMediaType(Connection * connection);
	std::string pageInternalServerError(Connection * connection);
	std::string pageGatewayTimeOut(Connection *connection);
	std::string pageHttpVersionNotSupported(Connection * connection);
	std::string setPageWithPath(Connection * connection);
	std::string setResponse(Connection * connection);

	void		setContentTypes(Connection * connection);
	void		setHeader(Connection * connection);
	void		setBody(Connection * connection);
	void		buildResponseBody(Connection *connection);
	void		setPathAndMethod(Connection *connection);
	Location	isPathValid(Connection * connection);


	bool		checkIndex(const Location &location, Connection * connection);
	bool		isDirectory(const std::string &path);
	bool		isFile(const std::string &path);
	bool		isValidMethod(const std::string &method);
	bool		isCGI(const std::string &path);
}

#endif /* RESPONSE_HPP */
