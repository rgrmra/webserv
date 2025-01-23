/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:18:42 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/22 21:34:53 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "directive.hpp"
#include "parser.hpp"

list<string> directive::setReturn(string value) {

	list<string> tmp = parser::split(value);

	if (tmp.size() < 1 || tmp.size() > 2)
		throw runtime_error("invalid return: " + value);

	string code = tmp.front();
	string path = tmp.back();

	if (not parser::validadeHttpCode(code))
		throw runtime_error("invalid return code: " + code);

	return tmp;
}

