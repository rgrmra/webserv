#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <bitset>
#include <map>
#include <set>
#include <string>

class Location {
	private:
		std::string _uri;
		std::set<std::string> _allow_methods;
		bool _deny_methods;
		std::string _root;
		std::bitset<2> _autoindex;
		std::size_t _max_body_size;
		std::set<std::string> _indexes;
		std::map<std::string, std::string> _error_pages;
		std::string _return_code;
		std::string _return_uri;

	public:
		Location(void);
		Location(std::string &configuration_file);
		Location(const Location &src);
		Location &operator=(const Location &rhs);
		~Location(void);

		void setURI(std::string path);
		std::string getURI(void) const;
		void addMethod(std::string method);
		void setMethods(std::set<std::string> methods);
		std::set<std::string> getMethods(void) const;
		void setDenyMethods(std::string deny_methods);
		bool getDenyMethods(void) const;
		void setRoot(std::string root);
		std::string getRoot(void) const;
		void setAutoIndex(std::string autoindex);
		void setAutoIndex(std::bitset<2> autoindex);
		std::bitset<2> getAutoIndexBitSet(void) const;
		bool getAutoIndex(void) const;
		void setMaxBodySize(std::string max_body_size);
		std::size_t getMaxBodySize(void) const;
		void addIndex(std::string index);
		void setIndexes(std::set<std::string> indexes);
		std::set<std::string> getIndexes(void) const;
		void addErrorPages(std::string error_page);
		void setErrorPages(std::map<std::string, std::string> error_pages);
		std::map<std::string, std::string> getErrorPages(void) const;
		std::string getErrorPageByCode(std::string code) const;
		void setReturn(std::string value);
		std::string getReturnCode(void) const;
		std::string getReturnURI(void) const;
		bool empty(void) const;
};

std::ostream &operator<<(std::ostream &os, const Location &src);

#endif /* LOCATION_HPP */
