#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>

class Connection;
class Location;

namespace process {

	void request(Connection *connection);

	bool isFile(const std::string &path);
	bool isDirectory(const std::string &path);
	bool isCGI(const std::string &path);
	bool checkIndex(const Location &location, std::string &path);
	std::string getFileExtension(std::string path);
	Location isValidPath(Connection *connection);
	std::string getPathFromReferer(std::string url);
	std::string getFileFromPath(std::string path);

};

#endif /* PROCESS_HPP */
