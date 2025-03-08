#ifndef CGI_HPP
#define CGI_HPP

#include "Resource.hpp"
#include <string>
#include <vector>

class Connection;

class Cgi : public Resource {

	private:
		int _sock[2];
		pid_t _pid;
		vector<string> _env;

		std::vector<char *> createVector(std::vector<std::string> &container);
		void populateEnv(Connection *connection);
 		void deleteVector(std::vector<char *> &container);
		void closeSockets(void);
		void processInput(size_t bytes);

	public:
		Cgi(Connection *connection);
		Cgi(const Cgi &src);
		Cgi &operator=(const Cgi &rhs);
		virtual ~Cgi(void);

		void sendCGI(void);

};

#endif /* CGI_HPP */
