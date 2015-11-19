include_directories(}
        ${cpp_properties_SOURCE_DIR}/test
		)
#define a macro to simplify declare and adding tests
macro (declare_test name)
  add_executable(${name} ${name}.cc)
  add_test(${name} ${name})
endmacro (declare_test)

# Test cases
declare_test(test_comment)
declare_test(test_key)
declare_test(test_separator)
declare_test(test_value)
