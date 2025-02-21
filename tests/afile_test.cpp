#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "AFile.hpp"
#include "Mime.hpp"
#include "Common.hpp"

Mime* mimes = NULL;

class TestFile : public AFile {
public:
	TestFile(const std::string &path) : AFile(path) {}
	virtual bool empty(void) const { return false; }
	virtual std::string getBuffer(size_t bytes) { return std::string(bytes, 'a'); }
};

namespace {
void create_mime_file(const std::string &filename) {
	std::ofstream ofs(filename.c_str());
	ofs << "{\"text/plain\":[\"txt\"],\"text/html\":[\"html\"]}";
	ofs.close();
}
}

class AFileTest : public ::testing::Test {
protected:
	std::string tmpMimeFile;
	virtual void SetUp() {
		tmpMimeFile = "test_mime.json";
		create_mime_file(tmpMimeFile);
		try {
			mimes = new Mime(tmpMimeFile);
		} catch (std::exception &e) {
			FAIL() << "Failed to create Mime object: " << e.what();
		}
	}
	virtual void TearDown() {
		delete mimes;
		mimes = NULL;
		std::remove(tmpMimeFile.c_str());
	}
};

TEST_F(AFileTest, GetMimeTxtExtensionReturnsTextPlain) {
	TestFile file("document.txt");
	EXPECT_EQ("text/plain", file.getMime());
}

TEST_F(AFileTest, GetMimeHtmlExtensionReturnsTextHtml) {
	TestFile file("index.html");
	EXPECT_EQ("text/html", file.getMime());
}

TEST_F(AFileTest, GetMimeNoExtensionReturnsDefault) {
	TestFile file("README");
	EXPECT_EQ("text/plain", file.getMime());
}

TEST_F(AFileTest, GetMimeUnknownExtensionReturnsDefault) {
	TestFile file("archive.zip");
	EXPECT_EQ("text/plain", file.getMime());
}
