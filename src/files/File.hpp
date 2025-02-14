#ifndef FILE_HPP
#define FILE_HPP

#include "AFile.hpp"
#include <fstream>
#include <string>
#include <vector>

class File : public AFile {
	private:
		std::string _path;
		std::ifstream _file;

	public:
		File(std::string file_path);
		File(const File &src);
		File &operator=(const File &rhs);
		~File(void);

		std::string getBuffer(size_t bytes);

};

#endif /* FILE_HPP */
