#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>

class Connection;
class Location;
class URL;

namespace process {

	void request(Connection *connection);
	void methodGet(Connection *connection);
	void methodPost(Connection *connection);
	void methodDelete(Connection *connection);

	void deleteDirectory(Connection *connection, URL *uri);
	void deleteFile(Connection *connection, URL *uri);
	void methodDeleteCgi(Connection *connection);
	
	void handleMultipartFormData(Connection *connection);
	void handleFormSubmission(Connection *connection);
	void handleFileUpload(Connection *connection);
	
	std::string urlDecode(const std::string& encoded);
};

#endif /* PROCESS_HPP */
