#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include "AStream.hpp"
#include "Connection.hpp"
#include <sched.h>
#include <string>
#include <vector>

class Archive : public AStream {
	private:
		//int _default_fd;
		int _sock[2];
		int _wpid;
		int _status;
		pid_t _pid;
		Connection *_connection;

	public:
		Archive(Connection *connection);
		Archive(const Archive &src);
		Archive &operator=(const Archive &rhs);
		~Archive(void);

		void setData(std::vector<char> &buffer, size_t bytes);
		void teste(void);

};

#endif /* ARCHIVE_HPP */
