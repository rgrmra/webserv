#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Server.hpp"
#include <map>
#include <string>
#include <vector>

class Connection {
	private:
		int _fd;
		std::string _ip;
		std::string _host;
		std::string _buffer;
		std::string _method;
		std::string _path;
		std::string _protocol;
		std::string _code;
		std::string _status;
		std::map<std::string, std::string> _headers;
		std::string _body;
		Server _server;
		std::string _response;
		time_t _time;
		bool _send;

		void parseRequest(void);

	public:
		Connection(int fd, std::string _ip);
		Connection(const Connection &src);
		Connection &operator=(const Connection &rhs);
		virtual ~Connection(void);

		int getFd(void) const;
		std::string getIp(void) const;
		void setHost(std::string host);
		std::string getHost(void) const;
		void append(std::vector<char> &text, int bytes);
		std::string getBuffer(void) const;
		void setMethod(std::string &method);
		std::string getMethod(void) const;
		void setPath(std::string path);
		std::string getPath(void) const;
		void setProtocol(std::string protocol);
		std::string getProtocol(void) const;
		void setCode(std::string code);
		void setCode(size_t code);
		std::string getCode(void) const;
		void setStatus(std::string status);
		std::string getStatus(void) const;
		void addHeader(std::string key, std::string value);
		void addHeader(std::string key, size_t value);
		void setHeaders(std::map<std::string, std::string> headers);
		std::string getHeaders(void) const;
		void setBody(std::string body);
		std::string getBody(void) const;
		Server getServer(void) const;
		time_t getTime(void) const;
		void buildResponse(void);
		std::string getResponse(int bytes);
		std::string getResponse(void) const;
		size_t getResponseSize(void) const;
		void setSend(bool send);
		bool getSend(void) const;

};

std::ostream &operator<<(std::ostream &os, const Connection &src);

#endif /* CONNECTION_HPP */
