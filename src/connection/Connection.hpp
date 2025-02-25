#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Location.hpp"
#include "Server.hpp"
#include <ctime>
#include <map>
#include <string>
#include <vector>

class AFile;
class Http;

class Connection {
	private:
		Http *_http;
		int _fd;
		std::string _ip;
		std::string _host;
		std::string _buffer;
		std::string _method;
		std::string _uri;
		std::string _path;
		std::string _query_string;
		std::string _protocol;
		std::string _code;
		std::string _status;
		std::map<std::string, std::string> _headers;
		std::string _body;
		AFile *_file;
		Server _server;
		Location _location;
		std::string _response;
		time_t _time;
		bool _startline_parsed;
		bool _headers_parsed;
		bool _send;
		bool _has_content_lenght;
		bool _has_transfer_enconding;
		size_t _transfers;

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
		void setUri(std::string uri);
		std::string getUri(void) const;
		void setPath(std::string path);
		std::string getPath(void) const;
		void setQueryString(std::string query_string);
		std::string getQueryString(void) const;
		void setProtocol(std::string protocol);
		std::string getProtocol(void) const;
		void setCode(std::string code);
		std::string getCode(void) const;
		void setStatus(std::string status);
		std::string getStatus(void) const;
		void addHeader(std::string key, std::string value);
		void addHeader(std::string key, size_t value);
		void setHeaders(std::map<std::string, std::string> headers);
		std::string getHeaderByKey(std::string key) const;
		std::string getHeaders(void) const;
		void setBody(std::string body);
		std::string getBody(void) const;
		void setFile(AFile *file);
		void setServer(Server server);
		Server &getServer(void);
		void setLocation(Location location);
		Location &getLocation(void);
		time_t getTime(void) const;
		void buildResponse(void);
		std::string getResponse(int bytes);
		std::string getResponse(void);
		size_t getResponseSize(void) const;
		void setStartLineParsed(bool value);
		bool getStartLineParsed(void) const;
		void setHeadersParsed(bool value);
		bool getHeadersParsed(void) const;
		bool hasContentLenght(void) const;
		bool hasTransferEnconding(void) const;
		void setSend(bool send);
		bool getSend(void) const;
		size_t getTransfers(void) const;
		void resetConnection(void);

		std::string operator[](std::string key);

};

std::ostream &operator<<(std::ostream &os, const Connection &src);

#endif /* CONNECTION_HPP */
