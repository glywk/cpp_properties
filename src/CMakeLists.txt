cmake_minimum_required(VERSION 3.10)
project(CppProperties
  VERSION 0.1.1
  DESCRIPTION "Java properties parser"
  HOMEPAGE_URL https://github.com/glywk/cpp_properties
  LANGUAGES CXX
)

find_package(Boost 1.47
  REQUIRED
#    spirit
)

add_library(cpp_properties INTERFACE)
target_compile_features(cpp_properties
  INTERFACE
    cxx_std_11
)

target_include_directories(cpp_properties
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(cpp_properties
  INTERFACE ${Boost_INCLUDE_DIR})

add_library(glywk::cpp_properties
  ALIAS
    cpp_properties
)