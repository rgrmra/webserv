/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 17:30:08 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/04 20:46:23 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "Location.hpp"
#include <map>
#include <ostream>
#include <string>

using namespace std;

class Configuration {

	private:
		string _name;
		string _port;
		string _host;
		string _max_body_size;
		string _root;
		string _default_file;
		map<string, string> _error_page;
		map<string, Location> _location;

	public:
		Configuration(void);
		Configuration(const Configuration &src);
		Configuration &operator=(const Configuration &rhs);
		~Configuration(void);

		bool operator<(const Configuration &rhs) const;

		void setName(string name);
		string getName(void) const;
		void setPort(string port);
		string getPort(void) const;
		void setHost(string host);
		string getHost(void) const;
		void setMaxBodySize(string max_body_size);
		string getMaxBodySize(void) const;
		void setRoot(string root);
		string getRoot(void) const;
		void setDefaultFile(string default_file);
		string getDefaultFile(void) const;
		void addErrorPage(string code, string path);
		string getErrorPage(string code) const;
		map<string, string> getErrorPage(void) const;
		void addLocation(Location location);
		Location getLocation(string path) const;
		map<string, Location> getLocation(void) const;
};

std::ostream &operator<<(std::ostream &os, const Configuration &src);

#endif /* CONFIGURATION_HPP */
