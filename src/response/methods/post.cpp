#include "Cgi.hpp"
#include "Connection.hpp"
#include "process.hpp"
#include "response.hpp"
#include "code.hpp"

using namespace std;

void process::methodPost(Connection *connection)
{
	if (connection->getUri()->isCgi())
		return connection->setResource(new Cgi(connection));
	
	return response::builder(connection, code::NOT_IMPLEMENTED);
}
