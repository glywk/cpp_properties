//=============================================================================
// Copyright (c) 2015-2018 glywk
// https://github.com/glywk
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPETIES_TEST_LEXER_HPP
#define CPP_PROPETIES_TEST_LEXER_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <cpp_properties/lexer.hpp>
#include <list>

using std::list;
using std::string;

namespace lex = boost::spirit::lex;
namespace cp = cpp_properties;
using namespace cp::token;

struct expected_token {
    int id;
    string value;
    expected_token(string text, int token_id):id(token_id), value(text) {}
};

class tokenizer {
    typedef lex::lexertl::token<char const*, boost::mpl::vector0<>, boost::mpl::true_> token_type;
    typedef lex::lexertl::actor_lexer<token_type> lexer_type;
    
    void load_token_name_id() {
        name[ID_SPACES]                   = "ID_SPACES";
        name[ID_CR]                       = "ID_CR";
        name[ID_LF]                       = "ID_LF";
        name[ID_EOL]                      = "ID_EOL";
        name[ID_LINE_BREAK_CR]            = "ID_LINE_BREAK_CR";
        name[ID_LINE_BREAK_LF]            = "ID_LINE_BREAK_LF";
        name[ID_LINE_BREAK_EOL]           = "ID_LINE_BREAK_EOL";
        name[ID_COMMENT_SHARP]            = "ID_COMMENT_SHARP";
        name[ID_COMMENT_EXCLAMATION]      = "ID_COMMENT_EXCLAMATION";
        name[ID_COMMENT_CHARS]            = "ID_COMMENT_CHARS";
        name[ID_COMMENT_SPACES]           = "ID_COMMENT_SPACES";
        name[ID_COMMENT_CR]               = "ID_COMMENT_CR";
        name[ID_COMMENT_LF]               = "ID_COMMENT_LF";
        name[ID_COMMENT_EOL]              = "ID_COMMENT_EOL";
        name[ID_KEY_CHARS]                = "ID_KEY_CHARS";
        name[ID_KEY_ESCAPE_CHAR]          = "ID_KEY_ESCAPE_CHAR";
        name[ID_KEY_UNICODE]              = "ID_KEY_UNICODE";
        name[ID_KEY_BAD_UNICODE]          = "ID_KEY_BAD_UNICODE";
        name[ID_KEY_CR]                   = "ID_KEY_CR";
        name[ID_KEY_LF]                   = "ID_KEY_LF";
        name[ID_KEY_EOL]                  = "ID_KEY_EOL";
        name[ID_KEY_LINE_BREAK_CR]        = "ID_KEY_LINE_BREAK_CR"; 
        name[ID_KEY_LINE_BREAK_LF]        = "ID_KEY_LINE_BREAK_LF"; 
        name[ID_KEY_LINE_BREAK_EOL]       = "ID_KEY_LINE_BREAK_EOL";
        name[ID_SEPARATOR_COLON]          = "ID_SEPARATOR_COLON";
        name[ID_SEPARATOR_EQUAL]          = "ID_SEPARATOR_EQUAL";
        name[ID_SEPARATOR_SPACES]         = "ID_SEPARATOR_SPACES";
        name[ID_SEPARATOR_CR]             = "ID_SEPARATOR_CR";
        name[ID_SEPARATOR_LF]             = "ID_SEPARATOR_LF";
        name[ID_SEPARATOR_EOL]            = "ID_SEPARATOR_EOL";
        name[ID_SEPARATOR_LINE_BREAK_CR]  = "ID_SEPARATOR_LINE_BREAK_CR";
        name[ID_SEPARATOR_LINE_BREAK_LF]  = "ID_SEPARATOR_LINE_BREAK_LF";
        name[ID_SEPARATOR_LINE_BREAK_EOL] = "ID_SEPARATOR_LINE_BREAK_EOL";
        name[ID_VALUE_SPACES]             = "ID_VALUE_SPACES";
        name[ID_VALUE_CHARS]              = "ID_VALUE_CHARS";
        name[ID_VALUE_ESCAPE_CHAR]        = "ID_VALUE_ESCAPE_CHAR";
        name[ID_VALUE_UNICODE]            = "ID_VALUE_UNICODE";
        name[ID_VALUE_BAD_UNICODE]        = "ID_VALUE_BAD_UNICODE";
        name[ID_VALUE_CR]                 = "ID_VALUE_CR";
        name[ID_VALUE_LF]                 = "ID_VALUE_LF";
        name[ID_VALUE_EOL]                = "ID_VALUE_EOL";
        name[ID_VALUE_LINE_BREAK_CR]      = "ID_VALUE_LINE_BREAK_CR";
        name[ID_VALUE_LINE_BREAK_LF]      = "ID_VALUE_LINE_BREAK_LF";
        name[ID_VALUE_LINE_BREAK_EOL]     = "ID_VALUE_LINE_BREAK_EOL";
    }
    
 public: 
    typedef expected_token value_type;

    tokenizer(const string &input): text(input) {
        load_token_name_id();
    }
    
    tokenizer(const tokenizer &rhs): text(rhs.text) {
        load_token_name_id();
    }
    
    bool as(const list<value_type>& rhs) {
        char const* first = text.c_str();
        char const* last = &first[text.size()];

        cp::cpp_properties_lexer<lexer_type> lexer;
        lexer_type::iterator_type iter = lexer.begin(first, last);
        lexer_type::iterator_type end = lexer.end();
        
        list<value_type>::const_iterator citer_expected = rhs.cbegin();
        list<value_type>::const_iterator cend_expected = rhs.cend();
        
        int i = 0;
        while (iter != end && token_is_valid(*iter) && citer_expected != cend_expected) {
            if (iter->id() != citer_expected->id) { 
                std::cout << "state: " << iter->state() << ", "
                            << "id: expected=" << name[citer_expected->id] << ", actual=" << name[iter->id()] << ", "
                          << "string:>" << iter->value() << "<\n";
                return false;
            }
            if (iter->value() != citer_expected->value) {
            std::cout << "state: " << iter->state() << ", "
                      << "id: " << name[iter->id()] << ", " 
                      << "string: expected=>" << iter->value() << "<, actual=>" << citer_expected->value << "<\n";
                return false;
            }
            ++i;
            ++iter;
            ++citer_expected;
        }
        if (iter == end) {
            std::cout << "*** Debug: " << i << " token(s) found\n";
            if (citer_expected != cend_expected) {
                std::cout << "missing expected tokens\n"; 
                return false;
            } else {
                return true;
            }
        } else {
            std::string rest(first, last);
            std::cout << "Lexical analysis failed\n" << "stopped at: \"" 
                      << rest << "\"\n";
        }
        return false;
    }
    
  private:
    const string text;
    std::map<int, std::string> name;
};

bool test(const string& text, const list<tokenizer::value_type>& tokens) {
  return tokenizer(text).as(tokens);
}

#endif