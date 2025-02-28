#include "AStream.hpp"
#include "Mime.hpp"
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
	  _time(time(NULL)),
	  _size(0),
	  _step(IStream::NONE) {

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

	if (!bytes || _input.empty())
		return "";

	string tmp = _input.substr(0, bytes);
	_input.erase(0, bytes);

	_time = time(NULL);

	return tmp;
}

size_t AStream::getSize(void) const {

	return _size;
}

string AStream::getMime(void) const {

	return Mime::getInstance()->getType(_id);
}

void AStream::setStep(int value) {

	_step = value;
}

int AStream::getStep(void) const {

	return _step;
}

bool AStream::isTimedOut(void) const {

	if (time(NULL) - _time > WebServ::TIMEOUT)
		return true;

	return false;
}
