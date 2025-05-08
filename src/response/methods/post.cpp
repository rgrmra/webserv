#include "process.hpp"
#include "Connection.hpp"
#include "response.hpp"
#include "code.hpp"
#include "Cgi.hpp"
#include <string>

using namespace std;

void process::methodPost(Connection *connection) {
	
	if (connection->getUri()->isCgi())
		return connection->setResource(new Cgi(connection));
	
	return response::builder(connection, code::NOT_IMPLEMENTED);
}
