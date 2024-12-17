/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:25:30 by rde-mour          #+#    #+#             */
/*   Updated: 2024/12/17 18:01:30 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include "Color.hpp"

class Logger {
	private:
		std::string getTime(void);
		
		void _print(std::string color, std::string level, std::string message);

	public:
		Logger(void);
		Logger(const Logger &src);
		Logger &operator=(const Logger &rhs);
		~Logger(void);

		void debug(std::string message);
		void info(std::string message);
		void warning(std::string message);
		void error(std::string message);
		void fatal(std::string message);
};

#endif /* LOGGER_HPP */
