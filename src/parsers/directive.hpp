/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:13:51 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 14:29:29 by rde-mour         ###   ########.org.br   */
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
	void setName(string name, list<string> &_name);
	void setListen(string listen, string &_host, string &_port);
	void setPath(string path, string &_path);
	void setIndex(string index, set<string> &_index);
	void setRoot(string root, string &_root);
	void setMaxBodySize(string max_body_size, size_t &_max_body_size);
	void addErrorPage(string error_page, map<string, string> &_error_pages);
	void addMethod(string method, set<string> &_allow_methods);
	void setAutoIndex(string autoindex, bool &_autoindex);
	void setReturn(string value, string &_code, string &_path);
	
}

#endif /* DIRECTIVE_HPP */
