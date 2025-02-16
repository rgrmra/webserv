#include "logger.hpp"
#include "Mime.hpp"
#include "parser.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <stdexcept>

using namespace std;

Mime::Mime(void)
	: _default_mime("text/plain") {

	string filename = "./src/parsers/mimes.json";

	try {
		ifstream file(filename.c_str());
		if (not file.is_open())
			throw runtime_error("failed to open json file: " + filename);

		string buffer;

		for (string line; getline(file, line); buffer.append(line)) {

			if (line.find("#") != string::npos)
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
	} catch (exception &e) {
		throw runtime_error(string("failed to parse json at: ") + e.what());
	}

	logger::info("mimes json file parsed: " + filename);
}

Mime::Mime(const Mime &src)
	: _default_mime(src._default_mime) {

	*this = src;
}

Mime &Mime::operator=(const Mime &rhs) {

	if (this == &rhs)
		return *this;

	_mimes = rhs._mimes;

	return *this;
}

Mime::~Mime(void) {

}

void Mime::addMime(string &key, string &values) {

	list<string> tmp = parser::split(values, ',');

	for (list<string>::iterator it = tmp.begin(); it != tmp.end(); it++) {
		
		string value = parser::find("\"", *it, "\"");
		if (value.empty() || it->size())
			throw runtime_error("empty value at key: \"" + key + "\"");

		_mimes[value] = key;
	}
}

void Mime::parseMimes(string &buffer) {

	if (parser::compare("{", buffer))
		buffer.erase(0, 1);
	else
		throw runtime_error("unclosed json");

	size_t npos = buffer.find_last_of("}");
	if (npos != string::npos)
		buffer.erase(npos, 1);
	else
	 	throw runtime_error("unclosed json");

	if (buffer.empty())
		return;
	
	string key, value;

	for (size_t i = buffer.size(); i > 0; i--) {

		key = parser::find("\"", buffer, "\"");
		if (key.empty())
			throw runtime_error("empty key: \"" + key + "\"" + buffer);
		if (buffer.at(0) == ':')
			buffer.erase(0, 1);
		else
			throw runtime_error("missing \":\"");

		value = parser::find("[", buffer, "]");
		if (value.empty())
			throw runtime_error("empty value at key: \"" + key + "\"");
		if (buffer.empty() || buffer.at(0) == ',') {
			addMime(key, value);

			if (buffer.empty())
				return;

			buffer.erase(0, 1);
		} else {
			throw runtime_error("missing \",\" at: " + buffer);
		}
	}
}

string Mime::getType(string extension) const {

	size_t pos = extension.find_last_of(".");
	if (pos == string::npos)
		return _default_mime;

	extension.erase(0, pos + 1);

	map<string, string>::const_iterator it = _mimes.find(extension);
	if (it == _mimes.end())
		return _default_mime;

	return it->second;
}
