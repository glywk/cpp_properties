include_directories(}
        ${cpp_properties_SOURCE_DIR}/test
		)
# Test cases
add_executable(test_lexer test_lex.cc)
add_test(test_lexer test_lexer)
