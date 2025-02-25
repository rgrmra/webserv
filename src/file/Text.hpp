#ifndef TEXT_HPP
#define TEXT_HPP

#include "AFile.hpp"
#include <string>

class Text : public AFile {
	private:
		std::string _buffer;

	public:
		Text(std::string file_path);
		Text(const Text &src);
		Text &operator=(const Text &rhs);
		~Text(void);

		virtual bool empty(void) const;
		std::string getBuffer(size_t bytes);

};

#endif /* TEXT_HPP */

