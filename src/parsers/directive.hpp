/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:13:51 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/29 18:54:25 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "Server.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace directive {

	void setAcessLog(string access_log, string &_access_log);
	void setErrorLog(string error_log, string &_error_log);
	void addServer(Server server, vector<Server> &_servers);
	bool validateName(string name);
	void addName(string name, vector<string> &_name);
	bool validateHttpListen(string listen);
	bool validateHttpHost(string host);
	bool validateHttpPort(string port);
	void addListen(string listen, vector<string> &_listen);
	void setURI(string uri, string &_uri);
	void addIndex(string index, set<string> &_index);
	void setRoot(string root, string &_root);
	void setMaxBodySize(string max_body_size, size_t &_max_body_size);
	void addErrorPage(string error_page, map<string, string> &_error_pages);
	bool validateHttpMethod(string method);
	void addMethod(string method, set<string> &_allow_methods);
	void setDenyMethods(string deny_methods, bool &_deny_methods);
	void setAutoIndex(string autoindex, bool &_autoindex);
	bool validateHttpCode(string code);
	void setReturn(string value, string &_code, string &_uri);
	
}

#endif /* DIRECTIVE_HPP */
