#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>

class Connection;
class Location;

namespace process {

	void request(Connection *connection);
	void methodGet(Connection *connection);
	void methodPost(Connection *connection);
	void methodDelete(Connection *connection);

	bool isFile(const std::string &path);
	bool isDirectory(const std::string &path);
	bool hasSlashAtEnd(const std::string &path);
	bool isCGI(const std::string &path);
	std::string checkIndex(const Location &location, std::string &path);

};

#endif /* PROCESS_HPP */
