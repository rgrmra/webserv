/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:53:01 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/22 10:22:28 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>

using namespace std;

class Color : public string {
	public:

		static string red(void);
		static string red(string message);
		static string green(void);
		static string green(string message);
		static string yellow(void);
		static string yellow(string message);
		static string blue(void);
		static string blue(string message);
		static string magenta(void);
		static string magenta(string message);
		static string reset(void);

};

#endif /* COLOR_HPP */
