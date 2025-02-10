#include "Request.hpp"
#include "Connection.hpp"
#include "parser.hpp"
#include "response.hpp"
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>

using namespace std;

void request::parseRequest(Connection *connection, string line) {

	string method, path, protocol, key, value, body;

	if (!connection->getStartLineParsed()) {

		if (line.empty()) {
			response::pageBadRequest(connection); 
			return;
		}

		istringstream startline(line);
		if (!(startline >> method >> path >> protocol)) {
			response::pageBadRequest(connection);
			return;
		}

		if (method != "GET" && method != "PUT" && method != "PATCH" && method != "POST" && method != "DELETE") {
			response::pageBadRequest(connection);
			return;
		}

		if (protocol != response::PROTOCOL) {
			response::pageHttpVersionNotSupported(connection);
			return;
		}

		connection->setMethod(method);
		connection->setPath(path);
		connection->setProtocol(protocol);
		connection->setStartLineParsed(true);

		cout << line << endl;
		return;
	}

	if (connection->getStartLineParsed() && !connection->getHeadersParsed()) {

		if (line == "\r" || line == "\r\n" || line.empty()) {
			return connection->setHeadersParsed(true);
		}

		size_t separator = line.find(":");
		if (separator == string::npos) {
			response::pageBadRequest(connection);
			return;
		}

		parser::trim(line, " \r");

		string key = line.substr(0, separator);
		string value = line.substr(separator + 1);

		if (key.find(" ") != string::npos) {
			response::pageBadRequest(connection);
			return;
		}

		parser::trim(value, " ");

		if (value.find(" ") != string::npos) {
			response::pageBadRequest(connection);
			return;
		}

		connection->addHeader(key, value);
	}

	if (connection->getHeadersParsed() && !connection->getSend()) {

		cout << "body_line: " << line << endl;
		string tmp = connection->getBody();
		tmp += line;
		connection->setBody(tmp);
		size_t body_size = connection->getBody().size();
		size_t content_length = parser::toSizeT(connection->getHeaderByKey("Content-Length"));

		cout << "body: " << body_size << ", length: " << content_length << endl;

		if (body_size == content_length) {
			connection->setSend(true);
		} else if (body_size > content_length) {
			response::pageBadRequest(connection);
			return;
		}
	}
}
