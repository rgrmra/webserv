#include "Connection.hpp"
#include "Environment.hpp"
#include "Server.hpp"
#include "URL.hpp"
#include "code.hpp"
#include "env.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "standard.hpp"
#include <cstring>
#include <list>
#include <map>

Environment::Environment(Connection *connection)
{
	URL *url = connection->getUri();
	Server &server = connection->getServer();

	std::string server_name;
	if (server.getNames().size())
		server_name = server.getNames()[0];

	std::list<std::string> tmp = parser::split(connection->getId(), ':');
	addEnv(env::GATEWAY_INTERFACE, standard::GATEWAY_INTERFACE);
	addEnv(env::QUERY_STRING, connection->getUri()->getQuery());
	addEnv(env::REMOTE_ADDR, connection->getId());
	addEnv(env::REMOTE_HOST, (*connection)[header::HOST]);
	addEnv(env::REMOTE_PORT, tmp.back());
	addEnv(env::REQUEST_METHOD, connection->getMethod());
	addEnv(env::SCRIPT_NAME, url->getPath());
	addEnv(env::SERVER_NAME, server_name);
	addEnv(env::SERVER_PROTOCOL, standard::PROTOCOL);
	addEnv(env::SERVER_SOFTWARE, standard::SERVER_SOFTWARE);
	addEnv(env::SCRIPT_FILENAME, connection->getUri()->getAbsolutePath());
	addEnv(env::REDIRECT_STATUS, code::OK);

	if (url->getPathInfo().size())
	{
		addEnv(env::PATH_INFO, url->getPathInfo());
		addEnv(env::PATH_TRANSLATED, url->getPathTranslated());
	}

	std::map<std::string, std::string> headers = connection->getHeaders();
	std::map<std::string, std::string>::const_iterator header = headers.begin();
	for (; header != headers.end(); ++header)
	{
		std::string key = header->first;
		std::string value = header->second;

		std::string transformed_key = parser::toUpper(key);
		parser::replace(transformed_key, '-', '_');

		if (transformed_key == env::CONTENT_LENGTH)
			addEnv(env::CONTENT_LENGTH, value);
		else if (transformed_key == env::CONTENT_TYPE)
			addEnv(env::CONTENT_TYPE, value);
		else if (transformed_key == env::AUTHORIZATION)
			addEnv(env::AUTHORIZATION, value);
		else
			addEnv(env::HTTP_PREFIX + transformed_key, value);
	}
	_env.push_back(NULL);

	connection = NULL;
}

Environment::Environment(const Environment &src)
{
	*this = src;
}

Environment &Environment::operator=(const Environment &rhs)
{
	if (this == &rhs)
		return *this;

	return *this;
}

Environment::~Environment(void)
{
	std::vector<char *>::iterator environment = _env.begin();
	for (; environment != _env.end(); ++environment)
		delete[] *environment;

	_env.clear();
}

void Environment::addEnv(const std::string &key, const std::string &value)
{
	std::string environment = key + "=" + value;
	char *env = new char[environment.size() + 1];
	_env.push_back(std::strcpy(env, environment.c_str()));
}

std::vector<char *> Environment::getEnvironment(void)
{
	return _env;
}
