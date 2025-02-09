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

class Cgi
{

public:
	Cgi(Request &req);
	~Cgi();
	static void	timeout_handler(int signum);

private:
	Request			&_req;
	static bool		_timeout = false;
	void			_launchCgi();
};

#endif
