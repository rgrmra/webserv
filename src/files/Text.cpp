#include "AFile.hpp"
#include "Text.hpp"

using namespace std;

Text::Text(std::string text)
	: AFile(".txt") {

	_buffer = text;
	_size = text.size();
}

Text::Text(const Text &src)
	: AFile(src._path) {

	*this = src;
}

Text &Text::operator=(const Text &rhs) {

	if (this == &rhs)
		return *this;

	_path = rhs._path;
	_buffer = rhs._buffer;
	_size = rhs._size;

	return *this;
}

Text::~Text(void) {

}

bool Text::empty(void) const {

	return _buffer.empty();
}

std::string Text::getBuffer(size_t bytes) {

	if (_buffer.empty())
		return "";

	string tmp = _buffer.substr(0, bytes);
	_buffer.erase(0, bytes);

	return tmp;
}

