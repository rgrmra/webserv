#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <string>

class Request
{
public:
	Request();
	Request(const std::string &raw_request);
	~Request();

	void	printRequest() const; //debug purposes

	const	std::string &getMethod() const;
	const	std::string &getUri() const;
	const	std::string &getVersion() const;
	const	std::map<std::string, std::string> &getHeaders() const;
	const	std::string &getBody() const;
	void	parseRequest(const std::string &raw_request);

private:
	std::string							_method;
	std::string							_uri;
	std::string							_version;
	std::map<std::string, std::string>	_headers;
	std::string 						_body;

};

#endif
