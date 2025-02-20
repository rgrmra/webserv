#ifndef URL_HPP
#define URL_HPP

#include <string>
#include <ostream>

class URL {
	private:
		std::string _scheme;
		std::string _user;
		std::string _password;
		std::string _host;
		std::string _port;
		std::string _path;
		std::string _param;
		std::string _query;

	public:
		URL(std::string url);
		URL(const URL &src);
		URL &operator=(const URL &rhs);
		~URL(void);

		std::string getScheme(void) const;
		void setScheme(std::string scheme);
		std::string getUser(void) const;
		void setUser(std::string User);
		std::string getPassword(void) const;
		void setPassword(std::string password);
		std::string getHost(void) const;
		void setHost(std::string host);
		std::string getPort(void) const;
		void setPort(std::string port);
		std::string getPath(void) const;
		void setPath(std::string path);
		std::string getParam(void) const;
		void setParam(std::string param);
		std::string getQuery(void) const;
		void setQuery(std::string query);

};

std::ostream &operator<<(std::ostream &os, const URL &src);

#endif /* URL_HPP */
