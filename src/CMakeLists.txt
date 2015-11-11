#include_directories(/etc/apps/include .)
include_directories(
        /usr/local/include
)
link_directories(/usr/local/lib/boost_1_59_0/lib)

add_executable(hello cpp_properties.cc)

add_executable(word_count word_count.cpp)

#link_directories (/etc/apps/lib)
add_executable(cpp_test cpp_test.cc)
target_link_libraries (cpp_test boost_unit_test_framework boost_regex)

add_executable(cpp_test2 cpp_test2.cc)
add_executable(cpp_test3 cpp_test3.cc)



