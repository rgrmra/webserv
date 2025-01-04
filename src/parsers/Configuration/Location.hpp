/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:28:38 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/04 20:18:40 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <ostream>
#include <set>
#include <string>

using namespace std;

class Location {

	private:
		string _path;
		set<string> _methods;
		string _redirect;

	public:
		Location(void);
		Location(const Location &src);
		Location &operator=(const Location &rhs);
		~Location(void);

		void setPath(string path);
		string getPath(void) const;
		void addMethod(string methods);
		set<string> getMethods(void) const;
		void setRedirect(string redirect);
		string getRedirect(void) const;
};

ostream &operator<<(ostream &os, const Location &src);

#endif /* LOCATION_HPP */
