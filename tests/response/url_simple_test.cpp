#include "gtest/gtest.h"
#include "URL.hpp"
#include "Connection.hpp"
#include "Location.hpp"
#include <string>
#include <iostream>

// Simple test fixture with a real Connection and Location (not mocks)
// This will help us debug the URL class without complications from mocking
class URLSimpleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a real Connection object for testing
        conn = new Connection(1, "127.0.0.1");
        
        // Set up the connection
        conn->setHost("localhost:8080");
        
        std::string method = "GET";
        conn->setMethod(method);
        
        conn->setPath("/test/path.html");
        
        // Create a Location
        Location loc;
        loc.setRoot("/var/www");
        
        // Set up indexes
        std::set<std::string> indexes;
        indexes.insert("index.html");
        loc.setIndexes(indexes);
        
        // Add the location to the connection
        conn->setLocation(loc);
        
        // Debug to see what Connection is returning
        std::cout << "Connection host: " << conn->getHost() << std::endl;
        std::cout << "Connection path: " << conn->getPath() << std::endl;
    }

    void TearDown() override {
        delete conn;
    }
    
    Connection* conn;
};

// Test the basic URL construction
TEST_F(URLSimpleTest, BasicURLConstruction) {
    // Just create a URL object and verify it doesn't crash
    URL url(conn);
    
    // Debug output
    std::cout << "URL host: " << url.getHost() << std::endl;
    std::cout << "URL port: " << url.getPort() << std::endl;
    std::cout << "URL path: " << url.getPath() << std::endl;
    std::cout << "URL file: " << url.getFile() << std::endl;
    
    // Basic assertions to make sure we're getting something sensible
    EXPECT_FALSE(url.getHost().empty());
    EXPECT_FALSE(url.getPort().empty());
} 