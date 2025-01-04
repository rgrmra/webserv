/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:55:28 by rde-mour          #+#    #+#             */
/*   Updated: 2024/12/17 18:26:04 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Color.hpp"
#include <string>

std::string Color::red(void) {
	return "\001\033[0;91m";
}

std::string Color::red(std::string message) {
	return red() + message + Color::reset();
}

std::string Color::green(void) {
	return "\001\033[0;92m";
}

std::string Color::green(std::string message) {
	return green() + message + Color::reset();
}

std::string Color::yellow(void) {
	return "\001\033[0;93m";
}

std::string Color::yellow(std::string message) {
	return yellow() + message + Color::reset();
}

std::string Color::blue(void) {
	return "\001\033[0;94m";
}

std::string Color::blue(std::string message) {
	return blue() + message + Color::reset();
}

std::string Color::magenta(void) {
	return "\001\033[0;95m";
}

std::string Color::magenta(std::string message) {
	return magenta() + message + Color::reset();
}

std::string Color::reset(void) {
	return "\001\033[0;m";
}
