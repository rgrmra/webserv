if(BUILD_TESTING)
    add_executable(webreq
            main.cpp
            parser_test.cpp
			location_test.cpp
    )

    target_include_directories(webreq PRIVATE ${CMAKE_SOURCE_DIR}/external/googletest/googletest/include)
    include_directories(${CMAKE_SOURCE_DIR}/build/lib)

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
