/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 19:13:23 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/29 15:21:53 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Http.hpp"
#include <list>
#include <map>
#include <ostream>

using namespace std;

class Location;

class Server {
	private:
		list<string> _name;
		list<string> _listen;
		string _root;
		set<string> _index;
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

		void setName(string name);
		list<string> getName(void) const;
		void setListen(string listen);
		list<string> getListen(void) const;
		void setRoot(string root);
		string getRoot(void) const;
		void setIndex(string index);
		set<string> getIndex(void) const;
		void setMaxBodySize(string size);
		size_t getMaxBodySize(void) const;
		void setErrorPage(string erro_page);
		string getErrorPage(string code) const;
		map<string, string> getErrorPages(void) const;
		void addLocation(Location location);
		Location getLocation(string uri) const;
		map<string, Location> getLocations(void) const;
		void setReturn(string value);
		string getReturnCode(void) const;
		string getReturnURI(void) const;
};

ostream &operator<<(ostream &os, const Server &src);

#endif /* SERVER_HPP */
