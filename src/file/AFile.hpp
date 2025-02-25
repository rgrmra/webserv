#ifndef AFILE_HPP
#define AFILE_HPP

#include <string>

class AFile {
	protected:
		std::string _path;
		size_t _size;

	public:
		AFile(std::string path);
		AFile(const AFile &src);
		AFile &operator=(const AFile &rhs);
		virtual ~AFile(void);

		virtual bool empty(void) const = 0;
		virtual size_t getSize(void) const;
		virtual std::string getBuffer(size_t bytes) = 0;
		std::string getMime(void) const;

};

#endif /* AFILE_HPP */
