#ifndef FILE_HPP
#define FILE_HPP

#include "AFile.hpp"
#include <fstream>
#include <string>

class File : public AFile {
	private:
		std::ifstream _file;

	public:
		File(std::string file_path);
		File(const File &src);
		File &operator=(const File &rhs);
		~File(void);

		virtual bool empty(void) const;
		std::string getBuffer(size_t bytes);

};

#endif /* FILE_HPP */
