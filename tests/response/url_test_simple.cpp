#include "gtest/gtest.h"
#include "URL.hpp"
#include "Connection.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>
#include <iostream>

class URLTestSimple : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up the server
        server = new Server();
        
        // Set up the location
        Location loc;
        loc.setRoot("/var/www");
        loc.setURI("/");
        
        std::set<std::string> indexes;
        indexes.insert("index.html");
        loc.setIndexes(indexes);
        
        server->addLocation(loc);
        
        // Create a real Connection object for testing
        conn = new Connection(1, "127.0.0.1");
        conn->setHost("localhost:8080");
        
        std::string method = "GET";
        conn->setMethod(method);
        conn->setPath("/test/path.html");
        conn->setServer(*server);
        conn->setLocation(loc);
    }

    void TearDown() override {
        delete conn;
        delete server;
    }
    
    Connection* conn;
    Server* server;
};

// Basic URL tests that ensure core functionality works
TEST_F(URLTestSimple, BasicURLParsing) {
    URL url(conn);
    
    EXPECT_EQ(url.getScheme(), "http");
    EXPECT_EQ(url.getHost(), "localhost");
    EXPECT_EQ(url.getPort(), "8080");
    EXPECT_EQ(url.getPath(), "/test/path.html");
    EXPECT_EQ(url.getFile(), "path.html");
    EXPECT_EQ(url.getExtension(), ".html");
}

// Test path formatting
TEST_F(URLTestSimple, PathFormatting) {
    conn->setPath("/path/./to/../file.txt");
    URL url(conn);
    
    EXPECT_EQ(url.getPath(), "/path/file.txt");
}

// Test query parameters
TEST_F(URLTestSimple, QueryParameters) {
    conn->setPath("/search.php?q=test&page=1");
    URL url(conn);
    
    EXPECT_EQ(url.getPath(), "/search.php");
    EXPECT_EQ(url.getQuery(), "q=test&page=1");
} 