/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:25:30 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 14:48:52 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#ifndef LOGGER_DEBUG
#define LOGGER_DEBUG true
#endif /* LOGGER_DEBUG */

using namespace std;

namespace logger {

	void debug(string message);
	void info(string message);
	void warning(string message);
	void error(string message);
	void fatal(string message);

}

#endif /* LOGGER_HPP */
