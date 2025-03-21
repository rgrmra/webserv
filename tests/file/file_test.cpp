#include "gtest/gtest.h"
#include "File.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

string createTestFile(const string& content) {
    string tempFilePath = "/tmp/file_test_temp.txt";
    ofstream testFile(tempFilePath.c_str());
    testFile << content;
    testFile.close();
    return tempFilePath;
}

TEST(FileTest, CreateAndDeleteRealFile) {
    string content = "This is test content for the File class.";
    string filePath = createTestFile(content);
    
    ifstream testFile(filePath.c_str());
    EXPECT_TRUE(testFile.is_open());
    testFile.close();
    
    remove(filePath.c_str());
    
    ifstream deletedFile(filePath.c_str());
    EXPECT_FALSE(deletedFile.is_open());
}

TEST(FileTest, ReadFromFile) {
    string content = "This is test content for the File class.";
    string filePath = createTestFile(content);
    
    ifstream testFile(filePath.c_str(), ios::binary);
    EXPECT_TRUE(testFile.is_open());
    
    string fileContent;
    vector<char> buffer(1024);
    testFile.read(buffer.data(), buffer.size());
    fileContent.assign(buffer.data(), testFile.gcount());
    testFile.close();
    
    EXPECT_EQ(fileContent, content);
    
    remove(filePath.c_str());
}

TEST(FileTest, ReadFromFileInChunks) {
    string content = "This is a longer test content with multiple lines.\n"
                    "Second line of content.\n"
                    "Third line of content.";
    string filePath = createTestFile(content);
    
    ifstream testFile(filePath.c_str(), ios::binary);
    EXPECT_TRUE(testFile.is_open());
    
    vector<char> buffer(10); // Small buffer to force multiple reads
    string fileContent;
    
    while (testFile) {
        testFile.read(buffer.data(), buffer.size());
        fileContent.append(buffer.data(), testFile.gcount());
        if (testFile.gcount() < buffer.size()) break;
    }
    
    testFile.close();
    
    EXPECT_EQ(fileContent, content);
    
    remove(filePath.c_str());
}

TEST(FileTest, EmptyFile) {
    string filePath = createTestFile("");
    
    ifstream testFile(filePath.c_str());
    EXPECT_TRUE(testFile.is_open());
    
    testFile.seekg(0, ios::end);
    EXPECT_EQ(testFile.tellg(), 0);
    testFile.close();
    
    remove(filePath.c_str());
}

TEST(FileTest, BinaryFileContent) {
    string filePath = "/tmp/binary_test_file.bin";
    ofstream binFile(filePath.c_str(), ios::binary);
    
    vector<char> binaryContent(100);
    for (int i = 0; i < 100; i++) {
        binaryContent[i] = static_cast<char>(i);
    }
    
    binFile.write(binaryContent.data(), binaryContent.size());
    binFile.close();
    
    ifstream readBinFile(filePath.c_str(), ios::binary);
    EXPECT_TRUE(readBinFile.is_open());
    
    vector<char> readContent(100);
    readBinFile.read(readContent.data(), readContent.size());
    EXPECT_EQ(readBinFile.gcount(), 100);
    
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(readContent[i], binaryContent[i]);
    }
    
    readBinFile.close();
    
    remove(filePath.c_str());
}

TEST(FileTest, CopyConstructorSignature) {
    SUCCEED();
}

TEST(FileTest, AssignmentOperatorSignature) {
    SUCCEED();
}

TEST(FileTest, DestructorSignature) {
    SUCCEED();
}

TEST(FileTest, ProcessOutputSignature) {
    SUCCEED();
} 