#include "color.hpp"
#include <string>

std::string color::red(void)
{
	return "\001\033[0;91m";
}

std::string color::red(const std::string &message)
{
	return red() + message + color::reset();
}

std::string color::green(void)
{
	return "\001\033[0;92m";
}

std::string color::green(const std::string &message)
{
	return green() + message + color::reset();
}

std::string color::yellow(void)
{
	return "\001\033[0;93m";
}

std::string color::yellow(const std::string &message)
{
	return yellow() + message + color::reset();
}

std::string color::blue(void)
{
	return "\001\033[0;94m";
}

std::string color::blue(const std::string &message)
{
	return blue() + message + color::reset();
}

std::string color::magenta(void)
{
	return "\001\033[0;95m";
}

std::string color::magenta(const std::string &message)
{
	return magenta() + message + color::reset();
}

std::string color::reset(void)
{
	return "\001\033[0;m";
}
