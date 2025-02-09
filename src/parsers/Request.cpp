#include "Request.hpp"
#include "Connection.hpp"
#include "parser.hpp"
#include "response.hpp"
#include <cstdio>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

using namespace std;

void request::parseRequest(Connection *connection) {

	istringstream iss(connection->getBuffer());
	string line;

	getline(iss, line);
	list<string> startline = parser::split(line, ' ');	
	if (startline.size() != 3) {
		response::pageBadRequest(connection);
		return;
	}

	cout << connection << endl;
	while(getline(iss, line) && line != "\r\n") {

		list<string> header = parser::split(line, ' ');

		if (header.size() != 2) {
			response::pageBadRequest(connection);
			return;
		}

		connection->addHeader(*header.begin(), *header.end());
	}

	string body;
	while (getline(iss, line))
		body.append(line);


	connection->setBody(body);
	//istringstream first_line(line);
	//first_line >> _method >> _uri >> _version;

	//while (getline(iss, line) && !line.empty())
	//{
	//	string::size_type pos = line.find(": ");
	//	string key = line.substr(0, pos);
	//	string value = line.substr(pos + 2);
	//	connection->addHeader(key, value);
	//}

	//iss >> _body;
	//while (getline(iss, line)) {
	//	_body += line + "\n";
	//}
}
