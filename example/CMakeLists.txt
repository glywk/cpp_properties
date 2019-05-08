cmake_minimum_required(VERSION 3.10)
project(CppPropertiesExamples)

add_library(cpp_properties_common_sample INTERFACE)

target_include_directories(cpp_properties_common_sample
  INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(glywk::common_sample
  ALIAS
    cpp_properties_common_sample
)

# Examples of lexer use
add_subdirectory(simple_reader)