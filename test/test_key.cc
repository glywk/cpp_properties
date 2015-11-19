//=============================================================================
// Copyright (c) 2015 glywk
// https://github.com/glywk
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <test_lexer.hpp>

int main(int argc, char* argv[]) {

    // key
    {
        BOOST_TEST(test(" k1",
                  {
                       {" "  , ID_SPACES},
                       {"k1" , ID_KEY_CHARS}
                  }));
        BOOST_TEST(test("k=v",
                  {
                       {"k"  , ID_KEY_CHARS},
                       {"="  , ID_SEPARATOR_EQUAL},
                       {"v"  , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k= v",
                  {
                       {"k"   , ID_KEY_CHARS},
                       {"="   , ID_SEPARATOR_EQUAL},
                       {" "   , ID_VALUE_SPACES},
                       {"v"   , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k =v",
                  {
                       {"k"   , ID_KEY_CHARS},
                       {" "   , ID_SEPARATOR_SPACES},
                       {"="   , ID_SEPARATOR_EQUAL},
                       {"v"   , ID_VALUE_CHARS}
                  }));
    }

    // key only
    {
        BOOST_TEST(test("k\n",
                  {
                       {"k"   , ID_KEY_CHARS},
                       {"\n"  , ID_KEY_LF},
                  }));
        BOOST_TEST(test("k\r",
                  {
                       {"k"  , ID_KEY_CHARS},
                       {"\r" , ID_KEY_CR},
                  }));
        BOOST_TEST(test("k\r\n",
                  {
                       {"k"    , ID_KEY_CHARS},
                       {"\r\n" , ID_KEY_EOL},
                  }));
        BOOST_TEST(test("k\n\r",
                  {
                      {"k"     , ID_KEY_CHARS},
                      {"\n"    , ID_KEY_LF},
                      {"\r"    , ID_CR},
                  }));
    }

    // key line break
    {
        BOOST_TEST(test("k\\\n",
                  {
                       {"k"    , ID_KEY_CHARS},
                       {"\\\n" , ID_KEY_LINE_BREAK_LF}
                  }));
        BOOST_TEST(test("k\\\n"
                        "1"    ,
                  {
                       {"k"    , ID_KEY_CHARS},
                       {"\\\n" , ID_KEY_LINE_BREAK_LF},
                       {"1"    , ID_KEY_CHARS}
                  }));
        BOOST_TEST(test(" k\\\n"
                        " 1"   ,
                  {
                       {" "    , ID_SPACES},
                       {"k"    , ID_KEY_CHARS },
                       {"\\\n ", ID_KEY_LINE_BREAK_LF},
                       {"1"    , ID_KEY_CHARS }
                  }));
        BOOST_TEST(test(" k\\\r"
                        " 1"   , {
                       {" "    , ID_SPACES },
                       {"k"    , ID_KEY_CHARS },
                       {"\\\r ", ID_KEY_LINE_BREAK_CR},
                       {"1"    , ID_KEY_CHARS }
                  }));
        BOOST_TEST(test(" k\\\r\n"
                        " 1"     , {
                       {" "      , ID_SPACES},
                       {"k"      , ID_KEY_CHARS},
                       {"\\\r\n ", ID_KEY_LINE_BREAK_EOL},
                       {"1"      , ID_KEY_CHARS}
                  }));
    }

    // unicode key
    {
        BOOST_TEST(test("\\u03b6",
                  {
                       {"\\u03b6", ID_KEY_UNICODE}
                  }));
        BOOST_TEST(test(" \\u03b6 ",
                  {
                       {" "        , ID_SPACES},
                       {"\\u03b6"  , ID_KEY_UNICODE},
                       {" "        , ID_SEPARATOR_SPACES}
                  }));
        BOOST_TEST(test("\\u03b6=epsilon",
                  {
                       { "\\u03b6"       , ID_KEY_UNICODE},
                       { "="             , ID_SEPARATOR_EQUAL},
                       { "epsilon"       , ID_VALUE_CHARS}
                  }));
    }

    // bad unicode key
    {
        BOOST_TEST(test("\\u000",
                  {
                       { "\\u000", ID_BAD_UNICODE},
                  }));
        BOOST_TEST(test(" \\u000 ",
                  {
                       {" "      , ID_SPACES},
                       {"\\u000" , ID_BAD_UNICODE},
                       {" "      , ID_SEPARATOR_SPACES}
                  }));
        BOOST_TEST(test("\\u000=epsilon",
                  {
                       { "\\u000"       , ID_BAD_UNICODE},
                       { "="            , ID_SEPARATOR_EQUAL},
                       { "epsilon"      , ID_VALUE_CHARS}
                  }));
    }
    // escape char key
    {
        BOOST_TEST(test("\\ \\:\\=" ,
                  {
                       {"\\ "       , ID_KEY_ESCAPE_CHAR},
                       {"\\:"       , ID_KEY_ESCAPE_CHAR},
                       {"\\="       , ID_KEY_ESCAPE_CHAR}
                  }));
    }

  return boost::report_errors();
}