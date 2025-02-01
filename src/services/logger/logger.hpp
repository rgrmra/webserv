#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#ifndef LOGGER_DEBUG
#define LOGGER_DEBUG true
#endif /* LOGGER_DEBUG */

namespace logger {

	void debug(std::string message);
	void info(std::string message);
	void warning(std::string message);
	void error(std::string message);
	void fatal(std::string message);

}

#endif /* LOGGER_HPP */
