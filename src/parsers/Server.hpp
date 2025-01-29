/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:13:23 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/29 18:57:05 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Http.hpp"
#include <map>
#include <set>
#include <ostream>
#include <vector>

using namespace std;

class Location;

class Server {
	private:
		vector<string> _names;
		vector<string> _listen;
		string _root;
		set<string> _indexes;
		size_t _max_body_size;
		map<string, string> _error_pages;
		map<string, Location> _locations;
		string _return_code;
		string _return_uri;

	public:
		Server(void);
		Server(string &configuration_file);
		Server(const Server &src);
		Server &operator=(const Server &rhs);
		~Server(void);

		bool operator<(const Server &rhs) const;

		void addName(string name);
		void setNames(vector<string> names);
		vector<string> getNames(void) const;
		void addListen(string listen);
		void setListen(vector<string> listen);
		vector<string> getListen(void) const;
		void setRoot(string root);
		string getRoot(void) const;
		void addIndex(string index);
		void setIndexes(set<string> indexes);
		set<string> getIndexes(void) const;
		void setMaxBodySize(string size);
		size_t getMaxBodySize(void) const;
		void addErrorPage(string error_page);
		void setErrorPages(map<string, string> error_pages);
		string getErrorPageByCode(string code) const;
		map<string, string> getErrorPages(void) const;
		void addLocation(Location location);
		void setLocations(map<string, Location> locations);
		Location getLocationByURI(string uri) const;
		map<string, Location> getLocations(void) const;
		void setReturn(string value);
		string getReturnCode(void) const;
		string getReturnURI(void) const;
};

ostream &operator<<(ostream &os, const Server &src);

#endif /* SERVER_HPP */
