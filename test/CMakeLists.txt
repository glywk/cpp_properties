cmake_minimum_required(VERSION 3.10)
project(CppPropertiesTests)

find_package(Boost 1.47 REQUIRED # test
)

# dependencies
enable_testing()

if(NOT TARGET glywk::cpp_properties)
  find_package(cpp_properties CONFIG REQUIRED)
endif()

# define a macro to simplify declare and adding tests
macro(declare_test name)
  add_executable(${name} ${name}.cc)
  target_include_directories(
    ${name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  )
  target_link_libraries(${name} PRIVATE glywk::cpp_properties)
  add_test(${name} ${name})
endmacro(declare_test)

# Test cases
add_subdirectory(lexer)
add_subdirectory(parser)
