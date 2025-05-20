#include "Connection.hpp"
#include "Location.hpp"
#include "URL.hpp"
#include "method.hpp"
#include "parser.hpp"
#include "process.hpp"
#include "response.hpp"
#include "code.hpp"
#include <string>

using namespace std;

void process::request(Connection *connection)
{
	const Location &location = connection->getLocation();
	if (location.empty())
		return response::builder(connection, code::NOT_FOUND);

	if (location.getFastCgi().empty() != location.getFastCgiExtension().empty())
		return response::builder(connection, code::BAD_GATEWAY);

	if (location.getReturnCode().size())
		return response::builder(connection, code::MOVED_PERMANENTLY);

	const string &method = connection->getMethod();
	if (location.getMethod(method).empty())
		return response::builder(connection, code::NOT_ALLOWED);

	if (method == method::GET)
		return methodGet(connection);

	if (method == method::POST)
		return methodPost(connection);

	if (method == method::DELETE)
		return methodDelete(connection);
}
