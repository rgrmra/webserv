#include "code.hpp"
#include "Connection.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "response.hpp"
#include "status.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <sys/stat.h>

using namespace std;

bool response::isDirectory(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;
    return (info.st_mode & S_IFDIR) != 0;
}

bool response::isFile(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
	{
        return false;

	}
    return (info.st_mode & S_IFREG) != 0;
}

bool response::isCGI(const std::string &path) {
    size_t pos = path.find_last_of(".");
    if (pos == std::string::npos) {
        return false;
    }
    std::string extension = path.substr(pos);
    return extension == ".php" || extension == ".py" || extension == ".go" || extension == ".cgi";
}

void	response::setContentTypes(Connection *connection)
{
	if (connection->getCode() != "200")
	{
		connection->addHeader("Content-Type", "text/html");
		return;
	}

	map<string, string> content_types;

	content_types[".html"] = "text/html";
	content_types[".css"] = "text/css";
	content_types[".js"] = "text/javascript";
	content_types[".jpg"] = "image/jpeg";
	content_types[".jpeg"] = "image/jpeg";
	content_types[".png"] = "image/png";
	content_types[".gif"] = "image/gif";
	content_types[".bmp"] = "image/bmp";
	content_types[".ico"] = "image/x-icon";
	content_types[".svg"] = "image/svg+xml";
	content_types[".mp3"] = "audio/mpeg";

	string path = connection->getPath();
	size_t pos = path.find_last_of(".");

	if (pos != string::npos)
	{
		string extension = path.substr(pos);
		map<string, string>::iterator it = content_types.find(extension);
		if (it != content_types.end())
			connection->addHeader("Content-Type", it->second);
		else
			connection->addHeader("Content-Type", "text/plain");
	}
}

void	response::setHeader(Connection *connection)
{
	setContentTypes(connection);
	connection->addHeader("Connection", "closed");

}

Location response::isPathValid(Connection *connection) {
	string path = connection->getPath();
	if (path.find("..") != string::npos || path.find('/') == string::npos)
		return Location();

	while (!path.empty()) {
		Location temp = connection->getServer().getLocationByURI(path);
		if (not temp.getURI().empty())
		{
			return temp;
		}

		size_t lastSlash = path.find_last_of('/');
		if (lastSlash == 0)
			path = "/";
		else if (lastSlash != string::npos)
			path = path.substr(0, lastSlash);
		else
			path.clear();
	}
	return Location();
}

static void buildHeaderAndBody(Connection *connection) {

	string tmp;

	if (tmp.empty()) {

		tmp = connection->getCode() + " " + connection->getStatus();

		ostringstream oss;
		oss << "<html>\n"
			"<head><title>" + tmp + "</title></head>\n"
			"<body>\n"
			"<center><h1>" + tmp + "</h1></center>\n"
			"<hr><center>webserv</center>\n"
			"</body>\n"
			"</html>\n";
		tmp = oss.str();
	}

	connection->setProtocol(response::PROTOCOL);
	connection->setHeaders(response::EMPTY_HEADER);

	// connection->addHeader(header::CONTENT_TYPE, "text/html");
	response::setContentTypes(connection);
	connection->addHeader(header::CONTENT_LENGTH, tmp.size());
	connection->addHeader(header::CONNECTION, "close");

	// connection->setBody(tmp);
	// connection->buildResponse();
	// response::setHeader(connection);

	if (connection->getBody().empty())
	{
		connection->addHeader("Content-Length", tmp.size());
		connection->setBody(tmp);
	}
	else
		connection->addHeader("Content-Length", connection->getBody().size());
}

bool response::checkIndex(const Location &location, Connection *connection) {
	const std::set<std::string> &indexes = location.getIndexes();
	const string &path = connection->getPath();
	const string &bar = path.find_last_of('/') == path.size() - 1 ? "" : "/";

	typedef std::set<std::string>::const_iterator set_iterator;
	for (set_iterator it = indexes.begin(); it != indexes.end(); ++it) {
        std::string indexPath = path + bar + *it;
        if (isFile(indexPath)) {
            connection->setPath(indexPath);
            return true;
        }
    }
    return false;
}



bool response::isValidMethod(const std::string &method) {
	return method == "GET" || method == "POST" || method == "DELETE";
}

void response::setResponse(Connection * connection) {
	if (connection->getProtocol().empty()
		|| connection->getCode().empty() || connection->getStatus().empty())
		response::pageInternalServerError(connection);
	else
	{
		if (not isValidMethod(connection->getMethod()))
			response::pageMethodNotAllowed(connection);

		Location location = isPathValid(connection);
		if (location.getURI().empty())
			response::pageNotFound(connection);

		string root = location.getRoot().empty() ?
			connection->getServer().getRoot()
			: location.getRoot();
		string path = "." + root + connection->getPath();

		connection->setPath(path);
		if (isDirectory(path)
			&& not checkIndex(location, connection))
			{
				response::pageForbbiden(connection);
				return;
			}

		if (isCGI(path))
		{
			// cout << "CGI Path:::::: " << path << endl;
			// function to handle CGI
		}

		connection->setCode(200);
		connection->setStatus("Ok");

		response::buildResponseBody(connection);
	}
}

void response::buildResponseBody(Connection *connection) {
    ifstream file(connection->getPath().c_str());
    if (not file.is_open()) {
		response::pageNotFound(connection);
        return;
    }

    string line;
    string body;
    while (getline(file, line))
        body += line;
    file.close();

    logger::info(body);
    connection->setBody(body);
    buildHeaderAndBody(connection);
}

void response::pageBadRequest(Connection *connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	buildHeaderAndBody(connection);
}

void response::pageUnauthorized(Connection *connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	buildHeaderAndBody(connection);
}

void response::pageForbbiden(Connection *connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	buildHeaderAndBody(connection);
}

void response::pageNotFound(Connection *connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	buildHeaderAndBody(connection);
}

void response::pageMethodNotAllowed(Connection *connection) {

	connection->setCode(code::METHOD_NOT_ALLOWED);
	connection->setStatus(status::METHOD_NOT_ALLOWED);
	buildHeaderAndBody(connection);
}

void response::pageLengthRequired(Connection *connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	buildHeaderAndBody(connection);
}

void response::pagePayloadTooLarge(Connection *connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	buildHeaderAndBody(connection);
}

void response::pageUnsupportedMediaType(Connection *connection) {

	connection->setCode(code::UNSUPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDIA_TYPE);
	buildHeaderAndBody(connection);
}

void response::pageUnprocessableContent(Connection *connection) {

	connection->setCode(code::UNPROCESSABLE_CONTENT);
	connection->setStatus(status::UNPROCESSABLE_CONTENT);
	buildHeaderAndBody(connection);
}

void response::pageInternalServerError(Connection *connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	buildHeaderAndBody(connection);
}

void response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	buildHeaderAndBody(connection);

	logger::warning(connection->getIp() + " timed out");
}

void response::pageHttpVersionNotSupported(Connection * connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPPORTED);

	buildHeaderAndBody(connection);
}
