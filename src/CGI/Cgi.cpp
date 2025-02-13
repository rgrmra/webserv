#include "Cgi.hpp"

Cgi::Cgi(Request &req) : _req(req)
{
	_env["REQUEST_METHOD"] = req.getMethod();
	_env["QUERY_STRING"] = req.getQueryString();
	std::ostringstream oss;
	oss << req.getBody().size();
	_env["CONTENT_LENGTH"] = oss.str();
	_defineInterpreter();
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
		char **envp = const_cast<char**>(convertMapToEnv(_env));
		char *argv[] = {_interpreter, strdup(_req.getUri().c_str()), NULL};
		execve(argv[0], argv, envp);
		_dealocateArgEnv(argv, envp);
		close(input[0]);
		close(output[1]);
		exit(1);
	}
	else
	{
		close(input[0]);
		close(output[1]);

		signal(SIGALRM, timeout_handler);
		alarm(5);

		char buffer[4096];
		ssize_t bytes_read;
		while ((bytes_read = read(output[0], buffer, sizeof(buffer))) > 0) {
			_cgi_output.append(buffer, bytes_read);
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
}

void Cgi::_defineInterpreter()
{
	std::string path = _req.getUri();
	if (path.find(".py") != std::string::npos)
		_interpreter = pythonInterpreter;
	else if (path.find(".php") != std::string::npos)
		_interpreter = phpInterpreter;
}

const char **Cgi::createArgv(const std::string &path)
{
	const char **argv = new const char*[3];
	argv[0] = _interpreter;
	argv[1] = strdup(path.c_str());
	argv[2] = NULL;
	return argv;
}

std::string Cgi::getCgiOutput() const
{
	return _cgi_output;
}

const char** Cgi::convertMapToEnv(std::map<std::string, std::string> &env)
{
	const char **envp = new const char*[env.size() + 1];
	size_t i = 0;
	for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); ++it)
	{
		std::string env_var = it->first + "=" + it->second;
		envp[i] = strdup(env_var.c_str());
		i++;
	}
	envp[i] = NULL;
	return envp;
}

void Cgi::_dealocateArgEnv(char **argv, char **envp)
{
	free(argv[1]);
	delete[] argv;
	for (size_t i = 0; envp[i] != NULL; i++)
		free(envp[i]);
	delete[] envp;
}
