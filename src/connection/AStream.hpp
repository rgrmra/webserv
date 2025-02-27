#ifndef ASTREAM_HPP
#define ASTREAM_HPP

#include "IStream.hpp"
#include <ctime>
#include <string>

class AStream : public IStream {
	
	protected:
		int _fd;
		std::string _id;
		std::string _input;
		std::string _output;
		std::time_t _time;

		AStream(int fd, std::string id);
		AStream(const AStream &src);
		AStream &opeartor(const AStream &rhs);
		
	public:
		virtual ~AStream(void);
		
		int getFd(void) const;
		std::string getId(void) const;

		virtual void setData(std::vector<char> &buffer, size_t bytes);
		virtual std::string getData(size_t bytes);
		virtual bool isTimedOut(void);

};

#endif /* ASTREAM_HPP */
