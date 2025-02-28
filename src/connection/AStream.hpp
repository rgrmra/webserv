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
		std::time_t _time;
		size_t _size;
		int _step;

		AStream(int fd, std::string id);
		AStream(const AStream &src);
		AStream &opeartor(const AStream &rhs);
		
	public:
		virtual ~AStream(void);
		
		int getFd(void) const;
		std::string getId(void) const;

		virtual void setData(std::vector<char> &buffer, size_t bytes);
		virtual std::string getData(size_t bytes);
		virtual size_t getSize(void) const;
		virtual std::string getMime(void) const;
		virtual void setStep(int values);
		virtual int getStep(void) const;
		virtual bool isTimedOut(void) const;

};

#endif /* ASTREAM_HPP */
