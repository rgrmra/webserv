#include "code.hpp"
#include "Connection.hpp"
#include "logger.hpp"
#include "response.hpp"
#include "status.hpp"
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

void	response::setContentTypes(Connection *connection)
{
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
		
	logger::debug("Content-Type: " + connection->getHeaderByKey("Content-Type"));
}

void	response::setHeader(Connection *connection)
{
	setContentTypes(connection);
	connection->addHeader("Connection", "closed");
	
}

static void buildHeaderAndBody(Connection *connection) {

	string tmp;

	if (tmp.empty()) {
		ostringstream oss;
		oss << "<html><head><title>" + connection->getCode() + " " + connection->getStatus() + "</title>"
			"</head><body><center><h1>" + connection->getCode() + " " + connection->getStatus() + "</h1>"
			"</center><hr><center>webserv</center></body></html>";
		tmp = oss.str();
	}

	connection->setProtocol(response::PROTOCOL);
	connection->setHeaders(response::EMPTY_HEADER);
	response::setHeader(connection);

	if (connection->getBody().empty())
	{
		connection->addHeader("Content-Lenght", tmp.size());
		connection->setBody(tmp);
	}
	else
		connection->addHeader("Content-Lenght", connection->getBody().size());
	connection->buildResponse();
}

string response::pageOk(Connection * connection) {

	cout << "Buffer: " << connection->getBuffer() << endl;
	istringstream iss(connection->getBuffer());
	string method, uri;
	iss >> method >> uri;
	connection->setMethod(method);
	connection->setPath(uri);

	connection->setCode(200);
	connection->setStatus("Ok");
	logger::info(connection->getIp() 
		+ " " + connection->getMethod() 
		+ " " + connection->getPath() 
		+ " " + connection->getCode() 
		+ " " + connection->getStatus());
	logger::info(connection->getPath());

	string path = "./var/www/html/mysite" + connection->getPath();
	ifstream file(path.c_str());
	if (not file.is_open())
		return response::pageNotFound(connection);
	string line;
	string body;
	while (getline(file, line))
		body += line;
	file.close();
	logger::info(body);
	connection->setBody(body);
	buildHeaderAndBody(connection);
	return connection->getResponse();
}

string response::pageBadRequest(Connection * connection) {

	connection->setCode(code::BAD_REQUEST);
	connection->setStatus(status::BAD_REQUEST);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageUnauthorized(Connection * connection) {

	connection->setCode(code::UNAUTHORIZED);
	connection->setStatus(status::UNAUTHORIZED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageForbbiden(Connection * connection) {

	connection->setCode(code::FORBBIDEN);
	connection->setStatus(status::FORBBIDEN);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string response::pageNotFound(Connection * connection) {

	connection->setCode(code::NOT_FOUND);
	connection->setStatus(status::NOT_FOUND);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageMethodNotAllowed(Connection * connection) {

	connection->setCode(code::METHOD_NOT_ALLOWED);
	connection->setStatus(status::METHOD_NOT_ALLOWED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageLengthRequired(Connection * connection) {

	connection->setCode(code::LENGTH_REQUIRED);
	connection->setStatus(status::LENGTH_REQUIRED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pagePayloadTooLarge(Connection * connection) {

	connection->setCode(code::PAYLOAD_TOO_LARGE);
	connection->setStatus(status::PAYLOAD_TOO_LARGE);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string pageUnsupportedMediaType(Connection * connection) {

	connection->setCode(code::UNSUPORTED_MEDIA_TYPE);
	connection->setStatus(status::UNSUPPORTED_MEDI_TYPE);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string response::pageInternalServerError(Connection * connection) {

	connection->setCode(code::INTERNAL_SERVER_ERROR);
	connection->setStatus(status::INTERNAL_SERVER_ERROR);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}

string response::pageGatewayTimeOut(Connection *connection) {

	connection->setCode(code::GATEWAY_TIMEOUT);
	connection->setStatus(status::GATEWAY_TIMEOUT);
	buildHeaderAndBody(connection);
	
	logger::warning(connection->getIp() + " timed out");

	return connection->getResponse();
}

string response::pageHttpVersionNotSupported(Connection * connection) {

	connection->setCode(code::HTTP_VERSION_NOT_SUPPORTED);
	connection->setStatus(status::HTTP_VERSION_NOT_SUPPPORTED);
	buildHeaderAndBody(connection);

	return connection->getResponse();
}
