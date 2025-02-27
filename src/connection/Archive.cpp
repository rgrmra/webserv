#include "Archive.hpp"
#include "AStream.hpp"

Archive::Archive(int fd, std::string id)
	: AStream(fd, id) {

}

Archive::Archive(const Archive &src)
	: AStream(src) {

	*this = src;
}

Archive &Archive::operator=(const Archive &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

Archive::~Archive(void) {

}
