/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 21:09:06 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 14:48:46 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "logger.hpp"
#include "color.hpp"
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

static std::string getTime(void) {

	std::time_t rawtime;
	std::time(&rawtime);

	struct std::tm *timeinfo;
	timeinfo = localtime(&rawtime);

	char buffer[22];
	std::strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", timeinfo);

	return buffer;
}

static void print(string color, string level, string message) {
	cout << color + getTime() + " " + level + ": " + message << color::reset() << endl;
}

void logger::debug(string message) {
	if (not LOGGER_DEBUG)
		return;

	print(color::blue(), "DEBUG", message);
}

void logger::info(string message) {
	print(color::green(), "INFO", message);
}

void logger::warning(string message) {
	print(color::yellow(), "WARNING", message);
}

void logger::error(string message) {
	print(color::red(), "ERROR", message);
}

void logger::fatal(string message) {
	print(color::magenta(), "FATAL", message);
}
