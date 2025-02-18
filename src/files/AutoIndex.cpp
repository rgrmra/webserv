#include "AutoIndex.hpp"
#include <iostream>

AutoIndex::AutoIndex(const std::string &path, const std::string &uri) : AFile(".html") {
	_path = path;
	_uri = uri;
	generateContent();
	_size = _content.size();
}

AutoIndex::AutoIndex(const AutoIndex &src) : AFile(src._path) {
	*this = src;
}

AutoIndex &AutoIndex::operator=(const AutoIndex &rhs) {
	if (this != &rhs) {
		_path = rhs._path;
		_content = rhs._content;
	}
	return *this;
}

AutoIndex::~AutoIndex(void) {}

bool AutoIndex::empty(void) const {
	return _content.empty();
}

std::string AutoIndex::getBuffer(size_t bytes) {
	if (_content.size() <= bytes) {
		std::string tmp = _content;
		_content.clear();
		return tmp;
	} else {
		std::string tmp = _content.substr(0, bytes);
		_content.erase(0, bytes);
		return tmp;
	}
}

void AutoIndex::generateContent(void) {
	DIR *dir;
	struct dirent *ent;
	std::string response;
	std::string partPath;
	std::string hasBar = _uri.find_last_of('/') == _uri.size() - 1 ? "" : "/";

	partPath = _uri + hasBar;
	response += std::string("<html>\n<head><title>Index of</title>"
		"<script src=\"https://cdn.tailwindcss.com\"></script>"
		"</head>\n"
		"<body class=\"bg-gray-100 text-gray-900 min-h-screen\">\n"
		"<div class=\"container mx-auto p-4\">\n"
		"<div class=\"bg-white text-gray-900 rounded-lg shadow-lg p-8 max-w-3xl w-full mx-auto mt-8\">\n"
		"<h1 class=\"text-3xl font-bold mb-4\">Index of</h1>\n"
		"<div class=\"space-y-2\">\n");

	std::vector<std::string> entries;

	if ((dir = opendir(_path.c_str())) != NULL) {
		std::cout << "Opened directory" << std::endl;
		while ((ent = readdir(dir)) != NULL) {
			std::string name = ent->d_name;
			std::cout << "Name: " << name << std::endl;
			// if (name == "." || name == "..")
			// 	continue;
			entries.push_back(name);
		}
		closedir(dir);
	} else {
		response += "<p>Unable to open directory</p>\n";
	}

	std::sort(entries.begin(), entries.end());

	typedef std::vector<std::string>::iterator vector_iterator;
	for (vector_iterator it = entries.begin(); it != entries.end(); ++it) {
		response += "<a href=\"" 
			+ partPath + *it + "\" class=\"text-blue-500 hover:underline text-lg block\">" 
			+ *it 
			+ "</a>\n";
	}

	response += "</div>\n</div>\n</div>\n</body>\n</html>\n";
	std::cout << "Response: " << response << std::endl;

	_content = response;
}