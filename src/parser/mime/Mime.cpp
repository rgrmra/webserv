#include "Mime.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "standard.hpp"
#include <exception>
#include <fstream>
#include <list>
#include <map>
#include <string>

Mime *Mime::_instance = NULL;

Mime::Mime(void) : _default_mime(standard::MIME) {}

Mime::~Mime(void) {}

Mime *Mime::getInstance(void)
{
	if (_instance == NULL)
		_instance = new Mime();

	return _instance;
}

void Mime::configure(const std::string &filename)
{
	try
	{
		_mimes.clear();

		std::ifstream file(filename.c_str());
		if (not file.is_open())
			throw std::runtime_error("failed to open json file: " + filename);

		std::string buffer;

		for (std::string line; getline(file, line); buffer.append(line))
		{
			if (line.find("#") != std::string::npos)
				line = line.substr(0, line.find_first_of("#"));

			parser::trim(line, " \n\t\r\v\f");
		}

		file.close();

		parser::replace(buffer, '\t', ' ');

		parser::erase(buffer, "  ", 1);
		parser::erase(buffer, " [", 1);
		parser::rerase(buffer, "] ", 1);
		parser::erase(buffer, " ]", 1);
		parser::rerase(buffer, "[ ", 1);
		parser::erase(buffer, " {", 1);
		parser::rerase(buffer, "{ ", 1);
		parser::erase(buffer, " }", 1);
		parser::rerase(buffer, "} ", 1);
		parser::erase(buffer, " ,", 1);
		parser::rerase(buffer, ", ", 1);

		parseMimes(buffer);
	}
	catch (std::exception &e)
	{
		throw std::runtime_error(std::string("failed to parse json at: ") + e.what());
	}

	logger::info("mimes json file parsed: " + filename);
}

void Mime::addMime(const std::string &key, const std::string &values) {

	std::list<std::string> extensions = parser::split(values, ',');

	std::list<std::string>::iterator extension = extensions.begin();
	for (; extension != extensions.end(); ++extension)
	{
		std::string value = parser::find("\"", *extension, "\"");
		if (value.empty() || extension->size())
			throw std::runtime_error("empty value at key: \"" + key + "\"");

		_mimes[value] = key;
	}
}

void Mime::parseMimes(std::string &buffer)
{
	if (parser::compare("{", buffer))
		buffer.erase(0, 1);
	else
		throw std::runtime_error("unclosed json");

	size_t npos = buffer.find_last_of("}");
	if (npos != std::string::npos)
		buffer.erase(npos, 1);
	else
	 	throw std::runtime_error("unclosed json");

	if (buffer.empty())
		return;
	
	std::string key, value;

	for (size_t i = buffer.size(); i > 0; --i)
	{

		key = parser::find("\"", buffer, "\"");
		if (key.empty())
			throw std::runtime_error("empty key: \"" + key + "\"" + buffer);
		if (buffer.at(0) == ':')
			buffer.erase(0, 1);
		else
			throw std::runtime_error("missing \":\"");

		value = parser::find("[", buffer, "]");
		if (value.empty())
			throw std::runtime_error("empty value at key: \"" + key + "\"");
		if (buffer.empty() || buffer.at(0) == ',')
		{
			addMime(key, value);

			if (buffer.empty())
				return;

			buffer.erase(0, 1);
			continue;
		}
		throw std::runtime_error("missing \",\" at: " + buffer);
	}
}

std::string Mime::getType(std::string extension) const
{
	size_t pos = extension.find_last_of(".");
	if (pos == std::string::npos)
		return _default_mime;

	extension.erase(0, pos + 1);

	std::map<std::string, std::string>::const_iterator mime = _mimes.find(extension);
	if (mime == _mimes.end())
		return _default_mime;

	return mime->second;
}
