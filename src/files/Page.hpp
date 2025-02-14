#ifndef PAGE_HPP
#define PAGE_HPP

#include "code.hpp"
#include "AFile.hpp"

class Connection;

class Page : public AFile {
	private:
		std::string _page;
		std::string _buffer;

	public:
		Page(const std::string code, const std::string status);
		Page(const Page &src);
		Page &operator=(const Page &rhs);
		~Page(void);

		std::string getBuffer(size_t bytes);

};

#endif /* PAGE_HPP */
