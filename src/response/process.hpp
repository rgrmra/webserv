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

};

#endif /* PROCESS_HPP */
