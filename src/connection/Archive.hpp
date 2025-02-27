#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include "AStream.hpp"
#include <sched.h>
#include <string>

class Archive : public AStream {
	private:
		int _default_fd;
		int sock[2];
		pid_t pid;

	public:
		Archive(int fd, std::string id);
		Archive(const Archive &src);
		Archive &operator=(const Archive &rhs);
		~Archive(void);

};

#endif /* ARCHIVE_HPP */
