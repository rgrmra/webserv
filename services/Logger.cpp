/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 21:09:06 by rde-mour          #+#    #+#             */
/*   Updated: 2024/12/17 18:23:07 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include <ctime>
#include <iostream>
#include <string>

Logger::Logger(void) {

}

Logger::Logger(const Logger &src) {

	*this = src;
}

Logger &Logger::operator=(const Logger &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

Logger::~Logger(void) {

}

std::string Logger::getTime(void) {

	std::time_t rawtime;
	std::time(&rawtime);

	struct std::tm *timeinfo;
	timeinfo = localtime(&rawtime);

	char buffer[22];
	std::strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", timeinfo);

	return buffer;
}

void Logger::_print(std::string color, std::string level, std::string message) {
	std::cout
		<< color + getTime() + " " + level + ": " + message << Color::reset()
		<< std::endl;
}

void Logger::debug(std::string message) {
	_print(Color::blue(), "DEBUG", message);
}

void Logger::info(std::string message) {
	_print(Color::green(), "INFO", message);
}

void Logger::warning(std::string message) {
	_print(Color::yellow(), "WARNING", message);
}

void Logger::error(std::string message) {
	_print(Color::red(), "ERROR", message);
}

void Logger::fatal(std::string message) {
	_print(Color::magenta(), "FATAL", message);
}