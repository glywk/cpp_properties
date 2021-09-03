//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <test_lexer.hpp>

int main(int argc, char* argv[]) {

    // separator
    {
        BOOST_TEST(test("k ",
                  {
                       {"k" , ID_KEY_CHARS},
                       {" " , ID_SEPARATOR_SPACES}
                  }));
        BOOST_TEST(test("k=",
                  {
                      {"k"  , ID_KEY_CHARS},
                      {"="  , ID_SEPARATOR_EQUAL}
                  }));
        BOOST_TEST(test("k= ",
                  {
                      {"k"  , ID_KEY_CHARS},
                      {"="  , ID_SEPARATOR_EQUAL},
                      {" "  , ID_VALUE_SPACES}
                  }));
        BOOST_TEST(test("k==",
                  {
                      {"k"  , ID_KEY_CHARS},
                      {"="  , ID_SEPARATOR_EQUAL},
                      {"="  , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k:",
                  {
                      {"k"  , ID_KEY_CHARS},
                      {":"  , ID_SEPARATOR_COLON}
                  }));
        BOOST_TEST(test("k::",
                  {
                      {"k"   , ID_KEY_CHARS},
                      {":"   , ID_SEPARATOR_COLON},
                      {":"   , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k:=",
                  {
                      {"k"   , ID_KEY_CHARS},
                      {":"   , ID_SEPARATOR_COLON},
                      {"="   , ID_VALUE_CHARS}
                  }));

    }

    // separator line break
    {
        BOOST_TEST(test("k \\\n"
                        "v"    ,
                  {
                       {"k"    , ID_KEY_CHARS},
                       {" "    , ID_SEPARATOR_SPACES},
                       {"\\\n" , ID_SEPARATOR_LINE_BREAK_LF},
                       {"v"    , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k \\\r"
                        ":v"    ,
                  {
                       {"k"     , ID_KEY_CHARS},
                       {" "     , ID_SEPARATOR_SPACES},
                       {"\\\r"  , ID_SEPARATOR_LINE_BREAK_CR},
                       {":"     , ID_SEPARATOR_COLON},
                       {"v"     , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k \\\r\n"
                        "=v"     ,
                   {
                       {"k"      , ID_KEY_CHARS},
                       {" "      , ID_SEPARATOR_SPACES},
                       {"\\\r\n" , ID_SEPARATOR_LINE_BREAK_EOL},
                       {"="      , ID_SEPARATOR_EQUAL},
                       {"v"      , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test(" k1 \\\n"
                        " v"    ,
                  {
                       {" "     , ID_SPACES},
                       {"k1"    , ID_KEY_CHARS},
                       {" "     , ID_SEPARATOR_SPACES},
                       {"\\\n " , ID_SEPARATOR_LINE_BREAK_LF},
                       {"v"     , ID_VALUE_CHARS}
                  }));
    }

  return boost::report_errors();
}