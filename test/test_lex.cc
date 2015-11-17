//=============================================================================
// Copyright (c) 2015 glywk
// https://github.com/glywk
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <test_lexer.hpp>

int main(int argc, char* argv[]) {
  
  // blanks and spaces
    {
        BOOST_TEST(test("" ,
                  {
                  }));
        BOOST_TEST(test(" ",
                  {
                       {" ", ID_SPACES}
                  }));
        BOOST_TEST(test(" \t\f",
                  {
                       {" \t\f", ID_SPACES}
                  }));
    }

    // line break
    // ...
    {
		BOOST_TEST(test("\\\n",
                  {
                       {"\\\n", ID_LINE_BREAK_LF}
                  }));
		BOOST_TEST(test("\\\r",
                  {
                       {"\\\r", ID_LINE_BREAK_CR}
                  }));
		BOOST_TEST(test("\\\r\n",
                  {
                       {"\\\r\n", ID_LINE_BREAK_EOL}
                  }));
        BOOST_TEST(test("\\\n"
                        "\r",
                  {
                       {"\\\n"  , ID_LINE_BREAK_LF},
                       {"\r"    , ID_CR}
                  }));
        BOOST_TEST(test("\\\n"
                        "! comment break_line",
                  {
                       {"\\\n"        , ID_LINE_BREAK_LF},
                       {"!"           , ID_COMMENT_EXCLAMATION},
                       { " "          , ID_COMMENT_SPACES },
                       { "comment"    , ID_COMMENT_CHARS },
                       { " "          , ID_COMMENT_SPACES },
                       { "break_line" , ID_COMMENT_CHARS }
                  }));
    }
    // comment
    {
        BOOST_TEST(test("!", {
                       {"!", ID_COMMENT_EXCLAMATION}
                  }));
        BOOST_TEST(test("#",
                  {
                       {"#", ID_COMMENT_SHARP}
                  }));
        BOOST_TEST(test("#\r",
                  {
                       {"#"  , ID_COMMENT_SHARP},
                       {"\r" , ID_COMMENT_CR}
                  }));
        BOOST_TEST(test("#\n",
                  {
                       {"#"  , ID_COMMENT_SHARP},
                       {"\n" , ID_COMMENT_LF}
                  }));
        BOOST_TEST(test("#\r\n", {
                       {"#"    , ID_COMMENT_SHARP},
                       {"\r\n" , ID_COMMENT_EOL}
                  }));
        BOOST_TEST(test("! new comment",
                  {
                       {"!"            , ID_COMMENT_EXCLAMATION},
                       {" "            , ID_COMMENT_SPACES},
                       {"new"          , ID_COMMENT_CHARS},
                       {" "            , ID_COMMENT_SPACES},
                       {"comment"      , ID_COMMENT_CHARS}
                  }));
        BOOST_TEST(test("   #comment ",
                  {
                       {"   "         , ID_SPACES},
                       {"#"           , ID_COMMENT_SHARP},
                       {"comment"     , ID_COMMENT_CHARS},
                       {" "           , ID_COMMENT_SPACES}
                  }));
        BOOST_TEST(test("#\\ ",
                  {
                       {"#"   , ID_COMMENT_SHARP},
                       {"\\"  , ID_COMMENT_CHARS},
                       {" "   , ID_COMMENT_SPACES}
                  }));
        BOOST_TEST(test("#\\\n",
                  {
                       {"#"    , ID_COMMENT_SHARP},
                       {"\\"   , ID_COMMENT_CHARS},
                       {"\n"   , ID_COMMENT_LF}
                  }));
    }

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
                       {" "          , ID_VALUE_CHARS}
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
                       { "\\u000" , ID_BAD_UNICODE},
                  }));
        BOOST_TEST(test("k= \\u000 ",
                  {
                       {"k"         , ID_KEY_CHARS},
                       {"="         , ID_SEPARATOR_EQUAL},
                       {" "         , ID_VALUE_SPACES},
                       {"\\u000"    , ID_BAD_UNICODE},
                       {" "         , ID_VALUE_CHARS}
                  }));
        BOOST_TEST(test("k=\\u000giga",
                  {
                       {"k"           , ID_KEY_CHARS},
                       {"="           , ID_SEPARATOR_EQUAL},
                       {"\\u000"      , ID_BAD_UNICODE},
                       {"giga"        , ID_VALUE_CHARS}
                  }));
    }

  return boost::report_errors();
}