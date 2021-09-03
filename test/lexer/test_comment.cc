//=============================================================================
// Copyright (c) 2015-2021 glywk
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

  return boost::report_errors();
}