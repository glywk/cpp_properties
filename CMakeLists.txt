cmake_minimum_required(VERSION 3.10)
project(cpp_properties)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

# Project options
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
  # project is include as subproject
  option(CPP_PROPERTIES_BUILD_EXAMPLE "Build examples" OFF)
  option(CPP_PROPERTIES_BUILD_TEST "Build tests" OFF)
  option(CPP_PROPERTIES_BUILD_DOC "Generate documentation" OFF)
  option(CPP_PROPERTIES_INSTALL "Add cpp_properties in install target" OFF)
else()
  option(CPP_PROPERTIES_BUILD_EXAMPLE "Build examples" ON)
  option(CPP_PROPERTIES_BUILD_TEST "Build tests" ON)
  option(CPP_PROPERTIES_BUILD_DOC "Generate documentation" ON)
  option(CPP_PROPERTIES_INSTALL "Add cpp_properties in install target" ON)
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

# Example
if(CPP_PROPERTIES_BUILD_EXAMPLE)
  add_subdirectory(example)
endif(CPP_PROPERTIES_BUILD_EXAMPLE)

# Unit tests
if(CPP_PROPERTIES_BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif(CPP_PROPERTIES_BUILD_TEST)

# Documentation
if(CPP_PROPERTIES_BUILD_DOC)
  add_subdirectory(doc)
endif(CPP_PROPERTIES_BUILD_DOC)
