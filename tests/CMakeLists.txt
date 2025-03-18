if(BUILD_TESTING)
	add_executable(webreq
			main.cpp
			directive_test.cpp
			location_test.cpp
			request_test.cpp
			connection_test.cpp
			mime_test.cpp
			header_test.cpp
			status_test.cpp
			code_test.cpp
			response_builder_test.cpp
	)

	target_link_libraries(webreq
			PRIVATE
			GTest::gtest_main
			gtest
			gtest_main
			webreq_lib
			gmock
	)

	include(GoogleTest)
	gtest_discover_tests(webreq)
endif()
