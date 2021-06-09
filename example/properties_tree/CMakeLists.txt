cmake_minimum_required(VERSION 3.10)
project(PropertyTreeSample)

if(NOT TARGET glywk::cpp_properties)
    find_package(cpp_properties CONFIG REQUIRED)
endif()

if(NOT TARGET glywk::common_sample)
    find_package(cpp_properties_common_sample CONFIG REQUIRED)
endif()

# Example of lexer use
add_executable(properties_tree
    properties_tree.cc
)
target_compile_features(properties_tree
  PUBLIC
    cxx_std_14
)
target_include_directories(properties_tree
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(properties_tree
  PRIVATE
    glywk::common_sample
    glywk::cpp_properties
)