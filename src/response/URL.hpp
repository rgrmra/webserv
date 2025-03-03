#ifndef URL_HPP
#define URL_HPP

#include <string>
#include <ostream>

class Connection;

class URL {
	private:
		Connection *_connection;
		std::string _scheme;
		std::string _host;
		std::string _port;
		std::string _path;
		std::string _file;
		std::string _extension;
		std::string _query;

		void convertCharacters(std::string &path);
		void formatPath(std::string path);
		void processPath(std::string path);

	public:
		URL(Connection *connection);
		URL(const URL &src);
		URL &operator=(const URL &rhs);
		~URL(void);

		std::string getScheme(void) const;
		std::string getHost(void) const;
		std::string getPort(void) const;
		std::string getPath(void) const;
		std::string getFile(void) const;
		std::string getExtension(void) const;
		std::string getQuery(void) const;
		std::string getAbsolutePath(void) const;
		std::string getLocation(void) const;

};

std::ostream &operator<<(std::ostream &os, const URL &src);

#endif /* URL_HPP */
