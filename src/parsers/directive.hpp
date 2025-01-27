/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:13:51 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/26 20:54:54 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <list>
#include <map>
#include <set>
#include <string>

using namespace std;

namespace directive {

	void setAcessLog(string access_log, string &_access_log);
	void setErrorLog(string error_log, string &_error_log);
	bool validateName(string name);
	void setName(string name, list<string> &_name);
	bool validateHttpListen(string listen);
	bool validateHttpHost(string host);
	bool validateHttpPort(string port);
	void setListen(string listen, string &_host, string &_port);
	void setURI(string uri, string &_uri);
	void setIndex(string index, set<string> &_index);
	void setRoot(string root, string &_root);
	void setMaxBodySize(string max_body_size, size_t &_max_body_size);
	void setErrorPage(string error_page, map<string, string> &_error_pages);
	bool validateHttpMethod(string method);
	void setMethods(string method, set<string> &_allow_methods);
	void setDenyMethods(string deny_methods, bool &_deny_methods);
	void setAutoIndex(string autoindex, bool &_autoindex);
	bool validateHttpCode(string code);
	void setReturn(string value, string &_code, string &_uri);
	
}

#endif /* DIRECTIVE_HPP */
