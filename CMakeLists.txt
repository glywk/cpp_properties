cmake_minimum_required(VERSION 3.10)
project(cpp_properties)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()
# Activate warnings
if(DEFINED CMAKE_CXX_COMPILER_ID)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR ${CMAKE_CXX_COMPILER_ID} STREQUAL
                                              Clang
  )
    set(warnings "-Wall -Wextra -Werror")
  elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set(warnings "/W4 /EHsc")
  else()
    message(WARNING "Unsupported CXX compiler: ${CMAKE_CXX_COMPILER_ID}")
  endif()
endif()

if(NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS
      "${warnings}"
      CACHE STRING "Flags used by the compiler during all build types." FORCE
  )
  set(CMAKE_C_FLAGS
      "${warnings}"
      CACHE STRING "Flags used by the compiler during all build types." FORCE
  )
endif()

set(CONFIGURED_ONCE
    TRUE
    CACHE INTERNAL "A flag showing that CMake has configured at least once."
)

# Project code
add_subdirectory(src)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
  # is a subproject
  option(BUILD_EXAMPLE "Build example" OFF)
  option(BUILD_TEST "Build test" OFF)
  option(NEED_DOC "Need install" OFF)
  option(NEED_INSTALL "Need install" OFF)
else()
  option(BUILD_EXAMPLE "Build example" ON)
  option(BUILD_TEST "Build test" ON)
  option(NEED_DOC "Need install" ON)
  option(NEED_INSTALL "Need install target" ON)
endif()
MESSAGE(STATUS BUILD_EXAMPLE=${BUILD_EXAMPLE})
# Example
if(BUILD_EXAMPLE)
  add_subdirectory(example)
endif(BUILD_EXAMPLE)
# Unit tests
if(BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif(BUILD_TEST)
# Documentation
if(NEED_DOC)
  add_subdirectory(doc)
endif(NEED_DOC)