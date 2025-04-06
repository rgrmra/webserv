#include "Text.hpp"

#include "Connection.hpp"
#include "Mime.hpp"

Text::Text(Connection *connection)
	: Resource(connection) {

	_type = Mime::getInstance()->getType(".txt");
	_output = connection->getLocation().getReturnURI();
	_size = _output.size();
	_step = IStream::CLOSE;
}

Text::Text(const Text &src)
	: Resource(src._connection) {

	*this = src;
}

Text &Text::operator=(const Text &rhs) {

	if (this == &rhs)
		return *this;

	return *this;
}

Text::~Text(void) {

}
