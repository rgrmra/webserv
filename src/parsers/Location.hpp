/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:25:37 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/21 17:01:08 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <cstddef>
#include <ostream>
#include <set>
#include <string>

using namespace std;

class Location {
	private:
		string _path;
		string _index;
		string _root;
		set<string> _allow_methods;
		bool _autoindex;
		size_t _max_body_size;
		string _return_code;
		string _return_path;

	public:
		Location(void);
		Location(string &configuration_file);
		Location(const Location &src);
		Location &operator=(const Location &rhs);
		~Location(void);

		void setPath(string path);
		string getPath(void) const;
		void setIndex(string index);
		string getIndex(void) const;
		void setRoot(string root);
		string getRoot(void) const;
		void addMethod(string method);
		set<string> getMethod(void) const;
		void setAutoIndex(string autoindex);
		bool getAutoIndex(void) const;
		void setMaxBodySize(string max_body_size);
		size_t getMaxBodySize(void) const;
		void setReturn(string value);
		string getReturnCode(void) const;
		string getReturnPath(void) const;
};

ostream &operator<<(ostream &os, const Location &src);

#endif /* LOCATION_HPP */
