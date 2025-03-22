#include "Connection.hpp"
#include "Location.hpp"
#include "URL.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "process.hpp"
#include "response.hpp"
#include "code.hpp"
#include <string>

using namespace std;

void process::request(Connection *connection) {

	Location &location = connection->getLocation();
	if (location.empty())
		return response::builder(connection, code::NOT_FOUND);

	URL *uri = connection->getUri();
	if (!uri->isFile() && !uri->isDirectory())
		return response::builder(connection, code::NOT_FOUND);

	if (location.getReturnCode().size())
		return response::builder(connection, code::MOVED_PERMANENTLY);

	const string &method = connection->getMethod();
	if (location.getMethod(method).empty())
		return response::builder(connection, code::NOT_ALLOWED);

	if (method == "GET")
		return methodGet(connection);

	if (method == "POST")
		return methodPost(connection);

	if (method == "DELETE")
		return methodDelete(connection);
}
	
bool process::hasSlashAtEnd(const std::string &path) {

	return (path.at(path.size() - 1) == '/');
}
