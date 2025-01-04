/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:53:01 by rde-mour          #+#    #+#             */
/*   Updated: 2024/12/17 18:14:52 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>

class Color : public std::string {
	public:

		static std::string red(void);
		static std::string red(std::string message);
		static std::string green(void);
		static std::string green(std::string message);
		static std::string yellow(void);
		static std::string yellow(std::string message);
		static std::string blue(void);
		static std::string blue(std::string message);
		static std::string magenta(void);
		static std::string magenta(std::string message);
		static std::string reset(void);

};

#endif /* COLOR_HPP */
