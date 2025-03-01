#include "Connection.hpp"
#include "File.hpp"
#include <iostream>

using namespace std;

File::File(Connection *connection)
	: Resource(connection) {

	cout << "path: " <<  _id.c_str() << endl;
	_file.open(_id.c_str(), ios::binary);
	if (!_file.is_open())
		return;

	_file.seekg(0, ios::end);
	_size = _file.tellg();
	_file.seekg(0, ios::beg);

	_connection->buildResponse();
	_connection->setStep(IStream::RESPONSE);
	_step = CLOSE;
}

File::File(const File &src)
	: Resource(src._connection){

	*this = src;
}

File &File::operator=(const File &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

File::~File(void) {

}

void File::processOutput(size_t bytes) {

	if (!_file.is_open())
		return;

	vector<char> buffer(bytes);
	_file.read(buffer.data(), bytes);

	string tmp = "";
	vector<char>::iterator it = buffer.begin();
	for (; it != buffer.begin() + _file.gcount(); it++)
		tmp += *it;

	_output.append(tmp);
}
