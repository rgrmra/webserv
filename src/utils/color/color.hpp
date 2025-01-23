/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:53:01 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/23 14:50:53 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>

using namespace std;


namespace color {

	string red(void);
	string red(string message);
	string green(void);
	string green(string message);
	string yellow(void);
	string yellow(string message);
	string blue(void);
	string blue(string message);
	string magenta(void);
	string magenta(string message);
	string reset(void);

}

#endif /* COLOR_HPP */
