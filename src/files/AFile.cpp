#include "AFile.hpp"
#include "Mime.hpp"
#include "WebServ.hpp"
#include "response.hpp"
#include <string>

using namespace std;

AFile::AFile(std::string path)
	: _path(path),
	  _size(0) {

}

AFile::AFile(const AFile &src)
	: _path(src._path),
	  _size(0) {

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

	extern Mime *mimes;

	return mimes->getType(_path);
}
