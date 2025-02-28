#ifndef ISTREAM_HPP
#define ISTREAM_HPP

#include <string>
#include <vector>

class IStream {

	public:
		enum {
			NONE = 0,
			STARTLINE = 1,
			HEADERS = 2,
			BODY = 4,
			RESPONSE = 8,
			CLOSE = 16
		};

		virtual ~IStream(void) {};

		virtual int getFd(void) const = 0;
		virtual std::string getId(void) const = 0;

		virtual void setData(std::vector<char> &buffer, size_t bytes) = 0;
		virtual std::string getData(size_t bytes) = 0;
		virtual size_t getSize(void) const = 0;
		virtual void setStep(int value) = 0;
		virtual int getStep(void) const = 0;
		virtual bool isTimedOut(void) const = 0;
};

#endif /* ISTREAM_HPP */
