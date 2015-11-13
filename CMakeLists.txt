cmake_minimum_required(VERSION 2.8)

project(cpp_properties)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Boost 1.47 REQUIRED spirit)

include_directories(
        ${Boost_INCLUDE_DIR}
        ${cpp_properties_SOURCE_DIR}/include
        ${cpp_properties_SOURCE_DIR})

link_directories(${Boost_SPIRIT_LIBRARY_DEBUG})

add_subdirectory(example)

# Test cases
enable_testing()

# Libraries
#add_library(lexer src/lexer.cc)
add_subdirectory(test)

#
#add_executable(test_lexer test/test_lexer.cpp)
#add_test(test_lexer test_lexer)
#
#add_executable(test_lexer2 test/test_lex.cc)
#add_test(test_lexer2 test_lexer2)
