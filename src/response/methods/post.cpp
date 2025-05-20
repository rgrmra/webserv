#include "Cgi.hpp"
#include "Connection.hpp"
#include "File.hpp"
#include "process.hpp"
#include "response.hpp"
#include "code.hpp"
#include <fstream>

using namespace std;

void process::methodPost(Connection *connection)
{
	URL *uri = connection->getUri();

	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));

	if (!uri->isDeletable())
		return response::builder(connection, code::FORBIDDEN);

	if (uri->isFile() || uri->isDirectory())
		return response::builder(connection, code::CONFLICT);

	ofstream file(uri->getAbsolutePath().c_str());

	if (!file.is_open())
		return response::builder(connection, code::INTERNAL_SERVER_ERROR);

	file << connection->getBody();
	file.close();

	return response::builder(connection, code::CREATED);
}
