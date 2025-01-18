/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:25:30 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/17 12:15:12 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include "Color.hpp"

#ifndef LOGGER_DEBUG
#define LOGGER_DEBUG true
#endif /* LOGGER_DEBUG */

using namespace std;

class Logger {
	private:
		Logger(void);
		Logger(const Logger &src);
		Logger &operator=(const Logger &rhs);
		~Logger(void);

		static string _getTime(void);
		static void _print(string color, string level, string message);

	public:
		static void debug(string message);
		static void info(string message);
		static void warning(string message);
		static void error(string message);
		static void fatal(string message);
};

#endif /* LOGGER_HPP */
