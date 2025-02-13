#ifndef CGI_HPP
#define CGI_HPP

#include "Request.hpp"
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

#define pythonInterpreter "/bin/python3"
#define phpInterpreter "/usr/bin/php-cgi"

class Cgi
{

public:
	Cgi(Request &req);
	~Cgi();

	static void			timeout_handler(int signum);
	const char ** 		createArgv(const std::string &path);
	std::string			getCgiOutput() const;
	const static char**	convertMapToEnv(std::map<std::string, std::string> &env);

	private:
	Request								&_req;
	std::map<std::string, std::string>	_env;
	char *								_interpreter;

	std::string		_cgi_output;
	void			_launchCgi();
	void			_defineInterpreter();
	void			_dealocateArgEnv(char **argv, char **envp);

};

#endif
