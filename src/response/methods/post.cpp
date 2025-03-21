#include "process.hpp"
#include "URL.hpp"
#include "Connection.hpp"
#include "response.hpp"
#include "code.hpp"
#include "header.hpp"
#include "Cgi.hpp"

using namespace std;

void process::methodPost(Connection *connection) {

	URL *uri = connection->getUri();

	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));

	response::pageNotFound(connection);
}