#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Server.hpp"
#include <ctime>
#include <map>
#include <ostream>
#include <string>
#include <vector>

class Connection {
	private:
		int _fd;
		std::string _ip;
		std::string _host;
		std::vector<char> _buffer;
		std::string _method;
		std::string _path;
		std::string _protocol;
		std::map<std::string, std::string> _headers;
		std::vector<char> _body;
		Server _server;
		std::string _response;
		time_t _time;

		bool validateRequest(void);
		bool validateGet(void);
		bool validatePost(void);
		bool validateDelete(void);
		bool validatePut(void);
		bool validatePatch(void);

	public:
		Connection(int fd, std::string _ip);
		Connection(const Connection &src);
		Connection &operator=(const Connection &rhs);
		virtual ~Connection(void);

		void append(std::vector<char> &text, int bytes);
		std::string getResponse(int bytes);

		int getFd(void) const;
		std::string getIp(void) const;
		std::string getHost(void) const;
		std::string getBuffer(void) const;
		std::string getMethod(void) const;
		std::string getPath(void) const;
		std::string getProtocol(void) const;
		std::string getHeaders(void) const;
		std::string getBody(void) const;
		Server getServer(void) const;
		time_t getTime(void) const;
		std::string getResponse(void) const;
		size_t getResponseSize(void) const;

};

std::ostream &operator<<(std::ostream &os, const Connection &src);

#endif /* CONNECTION_HPP */
