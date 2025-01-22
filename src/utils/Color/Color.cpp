/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:55:28 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/22 10:25:06 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "Color.hpp"
#include <string>

using namespace std;

string Color::red(void) {
	return "\001\033[0;91m";
}

string Color::red(string message) {
	return red() + message + Color::reset();
}

string Color::green(void) {
	return "\001\033[0;92m";
}

string Color::green(string message) {
	return green() + message + Color::reset();
}

string Color::yellow(void) {
	return "\001\033[0;93m";
}

string Color::yellow(string message) {
	return yellow() + message + Color::reset();
}

string Color::blue(void) {
	return "\001\033[0;94m";
}

string Color::blue(string message) {
	return blue() + message + Color::reset();
}

string Color::magenta(void) {
	return "\001\033[0;95m";
}

string Color::magenta(string message) {
	return magenta() + message + Color::reset();
}

string Color::reset(void) {
	return "\001\033[0;m";
}
