#include "gtest/gtest.h"
#include "Request.hpp"
#include "Connection.hpp"
#include "response.hpp"

using namespace std;

TEST(RequestTest, ParseRequest_ValidRequest) {
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.1";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getMethod(), "GET");
	EXPECT_EQ(connection.getPath(), "/index.html");
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1");
}

TEST(RequestTest, ParseRequest_InvalidSpacing) {
	Connection connection(5, "127.0.0.1");
	string line = "        GET    /index.html     HTTP/1.1   ";
	request::parseRequest(&connection, line);
	EXPECT_NE(connection.getMethod(), "GET") << "Method should not be GET";
	EXPECT_NE(connection.getPath(), "/index.html") << "Path should not be /index.html";
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1") << "Protocol should not be HTTP/1.1";
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 200";
}

TEST(RequestTest, ParseRequest_InvalidSpacingDuplicate) {
	Connection connection(5, "127.0.0.1");
	string line = "        GET    /index.html     HTTP/1.1   ";
	request::parseRequest(&connection, line);
	EXPECT_NE(connection.getMethod(), "GET") << "Method should not be GET";
	EXPECT_NE(connection.getPath(), "/index.html") << "Path should not be /index.html";
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1") << "Protocol should not be HTTP/1.1";
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 200";
}

TEST(RequestTest, ParseRequest_MissingPath) {
	Connection connection(5, "127.0.0.1");
	string line = "GET HTTP/1.1";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getMethod(), "") << "Method should be GET";
	EXPECT_EQ(connection.getPath(), "") << "Path should be empty";
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1") << "Protocol should be HTTP/1.1";
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 400";
}

TEST(RequestTest, ParseRequest_InvalidMethod) {
	Connection connection(5, "127.0.0.1");
	string line = "PATCH /index.html HTTP/1.1";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 400";
}

TEST(RequestTest, ParseRequest_InvalidMethodDuplicate) {
	Connection connection(5, "127.0.0.1");
	string line = "PATCH /index.html HTTP/1.1";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 400";
}

TEST(RequestTest, ParseRequest_InvalidProtocol) {
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.2";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getCode(), "505") << "Code should be 400";
}

TEST(RequestTest, ParseRequest_HeadersParsed) {
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.1\r";
	request::parseRequest(&connection, line);
	line = "\r";
	request::parseRequest(&connection, line);
	EXPECT_TRUE(connection.getHeadersParsed()) << "Headers should be parsed";
}

TEST(RequestTest, ParseRequest_HeadersNotParsed) {
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.1";
	request::parseRequest(&connection, line);
	EXPECT_FALSE(connection.getHeadersParsed()) << "Headers should not be parsed";
}
