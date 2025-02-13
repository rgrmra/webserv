#include "Request.hpp"

using namespace std;

Request::Request()
{
}

Request::Request(const string &raw_request)
{
	parseRequest(raw_request);
}

Request::~Request()
{
}

void	Request::printRequest() const
{
	cout << "Method: " << _method << endl;
	cout << "URI: " << _uri << endl;
	cout << "Version: " << _version << endl;
	cout << "Headers:" << endl;
	map<string, string>::const_iterator it = _headers.begin();
	for (; it != _headers.end(); it++)
	{
		cout << it->first << ": " << it->second << endl;
	}
	cout << "Body: " << _body << endl;
}

const string	&Request::getMethod() const
{
	return _method;
}

const string	&Request::getUri() const
{
	return _uri;
}

const string	&Request::getVersion() const
{
	return _version;
}

const map<string, string>	&Request::getHeaders() const
{
	return _headers;
}

const string	&Request::getHeader(const string &key) const
{
	map<string, string>::const_iterator it = _headers.find(key);
	if (it == _headers.end())
		return "";
	return it->second;
}

const string	&Request::getBody() const
{
	return _body;
}

const string	Request::getQueryString() const
{
	string::size_type pos = _uri.find("?");
	if (pos == string::npos)
		return "";
	return _uri.substr(pos + 1);
}

void	Request::parseRequest(const string &raw_request)
{
	istringstream iss(raw_request);
	string line;

	getline(iss, line);
	istringstream first_line(line);
	first_line >> _method >> _uri >> _version;

	while (getline(iss, line) && !line.empty())
	{
		string::size_type pos = line.find(": ");
		string key = line.substr(0, pos);
		string value = line.substr(pos + 2);
		_headers[key] = value;
	}

	_body = "";
	while (getline(iss, line))
	{
		_body += line + "\n";
	}
}
