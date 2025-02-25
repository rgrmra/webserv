#include "logger.hpp"
#include "color.hpp"
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

static std::string getTime(void) {

	time_t rawtime;
	time(&rawtime);

	struct tm *timeinfo;
	timeinfo = localtime(&rawtime);

	char buffer[22];
	strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", timeinfo);

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
