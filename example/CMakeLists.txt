cmake_minimum_required(VERSION 3.10)
project(CppPropertiesExamples)

if(NOT TARGET glywk::cpp_properties)
    find_package(cpp_properties CONFIG REQUIRED)
endif()

# Example of lexer use
add_executable(simple_reader
    simple_reader.cc
)
target_include_directories(simple_reader
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(simple_reader
  PRIVATE
    glywk::cpp_properties
)