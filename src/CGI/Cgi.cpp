#include "Cgi.hpp"

Cgi::Cgi(Connection &conn) : _conn(conn), _exit_status(0)
{
	_env["SCRIPT_NAME"] = _conn.getPath();
	_validateScript();
	_env["REQUEST_METHOD"] = _conn.getMethod();
	_env["QUERY_STRING"] = sanitizeQueryString(_conn.getQueryString());
	std::stringstream ss;
	ss << _conn.getBody().size();
	_env["CONTENT_LENGTH"] = ss.str();
	_env["CONTENT_TYPE"] = _conn.getHeaderByKey("Content-Type");
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["HTTP_USER_AGENT"] = _conn.getHeaderByKey("User-Agent");
	_env["HTTP_COOKIE"] = _conn.getHeaderByKey("Cookie");
	_launchCgi();
}

Cgi::~Cgi()
{
}

void Cgi::_launchCgi()
{
	int input[2];
	int output[2];
	pid_t pid;

	if (pipe(input) == -1 || pipe(output) == -1) {
		std::cerr << "Error: pipe failed" << std::endl;
		_exit_status = CGI_INTERNAL_ERROR;
		throw std::runtime_error("Pipe failed");
	}

	pid = fork();
	if (pid == -1) {
		std::cerr << "Error: fork failed" << std::endl;
		_exit_status = CGI_INTERNAL_ERROR;
		throw std::runtime_error("Fork failed");
	}

	if (pid == 0) {
		close(input[1]);
		close(output[0]);
		dup2(input[0], STDIN_FILENO);
		dup2(output[1], STDOUT_FILENO);
		std::vector<char*> envp = convertMapToEnv(_env);
		char *argv[] = {strdup(_env["SCRIPT_NAME"].c_str()), NULL};
		execve(argv[0], argv, envp.data());
		_dealocateArgEnv(argv, envp);
		close(input[0]);
		close(output[1]);
		exit(1);
	}
	else {
		close(input[0]);
		close(output[1]);

		write(input[1], _conn.getBody().data(), _conn.getBody().size());
		close(input[1]);

		signal(SIGALRM, timeout_handler);
		alarm(5);

		char buffer[4096];
		ssize_t bytes_read;
		while (true) {
			bytes_read = read(output[0], buffer, sizeof(buffer));
			if (bytes_read > 0) {
				_cgi_output.append(buffer, bytes_read);
			} else if (bytes_read == -1 && errno == EINTR) {
				break; // Timeout triggered
			} else {
				break; // Error or EOF
			}
		}

		alarm(0);

		int status;
		if (waitpid(pid, &status, WNOHANG) == 0) {
			kill(pid, SIGKILL);
			waitpid(pid, &status, 0);
			_exit_status = CGI_TIMEOUT;
			throw std::runtime_error("CGI timed out");
		}
		_exit_status = WEXITSTATUS(status);
		if (_exit_status != 0) {
			_exit_status = CGI_BAD_GATEWAY;
			throw std::runtime_error("CGI failed");
		}
		_exit_status = CGI_SUCCESS;
	}
}

void Cgi::_validateScript()
{
	if (access(_env["SCRIPT_NAME"].c_str(), F_OK) == -1) {
		_exit_status = CGI_NOT_FOUND;
		throw std::runtime_error("Script not found");
	}
	if (access(_env["SCRIPT_NAME"].c_str(), X_OK) == -1) {
		_exit_status = CGI_FORBIDDEN;
		throw std::runtime_error("Script not executable");
	}
}

void Cgi::timeout_handler(int sig) {
	(void)sig;
	std::cerr << "CGI timed out." << std::endl;
}

const std::string &Cgi::getCgiOutput() const
{
	return _cgi_output;
}

const int &Cgi::getExitStatus() const
{
	return _exit_status;
}

void Cgi::_dealocateArgEnv(char **argv, std::vector<char*> envp)
{
	free(argv[0]);
	for (std::vector<char*>::iterator it = envp.begin();
		it != envp.end(); ++it)
	{
		free(*it);
	}
}

std::string Cgi::sanitizeQueryString(const std::string& query) {

	std::string sanitized;

	for (std::string::const_iterator it = query.begin(); it != query.end(); ++it) {
		char c = *it;
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '=' || c == '&') {
			sanitized += c;
		}
	}
	return sanitized;
}

std::vector<char*> Cgi::convertMapToEnv(const std::map<std::string, std::string>& env) {

	std::vector<char*> envp;

	for (std::map<std::string, std::string>::const_iterator it = env.begin();
			it != env.end(); ++it) {
		std::string env_var = it->first + "=" + it->second;
		envp.push_back(strdup(env_var.c_str()));
	}
	envp.push_back(NULL);
	return envp;
}
