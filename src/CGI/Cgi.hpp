#ifndef CGI_HPP
#define CGI_HPP

#include "Connection.hpp"
#include <string>
#include <map>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <cerrno>
#include <cstdlib>

#define CGI_SUCCESS 200
#define CGI_TIMEOUT 504
#define CGI_BAD_GATEWAY 502
#define CGI_INTERNAL_ERROR 500
#define CGI_NOT_FOUND 404
#define CGI_FORBIDDEN 403

class Cgi
{

public:
	Cgi(Connection &conn);
	~Cgi();

	Connection			&_conn;
	static void			timeout_handler(int signum);
	const std::string	&getCgiOutput() const;
	const int			&getExitStatus() const;
	std::vector<char*>	convertMapToEnv(const std::map<std::string, std::string>& env);
	static std::string	sanitizeQueryString(const std::string& query);

private:
	std::map<std::string, std::string>	_env;
	int									_exit_status;
	std::string		_cgi_output;
	void			_launchCgi();
	void			_validateScript();
	void			_dealocateArgEnv(char **argv, std::vector<char*> envp);

};

#endif
