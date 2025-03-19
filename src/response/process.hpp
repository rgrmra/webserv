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

    void deleteDirectory(Connection *connection, URL *uri);
    void deleteFile(Connection *connection, URL *uri);
    bool hasSlashAtEnd(const std::string &path);
	void methodDeleteCgi(Connection *connection);
};

#endif /* PROCESS_HPP */
