#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "AStream.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <ctime>
#include <map>
#include <string>

class Resource;
class Http;

class Connection : public AStream {
	private:
		std::string _host;
		std::string _method;
		std::string _uri;
		std::string _path;
		std::string _query_string;
		std::string _protocol;
		std::string _code;
		std::string _status;
		std::map<std::string, std::string> _headers;
		std::string _body;
		Resource *_file;
		Server _server;
		Location _location;
		bool _has_content_lenght;
		bool _has_transfer_enconding;

		void parseRequest(void);

	public:
		Connection(int fd, std::string _ip);
		Connection(const Connection &src);
		Connection &operator=(const Connection &rhs);
		virtual ~Connection(void);

		void setHost(std::string host);
		std::string getHost(void) const;
		void processInput(size_t bytes);
		std::string getInput(void) const;
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
		void setResource(Resource *file);
		void setServer(Server server);
		Server &getServer(void);
		void setLocation(Location location);
		Location &getLocation(void);
		void buildResponse(void);
		void processOutput(size_t bytes);
		bool hasContentLenght(void) const;
		bool hasTransferEnconding(void) const;
		void resetConnection(void);

		std::string operator[](std::string key);

};

std::ostream &operator<<(std::ostream &os, const Connection &src);

#endif /* CONNECTION_HPP */
