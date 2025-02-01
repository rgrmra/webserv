//
// Created by vini on 2/1/25.
//

#include "directive.hpp"
#include <gtest/gtest.h>


TEST(directive, setAcessLogWithEmptyString) {
    std::string access_log = "";
    std::string _access_log = "/var/log/access.log";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_NE(_access_log, access_log);
}

TEST(directive, setAcessLogWithString) {
        std::string access_log = "up";
        std::string _access_log = "down";

        directive::setAcessLog(access_log, _access_log);

        EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithValidPath) {
    std::string access_log = "/var/log/nginx/access.log";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithPathContainingSpaces) {
    std::string access_log = "/var/log/nginx/access log.log";
    std::string _access_log = "";

    EXPECT_THROW(directive::setAcessLog(access_log, _access_log), std::runtime_error);
}

TEST(directive, setAcessLogWithSpecialCharacters) {
    std::string access_log = "/var/log/nginx/access_log-2023.log";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithLongPath) {
    std::string access_log = "/this/is/a/very/long/path/that/should/be/valid/access.log";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithEmptyAccessLog) {
    std::string access_log = "/var/log/access.log";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithNonEmptyAccessLog) {
    std::string access_log = "/var/log/nginx/access.log";
    std::string _access_log = "/var/log/old_access.log";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithSingleCharacterPath) {
    std::string access_log = "a";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithSingleDotPath) {
    std::string access_log = ".";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithDoubleDotPath) {
    std::string access_log = "..";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}

TEST(directive, setAcessLogWithMultipleSlashes) {
    std::string access_log = "/var////log///access.log";
    std::string _access_log = "";

    directive::setAcessLog(access_log, _access_log);

    EXPECT_EQ(_access_log, access_log);
}