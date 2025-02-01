#ifndef HTTP_HPP
#define HTTP_HPP

#include "Server.hpp"
#include <bitset>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

class Http {
	private:
		std::string _access_log;
		std::string _error_log;
		std::string _root;
		std::bitset<2> _autoindex;
		std::size_t _max_body_size;
		std::set<std::string> _indexes;
		std::map<std::string, std::string> _error_pages;
		std::vector<Server> _servers;

	public:
		Http(std::string configuration_file);
		Http(const Http &src);
		Http &operator=(const Http &rhs);
		~Http(void);

		void setAccessLog(std::string access_log);
		std::string getAccessLog(void) const;
		void setErrorLog(std::string error_log);
		std::string getErrorLog(void) const;
		void setRoot(std::string root);
		std::string getRoot(void) const;
		void setAutoIndex(std::string autoindex);
		void setAutoIndex(std::bitset<2> autoindex);
		std::bitset<2> getAutoIndexBitSet(void) const;
		bool getAutoIndex(void) const;
		void setMaxBodySize(std::string max_body_size);
		std::size_t getMaxBodySize(void) const;
		void addIndex(std::string index);
		void setIndex(std::set<std::string> indexes);
		std::set<std::string> getIndexes(void) const;
		void addErrorPage(std::string error_page);
		void setErrorPages(std::map<std::string, std::string> error_pages);
		std::map<std::string, std::string> getErrorPages(void) const;
		void addServer(Server server);
		void setServers(std::vector<Server> servers);
		Server getServerByListen(std::string listen) const;
		Server getServerByName(std::string name) const;
		std::vector<Server> getServers(void) const;
		bool empty(void) const;
		void start(void);
};

std::ostream &operator<<(std::ostream &os, const Http &src);

#endif /* HTTP_HPP */
