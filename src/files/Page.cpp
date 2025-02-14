#include "Page.hpp"
#include "AFile.hpp"
#include "parser.hpp"
#include <cstddef>
#include <sstream>
#include <string>

using namespace std;

Page::Page(const std::string code, const std::string status)
	: AFile() {

	ostringstream oss;
	oss << "<html>\n"
		"<head><title>" + code + " " + status + "</title></head>\n"
		"<body>\n"
		"<center><h1>" + code + " " + status + "</h1></center>\n"
		"<hr><center>webserv</center>\n"
		"</body>\n"
		"</html>\n";

	_page = oss.str();

	_size = _page.size();
	_buffer = _page;
}

Page::Page(const Page &src)
	: AFile() {

	*this = src;
}

Page &Page::operator=(const Page &rhs) {

	if (this != &rhs)
		return *this;

	_page = rhs._page;
	_buffer = rhs._page;
	_size = rhs._size;

	return *this;
}

Page::~Page(void) {

}

string Page::getBuffer(size_t bytes) {

	string tmp = _buffer.substr(0, bytes);
	_buffer.erase(0, bytes);

	return tmp;
}
