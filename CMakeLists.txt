cmake_minimum_required(VERSION 3.10)
project(cpp_properties)

# Activate warnings
if(DEFINED CMAKE_CXX_COMPILER_ID)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
        set(warnings "-Wall -Wextra -Werror")
    elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        set(warnings "/W4 /EHsc")
    else()
        message(WARNING "Unsupported CXX compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

set(CONFIGURED_ONCE TRUE CACHE INTERNAL "A flag showing that CMake has configured at least once.")

include_directories(
        ${Boost_INCLUDE_DIR}
        ${cpp_properties_SOURCE_DIR}/include
        ${cpp_properties_SOURCE_DIR})

link_directories(${Boost_SPIRIT_LIBRARY_DEBUG})

# Project code
add_subdirectory(src)

# Example
add_subdirectory(example)

# Unit tests
enable_testing()
add_subdirectory(test)
