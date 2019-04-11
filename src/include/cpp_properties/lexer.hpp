//=============================================================================
// Copyright (c) 2015-2018 glywk
// https://github.com/glywk
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_LEXER_HPP
#define CPP_PROPERTIES_LEXER_HPP

namespace cpp_properties
{
namespace lex = boost::spirit::lex;

namespace token
{
    /*!
     * the property tokens definition.
     */
    enum token_id
    {
        ID_SPACES = 1000,
        ID_CR,
        ID_LF,
        ID_EOL,
        ID_LINE_BREAK_CR,
        ID_LINE_BREAK_LF,
        ID_LINE_BREAK_EOL,
        ID_COMMENT_SHARP,
        ID_COMMENT_EXCLAMATION,
        ID_COMMENT_CHARS,
        ID_COMMENT_SPACES,
        ID_COMMENT_CR,
        ID_COMMENT_LF,
        ID_COMMENT_EOL,
        ID_KEY_CHARS,
        ID_KEY_ESCAPE_CHAR,
        ID_KEY_UNICODE,
        ID_KEY_BAD_UNICODE,
        ID_KEY_CR,
        ID_KEY_LF,
        ID_KEY_EOL,
        ID_KEY_LINE_BREAK_CR,
        ID_KEY_LINE_BREAK_LF,
        ID_KEY_LINE_BREAK_EOL,
        ID_SEPARATOR_COLON,
        ID_SEPARATOR_EQUAL,
        ID_SEPARATOR_SPACES,
        ID_SEPARATOR_CR,
        ID_SEPARATOR_LF,
        ID_SEPARATOR_EOL,
        ID_SEPARATOR_LINE_BREAK_CR,
        ID_SEPARATOR_LINE_BREAK_LF,
        ID_SEPARATOR_LINE_BREAK_EOL,
        ID_VALUE_SPACES,
        ID_VALUE_CHARS,
        ID_VALUE_ESCAPE_CHAR,
        ID_VALUE_UNICODE,
        ID_VALUE_BAD_UNICODE,
		ID_VALUE_CR,
        ID_VALUE_LF,
        ID_VALUE_EOL,
        ID_VALUE_LINE_BREAK_CR,
        ID_VALUE_LINE_BREAK_LF,
        ID_VALUE_LINE_BREAK_EOL
    };
}

/*!
 * definition of the property lexer.
 */
template <typename Lexer>
struct cpp_properties_lexer : lex::lexer<Lexer>
{
    cpp_properties_lexer()
    {
        this->self.add_pattern
            ("cr"          , "\\r")
            ("lf"          , "\\n")
            ("eol"         , "\\r\\n")

            ("lb_cr"       , "\\\\\\r[ \\f\\t]*")
            ("lb_lf"       , "\\\\\\n[ \\f\\t]*")
            ("lb_eol"      , "\\\\\\r\\n[ \\f\\t]*")

            ("spaces"      , "[ \\f\\t]+")

            ("sharp"       , "#")
            ("exclamation" , "!")
            ("comment"     , "[^ \\f\\t\\r\\n]+")

            ("colon"       , ":")
            ("equal"       , "=")

            ("backslash"   , "\\\\")
            ("first_key"   , "[^ \\f\\t\\r\\n\\\\=:#!][^ \\f\\t\\r\\n\\\\=:]*")
            ("key"         , "[^ \\f\\t\\r\\n\\\\=:]+")

            ("blank_value" , "[^ \\f\\t\\r\\n\\\\=:][^ \\f\\t\\r\\n\\\\]*")
            ("value" , "[^ \\f\\t\\r\\n\\\\]+")

            ("unicode"     , "\\\\u[0-9a-fA-F]{4}")
            ("bad_unicode" , "\\\\u[0-9a-fA-F]{0,3}")
            ("escape"      , "\\\\[^\\r\\n]")
        ;

        this->self("INITIAL").add 
            ("{spaces}"      , ID_SPACES)
            ("{cr}"          , ID_CR)
            ("{lf}"          , ID_LF)
            ("{eol}"         , ID_EOL)
            ("{lb_cr}"       , ID_LINE_BREAK_CR)
            ("{lb_lf}"       , ID_LINE_BREAK_LF)
            ("{lb_eol}"      , ID_LINE_BREAK_EOL)
        ;

        this->self("INITIAL", "COMMENT").add
            ("{sharp}"       , ID_COMMENT_SHARP)
            ("{exclamation}" , ID_COMMENT_EXCLAMATION)
        ;

        this->self("COMMENT").add
            ("{comment}"     , ID_COMMENT_CHARS)
            ("{spaces}"      , ID_COMMENT_SPACES)
        ;

        this->self("COMMENT", "INITIAL").add
            ("{cr}"          , ID_COMMENT_CR)
            ("{lf}"          , ID_COMMENT_LF)
            ("{eol}"         , ID_COMMENT_EOL)
        ;

        this->self("INITIAL", "KEY").add
            ("{first_key}"   , ID_KEY_CHARS)
            ("{escape}"      , ID_KEY_ESCAPE_CHAR)
            ("{unicode}"     , ID_KEY_UNICODE)
            ("{bad_unicode}" , ID_KEY_BAD_UNICODE)
        ;

        this->self("KEY").add
            ("{key}"         , ID_KEY_CHARS)
            ("{escape}"      , ID_KEY_ESCAPE_CHAR)
            ("{unicode}"     , ID_KEY_UNICODE)
            ("{lb_cr}"       , ID_KEY_LINE_BREAK_CR)
            ("{lb_lf}"       , ID_KEY_LINE_BREAK_LF)
            ("{lb_eol}"      , ID_KEY_LINE_BREAK_EOL)
            ("{bad_unicode}" , ID_KEY_BAD_UNICODE)
        ;

        this->self("KEY", "INITIAL").add
            ("{cr}"          , ID_KEY_CR)
            ("{lf}"          , ID_KEY_LF)
            ("{eol}"         , ID_KEY_EOL)
        ;

        this->self("KEY", "SEPARATOR").add
            ("{colon}"       , ID_SEPARATOR_COLON)
            ("{equal}"       , ID_SEPARATOR_EQUAL)
        ;

        this->self("KEY", "BLANK_SEPARATOR").add
            ("{spaces}"      , ID_SEPARATOR_SPACES)
        ;

        this->self("BLANK_SEPARATOR").add
            ("{lb_cr}"       , ID_SEPARATOR_LINE_BREAK_CR)
            ("{lb_lf}"       , ID_SEPARATOR_LINE_BREAK_LF)
            ("{lb_eol}"      , ID_SEPARATOR_LINE_BREAK_EOL)
            ("{spaces}"      , ID_SEPARATOR_SPACES)
        ;

        this->self("BLANK_SEPARATOR", "INITIAL").add
            ("{cr}"          , ID_SEPARATOR_CR)
            ("{lf}"          , ID_SEPARATOR_LF)
            ("{eol}"         , ID_SEPARATOR_EOL)
        ;

        this->self("BLANK_SEPARATOR", "SEPARATOR").add
            ("{colon}"       , ID_SEPARATOR_COLON)
            ("{equal}"       , ID_SEPARATOR_EQUAL)
        ;

        this->self("INITIAL", "SEPARATOR").add
            ("{colon}"       , ID_SEPARATOR_COLON)
            ("{equal}"       , ID_SEPARATOR_EQUAL)
        ;

        this->self("SEPARATOR", "INITIAL").add
            ("{cr}"          , ID_SEPARATOR_CR)
            ("{lf}"          , ID_SEPARATOR_LF)
            ("{eol}"         , ID_SEPARATOR_EOL)
        ;

        this->self("SEPARATOR", "VALUE").add
            ("{spaces}"      , ID_VALUE_SPACES)
            ("{value}"       , ID_VALUE_CHARS)
            ("{escape}"      , ID_VALUE_ESCAPE_CHAR)
            ("{unicode}"     , ID_VALUE_UNICODE)
            ("{lb_cr}"       , ID_VALUE_LINE_BREAK_CR)
            ("{lb_lf}"       , ID_VALUE_LINE_BREAK_LF)
            ("{lb_eol}"      , ID_VALUE_LINE_BREAK_EOL)
            ("{bad_unicode}" , ID_VALUE_BAD_UNICODE)
        ;

        this->self("BLANK_SEPARATOR", "VALUE").add
            ("{blank_value}" , ID_VALUE_CHARS)
            ("{escape}"      , ID_VALUE_ESCAPE_CHAR)
            ("{unicode}"     , ID_VALUE_UNICODE)
            ("{bad_unicode}" , ID_VALUE_BAD_UNICODE)
        ;

        this->self("VALUE").add
            ("{spaces}"      , ID_VALUE_SPACES)
            ("{value}"       , ID_VALUE_CHARS)
            ("{escape}"      , ID_VALUE_ESCAPE_CHAR)
            ("{unicode}"     , ID_VALUE_UNICODE)
            ("{lb_cr}"       , ID_VALUE_LINE_BREAK_CR)
            ("{lb_lf}"       , ID_VALUE_LINE_BREAK_LF)
            ("{lb_eol}"      , ID_VALUE_LINE_BREAK_EOL)
            ("{bad_unicode}" , ID_VALUE_BAD_UNICODE)
        ;

        this->self("VALUE", "INITIAL").add
            ("{cr}"          , ID_VALUE_CR)
            ("{lf}"          , ID_VALUE_LF)
            ("{eol}"         , ID_VALUE_EOL)
        ;

    }
};

}
#endif