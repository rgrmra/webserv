#include "Cgi.hpp"

Cgi::Cgi(Request &req) : _req(req)
{
}

Cgi::~Cgi()
{
}

void Cgi::_launchCgi()
{
	int input[2];
	int output[2];
	pid_t pid;

	if (pipe(input) == -1 || pipe(output) == -1)
	{
		std::cerr << "Error: pipe failed" << std::endl;
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		std::cerr << "Error: fork failed" << std::endl;
		exit(1);
	}
	if (pid == 0)
	{
		close(input[1]);
		close(output[0]);
		dup2(input[0], STDIN_FILENO);
		dup2(output[1], STDOUT_FILENO);
		char *argv[] = {"/bin/python3", "./src/CGI/cgi_test.py", NULL};
		execve("/bin/python3", argv, NULL);
		close(input[0]);
		close(output[1]);
	}
	else
	{
		close(input[0]);
		close(output[1]);

		signal(SIGALRM, timeout_handler);
		alarm(5);

		char buffer[4096];
		ssize_t bytes_read;
		std::string cgi_output;
		while (!_timeout &&
				(bytes_read = read(output[0], buffer, sizeof(buffer))) > 0) {
			cgi_output.append(buffer, bytes_read);
		}

		alarm(0);

		int status;
		if (waitpid(pid, &status, WNOHANG) == 0) {
			kill(pid, SIGKILL);
			waitpid(pid, &status, 0);
			std::cerr << "CGI timed out." << std::endl;
		}
	}
}

void Cgi::timeout_handler(int sig) {
	(void)sig;
	std::cerr << "CGI timed out." << std::endl;
	_timeout = true;
}
