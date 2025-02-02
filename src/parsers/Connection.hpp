#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <cstddef>
#include <string>

class Connection {

	public:
		static const size_t BUFFER_SIZE = 4096;
    	char buffer[BUFFER_SIZE];
    	size_t buffer_pos;
    	std::string request;
    	bool headers_complete;
    	size_t content_length;
    	bool request_complete;

    	Connection()
			: buffer_pos(0),
			  headers_complete(false),
			  content_length(0),
			  request_complete(false) {
		}

};

#endif /* CONNECTION_HPP */
