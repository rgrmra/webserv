//
// Created by vini on 2/1/25.
//

#include "Http.hpp"
#include "Mime.hpp"
#include <gtest/gtest.h>

Mime *mimes;
Http *http = NULL;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
