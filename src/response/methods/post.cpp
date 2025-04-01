#include "process.hpp"
#include "Connection.hpp"
#include "response.hpp"
#include "code.hpp"
#include "Cgi.hpp"
#include <string>

using namespace std;

/*
	POST method implementation according to RFC 7231 for HTTP/1.1
	- Process CGI requests 
	- Handle multipart/form-data for file uploads
	- Support standard form submissions
	- Return appropriate status codes based on the result
*/
void process::methodPost(Connection *connection) {
	
	if (connection->getUri()->isCgi())
		return connection->setResource(new Cgi(connection));
	
	return response::builder(connection, code::NOT_IMPLEMENTED);
}
