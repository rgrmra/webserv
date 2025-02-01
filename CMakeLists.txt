cmake_minimum_required(VERSION 3.14..3.28)
project(try_googletest
        VERSION 0.0.1
        DESCRIPTION "Unit Tests"
        LANGUAGES CXX)

include(cmake/UpdateSubModules.cmake)
include(CTest)
include_directories(${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE WEBREQ_SOURCES
        "src/*.cpp"
)

add_library(webreq_lib ${WEBREQ_SOURCES})

target_include_directories(webreq_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parsers
        ${CMAKE_CURRENT_SOURCE_DIR}/src/services
        ${CMAKE_CURRENT_SOURCE_DIR}/src/services/logger
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/color
)

add_subdirectory(tests)
add_subdirectory(external)