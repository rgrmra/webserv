#include "AStream.hpp"
#include "WebServ.hpp"
#include <iostream>

int _fd;
std::string _id;
std::string _buffer;
std::time_t _time;

using namespace std;

AStream::AStream(int fd, std::string id)
	: _fd(fd),
	  _id(id),
	  _time(time(NULL)) {

	// TODO: remove mock
	_output = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
}

AStream::AStream(const AStream &src) {

	*this = src;
}

AStream &AStream::opeartor(const AStream &rhs) {

	if (this == &rhs)
		return *this;

	_fd = rhs._fd;
	_id = rhs._id;
	_time = rhs._time;

	return *this;
}
		
AStream::~AStream(void) {

}

int AStream::getFd(void) const {

	return _fd;
}

std::string AStream::getId(void) const {

	return _id;
}

void AStream::setData(std::vector<char> &buffer, size_t bytes) {

	if (!bytes || buffer.empty())
		return;

	_input.append(buffer.begin(), buffer.begin() + bytes);

	_time = time(NULL);
}

std::string AStream::getData(size_t bytes) {

	if (!bytes || _output.empty())
		return "";

	string tmp = _output.substr(0, bytes);
	_output.erase(0, bytes);

	return tmp;
}

bool AStream::isTimedOut(void) {

	if (time(NULL) - _time > WebServ::TIMEOUT)
		return true;

	return false;
}
