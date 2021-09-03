//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <test_lexer.hpp>

int main(int argc, char* argv[]) {

    // value
    {
        BOOST_TEST(test("k1::v:",
                  {
                       {"k1"    , ID_KEY_CHARS},
                       {":"     , ID_SEPARATOR_COLON},
                       {":v:"   , ID_VALUE_CHARS}
                  }));
    }

    // value line break
    {
        BOOST_TEST(test("k=v\\\n",
                  {
                       {"k"      , ID_KEY_CHARS},
                       {"="      , ID_SEPARATOR_EQUAL},
                       {"v"      , ID_VALUE_CHARS},
                       {"\\\n"   , ID_VALUE_LINE_BREAK_LF}
                  }));
        BOOST_TEST(test("k=v\\\n"
                        "1"      ,
                  {
                       {"k"      , ID_KEY_CHARS},
                       {"="      , ID_SEPARATOR_EQUAL},
                       {"v"      , ID_VALUE_CHARS},
                       {"\\\n"   , ID_VALUE_LINE_BREAK_LF},
                       {"1"      , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k=v\\\n"
                        " 1"     ,
                  {
                       {"k"      , ID_KEY_CHARS},
                       {"="      , ID_SEPARATOR_EQUAL},
                       {"v"      , ID_VALUE_CHARS},
                       {"\\\n "  , ID_VALUE_LINE_BREAK_LF},
                       {"1"      , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k=v\\\r"
                        " 1"     ,
                  {
                       {"k"      , ID_KEY_CHARS},
                       {"="      , ID_SEPARATOR_EQUAL},
                       {"v"      , ID_VALUE_CHARS},
                       {"\\\r "  , ID_VALUE_LINE_BREAK_CR},
                       {"1"      , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k=v\\\r\n"
                        " 1"     ,
                  {
                       {"k"      , ID_KEY_CHARS},
                       {"="      , ID_SEPARATOR_EQUAL},
                       {"v"      , ID_VALUE_CHARS},
                       {"\\\r\n ", ID_VALUE_LINE_BREAK_EOL},
                       {"1"      , ID_VALUE_CHARS}
                  }));
    }

    // escape char value
    {
        BOOST_TEST(test("k \\ \\:\\=" ,
                  {
                       {"k"           , ID_KEY_CHARS},
                       {" "           , ID_SEPARATOR_SPACES},
                       {"\\ "         , ID_VALUE_ESCAPE_CHAR},
                       {"\\:"         , ID_VALUE_ESCAPE_CHAR},
                       {"\\="         , ID_VALUE_ESCAPE_CHAR}
                  }));
    }

    // unicode value
        BOOST_TEST(test("k=\\u03b6",
                  {
                       {"k"        , ID_KEY_CHARS},
                       {"="        , ID_SEPARATOR_EQUAL},
                       {"\\u03b6"  , ID_VALUE_UNICODE}
                  }));
        BOOST_TEST(test("k= \\u03b6 ",
                  {
                       {"k"          , ID_KEY_CHARS},
                       {"="          , ID_SEPARATOR_EQUAL},
                       {" "          , ID_VALUE_SPACES},
                       {"\\u03b6"    , ID_VALUE_UNICODE},
                       {" "          , ID_VALUE_SPACES}
                  }));
        BOOST_TEST(test("k=\\u03b6epsilon",
                  {
                       {"k"          , ID_KEY_CHARS},
                       {"="          , ID_SEPARATOR_EQUAL},
                       { "\\u03b6"   , ID_VALUE_UNICODE},
                       { "epsilon"   , ID_VALUE_CHARS}
                  }));

    // bad unicode value
    {
        BOOST_TEST(test("k=\\u000",
                  {
                       {"k"       , ID_KEY_CHARS},
                       {"="       , ID_SEPARATOR_EQUAL},
                       { "\\u000" , ID_VALUE_BAD_UNICODE},
                  }));
        BOOST_TEST(test("k= \\u000 ",
                  {
                       {"k"         , ID_KEY_CHARS},
                       {"="         , ID_SEPARATOR_EQUAL},
                       {" "         , ID_VALUE_SPACES},
                       {"\\u000"    , ID_VALUE_BAD_UNICODE},
                       {" "         , ID_VALUE_SPACES}
                  }));
        BOOST_TEST(test("k=\\u000giga",
                  {
                       {"k"           , ID_KEY_CHARS},
                       {"="           , ID_SEPARATOR_EQUAL},
                       {"\\u000"      , ID_VALUE_BAD_UNICODE},
                       {"giga"        , ID_VALUE_CHARS}
                  }));
    }

  return boost::report_errors();
}