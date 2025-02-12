if(BUILD_TESTING)
    add_executable(webreq
            main.cpp
            directive_test.cpp
            location_test.cpp
            request_test.cpp
    )

    target_link_libraries(webreq
            PRIVATE
            GTest::gtest_main
            gtest
            gtest_main
            webreq_lib
    )

    include(GoogleTest)
    gtest_discover_tests(webreq)
endif()
