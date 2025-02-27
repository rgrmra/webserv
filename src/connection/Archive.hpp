#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include "AStream.hpp"
#include <string>

class Archive : public AStream {

	public:
		Archive(int fd, std::string id);
		Archive(const Archive &src);
		Archive &operator=(const Archive &rhs);
		~Archive(void);

};

#endif /* ARCHIVE_HPP */
