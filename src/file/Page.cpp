#include "Connection.hpp"
#include "Page.hpp"
#include <sstream>
#include <string>

using namespace std;

Page::Page(Connection *connection)
	: Resource(connection) {

	_type = "text/html";

	string code = connection->getCode();
	string status = connection->getStatus();

	ostringstream oss;
	oss << "<html>\n"
		"<head><title>" + code + " " + status + "</title></head>\n"
		"<body>\n"
		"<center><h1>" + code + " " + status + "</h1></center>\n"
		"<hr><center>webserv/0.1.0</center>\n"
		"</body>\n"
		"</html>\n";

	_output = oss.str();
	_size = _output.size();
	_connection->buildResponse();
	_connection->setStep(IStream::RESPONSE);
	_step = CLOSE;
}

Page::Page(const Page &src)
	: Resource(src._connection) {

	*this = src;
}

Page &Page::operator=(const Page &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

Page::~Page(void) {

}
