#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Connection.hpp"

namespace request {

	void parseRequest(Connection *connection, std::string line);
	// bool 

}

#endif /* REQUEST_HPP */
