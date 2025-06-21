#include "logger.hpp"
#include "color.hpp"
#include <ctime>
#include <iostream>
#include <string>

static const std::string getTime(void)
{
	time_t rawtime = time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);

	char buffer[22];
	strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", timeinfo);

	return buffer;
}

static void print(const std::string &color, const std::string &level, const std::string &message)
{
	std::cout << color + getTime() + " " + level + ": " + message << color::reset() << std::endl;
}

void logger::debug(const std::string &message)
{
	if (not LOGGER_DEBUG)
		return;

	print(color::blue(), "DEBUG", message);
}

void logger::info(const std::string &message)
{
	print(color::green(), "INFO", message);
}

void logger::warning(const std::string &message)
{
	print(color::yellow(), "WARNING", message);
}

void logger::error(const std::string &message)
{
	print(color::red(), "ERROR", message);
}

void logger::fatal(const std::string &message)
{
	print(color::magenta(), "FATAL", message);
}
