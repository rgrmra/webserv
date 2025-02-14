//
// Created by vini on 2/1/25.
//

#include "directive.hpp"
#include "parser.hpp"
#include <gtest/gtest.h>
#include <set>
#include <stdexcept>
#include <bitset>
#include <map>
#include <limits>
#include <vector>

using namespace std;

// Test suite for the setAcessLog function
TEST(DirectiveTest, SetAcessLog) {
  string result;

  // Test with a valid access log
  EXPECT_NO_THROW(directive::setAcessLog("valid_access_log", result));
  EXPECT_EQ(result, "valid_access_log");

  // Test with an empty access log
  result.clear();
  EXPECT_NO_THROW(directive::setAcessLog("", result));
  EXPECT_TRUE(result.empty()); // Ensure result is not modified

  // Test with an access log containing spaces
  EXPECT_THROW(directive::setAcessLog("invalid access log", result),
      runtime_error);
  EXPECT_TRUE(result.empty()); // Ensure result is not modified on error

  // Test with an access log containing only spaces
  EXPECT_THROW(directive::setAcessLog("   ", result), runtime_error);
  EXPECT_TRUE(result.empty()); // Ensure result is not modified on error

  // Test with an access log containing leading/trailing spaces
  EXPECT_THROW(directive::setAcessLog("  leading_trailing  ", result),
      runtime_error);
  EXPECT_TRUE(result.empty()); // Ensure result is not modified on error
}

// Test suite for the setErrorLog function
TEST(DirectiveTest, SetErrorLog) {
  string result;

  // Test with a valid error log
  EXPECT_NO_THROW(directive::setErrorLog("valid_error_log", result));
  EXPECT_EQ(result, "valid_error_log");

  // Test with an empty error log
  result.clear();
  EXPECT_NO_THROW(directive::setErrorLog("", result));
  EXPECT_TRUE(result.empty());

  // Test with an error log containing spaces
  EXPECT_THROW(directive::setErrorLog("invalid error log", result),
      runtime_error);
  EXPECT_TRUE(result.empty()); // Ensure result is not modified on error

  // Test with an error log containing only spaces
  EXPECT_THROW(directive::setErrorLog("   ", result), runtime_error);
  EXPECT_TRUE(result.empty()); // Ensure result is not modified on error

  // Test with an error log containing leading/trailing spaces
  EXPECT_THROW(directive::setErrorLog("  leading_trailing  ", result),
      runtime_error);
  EXPECT_TRUE(result.empty()); // Ensure result is not modified on error
}

// Test suite for the ValidateHttpListen function
TEST(DirectiveTest, ValidateHttpListen) {
  // Test with an empty string
  EXPECT_FALSE(directive::validateHttpListen(""));

  // Test with valid IPv4 address
  EXPECT_TRUE(directive::validateHttpListen("127.0.0.1:8080"));
  EXPECT_TRUE(directive::validateHttpListen("192.168.1.1:80"));

  // Test with valid IPv6 address
  EXPECT_FALSE(directive::validateHttpListen("[::1]:8080"));
  EXPECT_FALSE(directive::validateHttpListen(
        "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:80"));

  // Test with invalid characters
  EXPECT_FALSE(
      directive::validateHttpListen("127.0.0.1:abc")); // Non-numeric port
  EXPECT_FALSE(directive::validateHttpListen(
        "127.0.0.1:8080a")); // Invalid character at the end
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:8080:")); // Extra colon
  EXPECT_FALSE(
      directive::validateHttpListen("127.0.0.1:8080 ")); // Space at the end

  // Test with leading or trailing colon
  EXPECT_FALSE(directive::validateHttpListen(":8080"));      // Leading colon
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:")); // Trailing colon

  // Test with double colon (invalid)
  EXPECT_FALSE(
      directive::validateHttpListen("127.0.0.1::8080")); // Double colon

  // Test with more than two parts after splitting by colon
  EXPECT_FALSE(directive::validateHttpListen(
        "127.0.0.1:8080:extra")); // More than two parts

  // Test with valid port-only input
  EXPECT_TRUE(directive::validateHttpListen("8080")); // Port only
  EXPECT_FALSE(directive::validateHttpListen(
        ":8080")); // Port only with leading colon (invalid in your function)
  EXPECT_FALSE(
      directive::validateHttpListen("8080:")); // Port only with trailing colon

  // Test with invalid IPv6 format
  EXPECT_FALSE(
      directive::validateHttpListen("[::1]::8080")); // Double colon in IPv6
  EXPECT_FALSE(
      directive::validateHttpListen("[::1]:8080:extra")); // More than two parts
}

// Test suite for the validateHttpHost function
TEST(DirectiveTest, ValidateHttpHost) {
  // Test with valid IPv4 addresses
  EXPECT_TRUE(directive::validateHttpHost(string("192.168.1.1")));
  EXPECT_TRUE(directive::validateHttpHost(string("10.0.0.1")));
  EXPECT_TRUE(directive::validateHttpHost(string("255.255.255.255")));

  // Test with invalid characters
  EXPECT_FALSE(directive::validateHttpHost(
        string("192.168.1.a"))); // Contains non-numeric character
  EXPECT_FALSE(
      directive::validateHttpHost(string("192.168.1.1 "))); // Contains space
  EXPECT_FALSE(
      directive::validateHttpHost(string("192.168.1.1."))); // Ends with a dot

  // Test with consecutive dots
  EXPECT_FALSE(directive::validateHttpHost(
        string("192..168.1.1"))); // Contains consecutive dots

  // Test with leading or trailing dots
  EXPECT_FALSE(
      directive::validateHttpHost(string(".192.168.1.1"))); // Starts with a dot
  EXPECT_FALSE(
      directive::validateHttpHost(string("192.168.1.1."))); // Ends with a dot

  // Test with incorrect number of octets
  EXPECT_FALSE(
      directive::validateHttpHost(string("192.168.1"))); // Only 3 octets
  EXPECT_FALSE(
      directive::validateHttpHost(string("192.168.1.1.1"))); // 5 octets

  // Test with octets greater than 255
  EXPECT_FALSE(
      directive::validateHttpHost(string("256.168.1.1"))); // First octet > 255
  EXPECT_FALSE(directive::validateHttpHost(
        string("192.168.256.1"))); // Third octet > 255
  EXPECT_FALSE(directive::validateHttpHost(
        string("192.168.1.256"))); // Fourth octet > 255

  // Test with empty string
  EXPECT_FALSE(directive::validateHttpHost(string(""))); // Empty string
}

// Test suite for the validateHttpPort function
TEST(DirectiveTest, ValidateHttpPort) {
  // Test with valid ports
  EXPECT_TRUE(directive::validateHttpPort(string("8080"))); // Valid port
  EXPECT_TRUE(directive::validateHttpPort(string("80")));   // Valid port
  EXPECT_TRUE(
      directive::validateHttpPort(string("65535"))); // Maximum valid port

  // Test with invalid ports
  EXPECT_FALSE(directive::validateHttpPort(string("")));        // Empty string
  EXPECT_FALSE(directive::validateHttpPort(string("65536")));   // Port > 65535
  EXPECT_FALSE(directive::validateHttpPort(string("123456")));  // Port > 65535
  EXPECT_FALSE(directive::validateHttpPort(string("-123456"))); // Port < 0

  EXPECT_FALSE(directive::validateHttpPort(string(""))); // Port < 0

  // Test with non-numeric characters
  EXPECT_FALSE(directive::validateHttpPort(
        string("8080a"))); // Contains non-numeric character
  EXPECT_FALSE(directive::validateHttpPort(string("80 80"))); // Contains space
  EXPECT_FALSE(directive::validateHttpPort(string("80.80"))); // Contains dot

  // Test with leading/trailing whitespace (invalid)
  EXPECT_FALSE(
      directive::validateHttpPort(string(" 8080 "))); // Leading/trailing spaces
  EXPECT_FALSE(
      directive::validateHttpPort(string("\t8080\t"))); // Leading/trailing tabs
}

// Test suite for the addListen function
TEST(DirectiveTest, AddListen) {
  vector<string> listenList;

  // Test with valid listen string (host:port)
  EXPECT_NO_THROW(directive::addListen(string("192.168.1.1:8080"), listenList));
  EXPECT_EQ(listenList.back(), "192.168.1.1:8080");

  // Test with valid listen string (port only)
  EXPECT_NO_THROW(directive::addListen(string("8080"), listenList));
  EXPECT_EQ(listenList.back(), "0.0.0.0:8080");

  // Test with valid listen string (host only)
  EXPECT_NO_THROW(directive::addListen(string("192.168.1.1"), listenList));
  EXPECT_EQ(listenList.back(), "192.168.1.1:80");

  // Test with empty listen string
  EXPECT_NO_THROW(
      directive::addListen(string(""), listenList)); // Should do nothing
  EXPECT_EQ(listenList.size(), 3);                   // No change in size

  // Test with invalid listen string (invalid characters)
  EXPECT_THROW(directive::addListen(string("192.168.1.1:8080a"), listenList),
      runtime_error);

  // Test with invalid listen string (invalid host)
  EXPECT_THROW(directive::addListen(string("256.256.256.256:8080"), listenList),
      runtime_error);

  // Test with invalid listen string (invalid port)
  EXPECT_THROW(directive::addListen(string("192.168.1.1:65536"), listenList),
      runtime_error);

  // Test with duplicate listen string
  EXPECT_THROW(directive::addListen(string("192.168.1.1:8080"), listenList),
      runtime_error); // Already added
}

TEST(DirectiveTest, ValidateName) {
  // Test with valid names
  EXPECT_TRUE(directive::validateName(string("example"))); // Alphanumeric
  EXPECT_TRUE(directive::validateName(
        string("example123"))); // Alphanumeric with numbers
  EXPECT_TRUE(directive::validateName(
        string("example-name"))); // Alphanumeric with hyphen
  EXPECT_TRUE(
      directive::validateName(string("example.name"))); // Alphanumeric with dot

  // Test with invalid names
  EXPECT_FALSE(directive::validateName(string(""))); // Empty string
  EXPECT_FALSE(
      directive::validateName(string("example name"))); // Contains space
  EXPECT_FALSE(directive::validateName(
        string("example@name"))); // Contains special character
  EXPECT_FALSE(directive::validateName(
        string("example!name"))); // Contains special character
  EXPECT_FALSE(
      directive::validateName(string("example_name"))); // Contains underscore
  EXPECT_FALSE(directive::validateName(
        string("example#name"))); // Contains special character
  EXPECT_FALSE(directive::validateName(
        string("example!name"))); // Contains special character

  // Test with leading/trailing invalid characters
  EXPECT_FALSE(
      directive::validateName(string("-example"))); // Starts with hyphen
  EXPECT_FALSE(directive::validateName(string(".example"))); // Starts with dot
  EXPECT_FALSE(directive::validateName(string("example-"))); // Ends with hyphen
  EXPECT_FALSE(directive::validateName(string("example."))); // Ends with dot

  // Test with mixed valid and invalid characters
  EXPECT_FALSE(directive::validateName(
        string("example-name@domain"))); // Contains special character
  EXPECT_FALSE(directive::validateName(
        string("example.name with space"))); // Contains space
}

TEST(DirectiveTest, AddName) {
  vector<string> nameList;

  // Test empty name
  directive::addName("", nameList);
  EXPECT_TRUE(nameList.empty()) << "Expected empty vector for empty input";

  // Test valid single name
  directive::addName("example", nameList);
  ASSERT_EQ(nameList.size(), 1) << "Expected one element";
  EXPECT_EQ(nameList[0], "example") << "Expected 'example'";

  // Test valid multiple names
  directive::addName("example.com my-site 123server", nameList);
  ASSERT_EQ(nameList.size(), 4) << "Expected 4 elements";
  EXPECT_EQ(nameList[1], "example.com");
  EXPECT_EQ(nameList[2], "my-site");
  EXPECT_EQ(nameList[3], "123server");

  // Test invalid name throwing exception
  EXPECT_THROW(directive::addName("validname @invalid! anothervalid", nameList),
      runtime_error)
    << "Expected exception for invalid name";
  EXPECT_EQ(nameList[4], "validname");

  // Test lowercase conversion
  directive::addName("Example.COM My-SITE", nameList);
  EXPECT_EQ(nameList[5], "example.com");
  EXPECT_EQ(nameList[6], "my-site");
  ASSERT_EQ(nameList.size(), 7) << "Expected 6 elements";
}

TEST(DirectiveTest, SetURI) {
  string result = "unchanged";

  // empty should not change the string
  directive::setURI("", result);
  EXPECT_EQ(result, "unchanged");

  // invalid uri throw error
  EXPECT_THROW(directive::setURI("invalid uri", result), runtime_error);

  // valid path should change value
  directive::setURI("/valid/path", result);
  EXPECT_EQ(result, "/valid/path");

  // TODO: instruction bellow
  // need implement validation for specials character
  // directive::setURI("/invalid/path@", result);
  // EXPECT_EQ(result, "/valid/path");
  // EXPECT_THROW(directive::setURI("/path/with/!@#$%^&*()_+-=", result),
  // runtime_error);
}

TEST(DirectiveTest, validateHttpMethod) {
  // test get method
  EXPECT_TRUE(directive::validateHttpMethod("GET"));

  // test post method
  EXPECT_TRUE(directive::validateHttpMethod("POST"));

  // test delete method
  EXPECT_TRUE(directive::validateHttpMethod("DELETE"));

  // test put method, expected false
  EXPECT_FALSE(directive::validateHttpMethod("PUT"));

  // test patch method, expected false
  EXPECT_FALSE(directive::validateHttpMethod("PATCH"));
}

TEST(DirectiveTest, addMethod) {
  set<string> allow_methods = {"GET POST DELETE"};

  EXPECT_THROW(directive::addMethod("MATCH", allow_methods), runtime_error);
  EXPECT_THROW(directive::addMethod("PATCH", allow_methods), runtime_error);
  EXPECT_THROW(directive::addMethod("PUT", allow_methods), runtime_error);
  EXPECT_THROW(directive::addMethod("PUT", allow_methods), runtime_error);
  EXPECT_NO_THROW(directive::addMethod("", allow_methods));
}

TEST(DirectiveTest, setDenyMethods) {
  bool denyMethod = false;

  EXPECT_NO_THROW(directive::setDenyMethods("", denyMethod));

  // send `all` and then check if denyMethod turn from false to true.
  directive::setDenyMethods("all", denyMethod);
  EXPECT_TRUE(denyMethod);

  // send another word instead all and expect a excpetion throw
  EXPECT_THROW(directive::setDenyMethods("anything", denyMethod), runtime_error);
}


// Test suite for setRoot function
TEST(DirectiveTest, setRoot) {
  std::string root;

  // Test empty string - should not modify root
  EXPECT_NO_THROW(directive::setRoot("", root));
  EXPECT_TRUE(root.empty());

  // Test valid root path
  EXPECT_NO_THROW(directive::setRoot("/path/to/root", root));
  EXPECT_EQ(root, "/path/to/root");

  // Test invalid root path with spaces
  EXPECT_THROW(directive::setRoot("invalid path", root), std::runtime_error);

  // Test multiple consecutive valid calls
  EXPECT_NO_THROW(directive::setRoot("/another/path", root));
  EXPECT_EQ(root, "/another/path");

  // Test path with special characters but no spaces
  EXPECT_NO_THROW(directive::setRoot("/path-with_special.chars", root));
  EXPECT_EQ(root, "/path-with_special.chars");
}

// Test suite for setAutoIndex function
TEST(DirectiveTest, setAutoIndex) {
  std::bitset<2> autoindex;

  // Test empty string - should not modify autoindex
  EXPECT_NO_THROW(directive::setAutoIndex("", autoindex));
  EXPECT_EQ(autoindex.to_ulong(), 0);

  // Test setting to "on"
  EXPECT_NO_THROW(directive::setAutoIndex("on", autoindex));
  EXPECT_EQ(autoindex.to_ulong(), parser::AUTOINDEX_ON);

  // Test setting to "off"
  EXPECT_NO_THROW(directive::setAutoIndex("off", autoindex));
  EXPECT_EQ(autoindex.to_ulong(), parser::AUTOINDEX_OFF);

  // Test invalid value
  EXPECT_THROW(directive::setAutoIndex("invalid", autoindex), std::runtime_error);

  // Test case sensitivity
  EXPECT_THROW(directive::setAutoIndex("ON", autoindex), std::runtime_error);
  EXPECT_THROW(directive::setAutoIndex("OFF", autoindex), std::runtime_error);
}

// Test suite for setMaxBodySize
TEST(DirectiveTest, setMaxBodySize) {
  size_t maxBodySize = 0;

  // Test empty string
  EXPECT_NO_THROW(directive::setMaxBodySize("", maxBodySize));
  EXPECT_EQ(maxBodySize, 0);

  // Test zero value (should set to max size_t)
  EXPECT_NO_THROW(directive::setMaxBodySize("0", maxBodySize));
  EXPECT_EQ(maxBodySize, std::numeric_limits<size_t>::max());

  // Test different formats
  EXPECT_NO_THROW(directive::setMaxBodySize("1024", maxBodySize));
  EXPECT_EQ(maxBodySize, 1024 * parser::BYTE);

  EXPECT_NO_THROW(directive::setMaxBodySize("1024B", maxBodySize));
  EXPECT_EQ(maxBodySize, 1024 * parser::BYTE);

  EXPECT_NO_THROW(directive::setMaxBodySize("1K", maxBodySize));
  EXPECT_EQ(maxBodySize, 1 * parser::KILOBYTE);

  EXPECT_NO_THROW(directive::setMaxBodySize("1M", maxBodySize));
  EXPECT_EQ(maxBodySize, 1 * parser::MEGABYTE);

  EXPECT_NO_THROW(directive::setMaxBodySize("1G", maxBodySize));
  EXPECT_EQ(maxBodySize, 1 * parser::GIGABYTE);

  // Test invalid format
  EXPECT_THROW(directive::setMaxBodySize("1T", maxBodySize), std::runtime_error);
  EXPECT_THROW(directive::setMaxBodySize("abc", maxBodySize), std::runtime_error);
}

// Test suite for addIndex
TEST(DirectiveTest, addIndex) {
  std::set<string> indexes;

  // Test empty string
  EXPECT_NO_THROW(directive::addIndex("", indexes));
  EXPECT_TRUE(indexes.empty());

  // Test single index
  EXPECT_NO_THROW(directive::addIndex("index.html", indexes));
  EXPECT_EQ(indexes.size(), 1);
  EXPECT_TRUE(indexes.find("index.html") != indexes.end());

  // Test multiple indexes
  EXPECT_NO_THROW(directive::addIndex("index.html index.php default.html", indexes));
  EXPECT_EQ(indexes.size(), 3);
  EXPECT_TRUE(indexes.find("index.html") != indexes.end());
  EXPECT_TRUE(indexes.find("index.php") != indexes.end());
  EXPECT_TRUE(indexes.find("default.html") != indexes.end());
}

// Test suite for setFastCgi
TEST(DirectiveTest, setFastCgi) {
  std::string fastcgi;

  // Test empty string
  EXPECT_NO_THROW(directive::setFastCgi("", fastcgi));
  EXPECT_TRUE(fastcgi.empty());

  // Test setting value
  EXPECT_NO_THROW(directive::setFastCgi("/path/to/fastcgi", fastcgi));
  EXPECT_EQ(fastcgi, "/path/to/fastcgi");

  // Test overwriting value
  EXPECT_NO_THROW(directive::setFastCgi("/new/path", fastcgi));
  EXPECT_EQ(fastcgi, "/new/path");
}

// Test suite for addErrorPage
TEST(DirectiveTest, addErrorPage) {
  std::map<string, string> errorPages;

  // Test empty string
  EXPECT_NO_THROW(directive::addErrorPage("", errorPages));
  EXPECT_TRUE(errorPages.empty());

  // Test valid error page
  EXPECT_NO_THROW(directive::addErrorPage("404 /404.html", errorPages));
  EXPECT_EQ(errorPages["404"], "/404.html");

  // Test multiple error codes
  EXPECT_NO_THROW(directive::addErrorPage("500 502 503 /50x.html", errorPages));
  EXPECT_EQ(errorPages["500"], "/50x.html");
  EXPECT_EQ(errorPages["502"], "/50x.html");
  EXPECT_EQ(errorPages["503"], "/50x.html");

  // Test invalid cases
  EXPECT_THROW(directive::addErrorPage("invalid", errorPages), std::runtime_error);
  EXPECT_THROW(directive::addErrorPage("600 /error.html", errorPages), std::runtime_error);
  EXPECT_THROW(directive::addErrorPage("abc /error.html", errorPages), std::runtime_error);
}

// Test suite for mergeErrorPages
TEST(DirectiveTest, mergeErrorPages) {
  std::map<string, string> basePages;
  std::map<string, string> newPages;

  // Setup initial error pages
  basePages["404"] = "/404.html";
  basePages["500"] = "/500.html";

  // Setup new error pages
  newPages["404"] = "/new404.html";
  newPages["503"] = "/503.html";

  // Test merging
  EXPECT_NO_THROW(directive::mergeErrorPages(newPages, basePages));
  EXPECT_EQ(basePages["404"], "/404.html");  // Should keep original
  EXPECT_EQ(basePages["500"], "/500.html");  // Should keep original
  EXPECT_EQ(basePages["503"], "/503.html");  // Should add new
}

// Test suite for validateHttpCode
TEST(DirectiveTest, validateHttpCode) {
  // Test valid codes
  EXPECT_TRUE(directive::validateHttpCode("200"));
  EXPECT_TRUE(directive::validateHttpCode("404"));
  EXPECT_TRUE(directive::validateHttpCode("500"));

  // Test invalid codes
  EXPECT_FALSE(directive::validateHttpCode("99"));   // Too low
  EXPECT_FALSE(directive::validateHttpCode("600"));  // Too high
  EXPECT_FALSE(directive::validateHttpCode("abc"));  // Non-numeric
  EXPECT_FALSE(directive::validateHttpCode("4o4"));  // Contains letters
  EXPECT_FALSE(directive::validateHttpCode("-404")); // Negative
}

// Test suite for setReturn
TEST(DirectiveTest, setReturn) {
  std::string code, uri;

  // Test empty string
  EXPECT_NO_THROW(directive::setReturn("", code, uri));
  EXPECT_TRUE(code.empty());
  EXPECT_TRUE(uri.empty());

  // Test valid return with only code
  EXPECT_NO_THROW(directive::setReturn("404", code, uri));
  EXPECT_EQ(code, "404");
  EXPECT_TRUE(uri.empty());

  // Test valid return with code and URI
  EXPECT_NO_THROW(directive::setReturn("301 /redirect", code, uri));
  EXPECT_EQ(code, "301");
  EXPECT_EQ(uri, "/redirect");

  // Test invalid cases
  EXPECT_THROW(directive::setReturn("invalid", code, uri), std::runtime_error);
  EXPECT_THROW(directive::setReturn("600 /error", code, uri), std::runtime_error);
  EXPECT_THROW(directive::setReturn("200 /ok extra", code, uri), std::runtime_error);
}

// Test suite for addServer
TEST(DirectiveTest, addServer) {
  std::vector<Server> servers;
  Server server1, server2;

  // Setup test servers
  std::vector<std::string> listen1;
  listen1.push_back("127.0.0.1:8080");
  server1.setListen(listen1);

  std::vector<std::string> names1;
  names1.push_back("server1");
  server1.setNames(names1);

  // Test adding first server
  EXPECT_NO_THROW(directive::addServer(server1, servers));
  EXPECT_EQ(servers.size(), 1);

  // Setup second server with same port but different IP
  std::vector<std::string> listen2;
  listen2.push_back("192.168.1.1:8080");
  server2.setListen(listen2);

  std::vector<std::string> names2;
  names2.push_back("server2");
  server2.setNames(names2);

  // Test adding second server
  EXPECT_NO_THROW(directive::addServer(server2, servers));
  EXPECT_EQ(servers.size(), 2);

  // Setup third server with conflicting address
  Server server3;
  server3.setListen(listen1);  // Same as server1
  std::vector<std::string> names3;
  names3.push_back("server3");
  server3.setNames(names3);

  EXPECT_THROW(directive::addServer(server3, servers), runtime_error); 
}

#include <cstdlib>
#include <iostream>

TEST(DirectiveTest, setHttpDefaultValues) {
    Http http("../../configurations/default.conf");
    
    // Test setting default values
    EXPECT_NO_THROW(directive::setHttpDefaultValues(http));
    
    // Verify default values were set
    EXPECT_FALSE(http.getIndexes().empty());
    EXPECT_EQ(http.getAccessLog(), "/var/log/access.log");
    EXPECT_EQ(http.getAutoIndexBitSet().to_ulong(), parser::AUTOINDEX_OFF);
    EXPECT_EQ(http.getErrorLog(), "test");
    EXPECT_EQ(http.getRoot(), "/var/www/html");
}

// Test suite for setServerDefaultValues
TEST(DirectiveTest, setServerDefaultValues) {
  Http http("../../configurations/default.conf");
  Server server;

  // Setup http with some values
  http.setMaxBodySize("1024");
  http.setRoot("/var/www");

  // Test setting server default values
  EXPECT_NO_THROW(directive::setServerDefaultValues(http, server));

  // Verify values were inherited from http
  EXPECT_EQ(server.getMaxBodySize(), 1024);
  EXPECT_EQ(server.getRoot(), "/var/www");
  
  std::set<std::string> indexes;
  indexes.insert("index.htm");
  indexes.insert("index.html");
  EXPECT_EQ(server.getIndexes(), indexes);
}

// Test suite for setLocationDefaultValues
TEST(DirectiveTest, setLocationDefaultValues) {
  Server server;
  Location location;

  // Setup server with some values
  server.setMaxBodySize("1024");
  server.setRoot("/var/www");
  std::set<std::string> indexes;
  indexes.insert("index.html");
  server.setIndexes(indexes);

  // Test setting location default values
  EXPECT_NO_THROW(directive::setLocationDefaultValues(server, location));

  // Verify values were inherited from server
  EXPECT_EQ(location.getMaxBodySize(), 1024);
  EXPECT_EQ(location.getRoot(), "/var/www");
  EXPECT_EQ(location.getIndexes(), indexes);
  EXPECT_FALSE(location.getDenyMethods());  // Should be set to default allow methods
}
