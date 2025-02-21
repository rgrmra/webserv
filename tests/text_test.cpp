#include "gtest/gtest.h"
#include "Text.hpp"

TEST(TextTest, EmptyInstanceReturnsEmptyTrue) {
	Text text("");
	EXPECT_TRUE(text.empty());
	EXPECT_EQ(text.getBuffer(10), "");
}

TEST(TextTest, GetBufferLessThanSize) {
	std::string content = "Hello World";
	Text text(content);

	std::string part = text.getBuffer(5);
	EXPECT_EQ(part, "Hello");

	std::string part2 = text.getBuffer(20);
	EXPECT_EQ(part2, " World");
	EXPECT_TRUE(text.empty());
}

TEST(TextTest, GetBufferExactSize) {
	std::string content = "Test";
	Text text(content);

	std::string part = text.getBuffer(4);
	EXPECT_EQ(part, "Test");
	EXPECT_TRUE(text.empty());
}

TEST(TextTest, MultipleSequentialCalls) {
	std::string content = "1234567890";
	Text text(content);

	std::string part1 = text.getBuffer(3);
	EXPECT_EQ(part1, "123");

	std::string part2 = text.getBuffer(3);
	EXPECT_EQ(part2, "456");

	std::string part3 = text.getBuffer(3);
	EXPECT_EQ(part3, "789");

	std::string part4 = text.getBuffer(3);
	EXPECT_EQ(part4, "0");
	EXPECT_TRUE(text.empty());
}

TEST(TextTest, CopyConstructor) {
	std::string content = "CopyTest";
	Text text1(content);
	Text text2(text1);

	std::string part1 = text1.getBuffer(4);
	EXPECT_EQ(part1, "Copy");
	EXPECT_EQ(text1.getBuffer(4), "Test");
	EXPECT_TRUE(text1.empty());
	EXPECT_EQ(text2.getBuffer(8), "CopyTest");
	EXPECT_TRUE(text2.empty());
}

TEST(TextTest, AssignmentOperator) {
	std::string content = "AssignOp";
	Text text1(content);
	Text text2("");
	text2 = text1;
	EXPECT_EQ(text2.getBuffer(8), "AssignOp");
	EXPECT_TRUE(text2.empty());
}
