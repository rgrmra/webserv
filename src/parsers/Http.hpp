/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:37:35 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/26 20:37:44 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

#include <ostream>
#include <set>
#include <string>

using namespace std;

class Server;

class Http {
	private:
		size_t _max_body_size;
		string _access_log;
		string _error_log;
		string _root;
		set<Server> _servers;

	public:
		Http(string &configuration_file);
		Http(const Http &src);
		Http &operator=(const Http &rhs);
		~Http(void);

		void setMaxBodySize(string max_body_size);
		size_t getMaxBodySize(void) const;
		void setAcessLog(string access_log);
		string getAcessLog(void) const;
		void setErrorLog(string error_log);
		string getErrorLog(void) const;
		void setRoot(string root);
		string getRoot(void) const;
		void addServer(Server server);
		Server getServer(string host, string port) const;
		set<Server> getServers(void) const;
};

ostream &operator<<(ostream &os, const Http &src);

#endif /* HTTP_HPP */
