#include "AStream.hpp"
#include "WebServ.hpp"
#include <ctime>
#include <string>

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
	  _transfers(0),
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
	_input = rhs._input;
	_output = rhs._output;
	_time = rhs._time;
	_size = rhs._size;
	_transfers = rhs._transfers;
	_step = rhs._step;

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

void AStream::processInput(size_t bytes) {

	(void) bytes;
}

void AStream::setData(std::vector<char> &buffer, size_t bytes) {

	if (!bytes || buffer.empty())
		return processInput(bytes);

	_input.append(buffer.begin(), buffer.begin() + bytes);

	processInput(bytes);

	_time = time(NULL);
}

void AStream::processOutput(size_t bytes) {

	(void) bytes;
}

std::string AStream::getData(size_t bytes) {

	processOutput(bytes);

	if (_output.empty())
		return "";

	string tmp = _output.substr(0, bytes);
	_output.erase(0, bytes);

	_time = time(NULL);

	return tmp;
}

size_t AStream::getSize(void) const {

	return _size;
}

void AStream::setStep(int step) {

	_step = step;
}

int AStream::getStep(void) const {

	return _step;
}

bool AStream::isTimedOut(void) const {

	size_t elapsed_time = time(NULL) - _time;
	
	if (_transfers && elapsed_time >= WebServ::KEEP_ALIVE_TIMEOUT)
		return true;

	if (elapsed_time >= WebServ::TIMEOUT)
		return true;

	return false;
}
