#include "process.hpp"
#include "URL.hpp"
#include "Connection.hpp"
#include "response.hpp"
#include "code.hpp"
#include "header.hpp"
#include "Cgi.hpp"
#include "File.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cerrno>

using namespace std;

/*
	POST method implementation according to RFC 7231 for HTTP/1.1
	- Process CGI requests 
	- Handle multipart/form-data for file uploads
	- Support standard form submissions
	- Return appropriate status codes based on the result
*/
void process::methodPost(Connection *connection) {
	URL *uri = connection->getUri();
	
	if (connection->getHeaderByKey(header::CONTENT_LENGTH).empty())
		return response::builder(connection, code::LENGTH_REQUIRED);

	string contentType = connection->getHeaderByKey(header::CONTENT_TYPE);
	if (contentType.empty())
		// TODO: Verificar se isso é necessário
		// Content-Type is not strictly required but recommended
		contentType = "application/octet-stream"; // Default
	
	// TODO: Check max body size from location config
	size_t contentLength = 0;
	istringstream contentLengthStream(connection->getHeaderByKey(header::CONTENT_LENGTH));
	if (!(contentLengthStream >> contentLength))
		return response::builder(connection, code::BAD_REQUEST);
	
	size_t maxBodySize = connection->getLocation().getMaxBodySize();
	if (maxBodySize > 0 && contentLength > maxBodySize)
		return response::builder(connection, code::PAYLOAD_TOO_LARGE);
	
	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));
		
	if (!uri->isWritable())
		return response::builder(connection, code::FORBBIDEN);
	
	// Handle multipart/form-data (file uploads)
	if (contentType.find("multipart/form-data") != string::npos)
		return handleMultipartFormData(connection);
	
	// Handle normal form submissions
	if (contentType == "application/x-www-form-urlencoded")
		return handleFormSubmission(connection);
	
	// Create or append to a file
	if (uri->isFile() || !uri->isDirectory())
		return handleFileUpload(connection);
	
	return response::builder(connection, code::NOT_IMPLEMENTED);
}
