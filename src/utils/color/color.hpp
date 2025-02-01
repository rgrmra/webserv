#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>

namespace color {

	std::string red(void);
	std::string red(std::string message);
	std::string green(void);
	std::string green(std::string message);
	std::string yellow(void);
	std::string yellow(std::string message);
	std::string blue(void);
	std::string blue(std::string message);
	std::string magenta(void);
	std::string magenta(std::string message);
	std::string reset(void);

}

#endif /* COLOR_HPP */
