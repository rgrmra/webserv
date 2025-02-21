#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Connection.hpp"

namespace request {

	void parseRequest(Connection *connection, std::string line);
	void parseStartLine(Connection *connection, std::string line);
	void splitPathQuery(Connection *connection);
	void parseHeaders(Connection *connection, std::string line);
	bool validateHeaders(Connection *connection, size_t separator);
}

#endif /* REQUEST_HPP */
