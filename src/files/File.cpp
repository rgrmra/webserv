#include "File.hpp"
#include "AFile.hpp"
#include <fstream>
#include <ios>

using namespace std;

File::File(std::string file_path)
	: AFile(),
	  _path(file_path) {

	_file.open(_path.c_str(), ios::binary);
	if (!_file.is_open())
		return;

	_file.seekg(0, ios::end);
	_size = _file.tellg();
	_file.seekg(0, ios::beg);
}

File::File(const File &src)
	: AFile() {

	*this = src;
}

File &File::operator=(const File &rhs) {

	if (this == &rhs)
		return *this;

	_path = rhs._path;
	_file.open(rhs._path.c_str(), ios::binary);
	_size = rhs._size;

	return *this;
}

File::~File(void) {

	if (_file.is_open())
		_file.close();
}

std::string File::getBuffer(size_t bytes) {

	if (!_file.is_open())
		return "";

	vector<char> buffer(bytes);
	_file.read(buffer.data(), bytes);

	string tmp = "";
	vector<char>::iterator it = buffer.begin();
	for (; it != buffer.begin() + _file.gcount(); it++)
		tmp += *it;

	_size -= (_size > bytes) ? bytes : _size;

	return tmp;
}
