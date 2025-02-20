#ifndef URL_HPP
#define URL_HPP

#include <string>
#include <ostream>

class URL {
	private:
		std::string _scheme;
		std::string _host;
		std::string _port;
		std::string _path;
		std::string _last;
		std::string _query;

	public:
		URL(void);
		URL(std::string url);
		URL(const URL &src);
		URL &operator=(const URL &rhs);
		~URL(void);

		void setScheme(std::string scheme);
		std::string getScheme(void) const;
		void setHost(std::string host);
		std::string getHost(void) const;
		void setPort(std::string port);
		std::string getPort(void) const;
		void setPath(std::string path);
		std::string getPath(void) const;
		void setQuery(std::string query);
		std::string getQuery(void) const;
		std::string getLocation(void);
		void clear(void);

};

std::ostream &operator<<(std::ostream &os, const URL &src);

#endif /* URL_HPP */
