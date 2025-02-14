#include "AFile.hpp"
#include <string>

using namespace std;

AFile::AFile(void)
	: _size(0) {

}

AFile::AFile(const AFile &src)
	: _size(0) {

	*this = src;
}

AFile &AFile::operator=(const AFile &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

AFile::~AFile(void) {

}

size_t AFile::getSize(void) const {

	return _size;
}

string AFile::getMime(void) const {

	return "text/plain";
}
