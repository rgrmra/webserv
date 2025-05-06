#ifndef ASTREAM_HPP
#define ASTREAM_HPP

#include "IStream.hpp"
#include <ctime>
#include <string>
#include <vector>

class AStream : public IStream {
	
	protected:
		int _fd;
		std::string _id;
		std::string _input;
		std::string _output;
		std::time_t _time;
		size_t _size;
		size_t _transfers;
		int _step;

		AStream(int fd, std::string id);
		AStream(const AStream &src);
		AStream &opeartor(const AStream &rhs);

		virtual void processInput(size_t bytes);
		virtual void processOutput(size_t bytes);
		
	public:
		virtual ~AStream(void);
		
		int getFd(void) const;
		std::string getId(void) const;

		virtual void setData(std::vector<char> &buffer, size_t bytes);
		virtual std::string getData(size_t bytes);
		virtual size_t getSize(void) const;
		virtual void setStep(int step);
		virtual int getStep(void) const;
		virtual bool isTimedOut(void) const;

};

#endif /* ASTREAM_HPP */
