//=============================================================================
// Copyright (c) 2015 glywk
// https://github.com/glywk
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

//  its purpose is to simply read a java like property file and to print the
//  key-value pairs on the standard output. This example also substitute escape
//  sequence and unicode code point by their utf-8 character representation.  

// #define BOOST_SPIRIT_LEXERTL_DEBUG

#include <boost/config/warning_disable.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#include <iostream>
#include <string>
#include <codecvt>
#include <map> 

#include <lexer.hpp>

#include <fstream>

/**
 * helper function reading a file into a string
 */
 inline std::string
read_from_file(char const* infile)
{
    std::ifstream instream(infile);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << infile << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
        std::istreambuf_iterator<char>());
}

/**
 * helper function to concatenate char range to the given string.
 */
template <typename ForwardTraversalIterator>
std::string & operator+=(std::string & lhs, const boost::iterator_range<ForwardTraversalIterator> & range) {
    lhs += std::string(range.begin(), range.end());
    return lhs;
}

/**
 * convert escape char to its given value or remove '\' when escape sequence is
 * not specific.
 */
template <typename ForwardTraversalIterator>
const char escape_sequence(const boost::iterator_range<ForwardTraversalIterator> & sequence) {
    const char c = sequence.back();
    switch (c) {
    case 't':  return '\t'; break;
    case 'r':  return '\r'; break;
    case 'n':  return '\n'; break;
    case 'f':  return '\f'; break;
    default:
        return c;
    }
}

/**
 * convert escape unicode codepoint to its utf-8 character representation.
 */
template <typename ForwardTraversalIterator>
std::string to_utf8(const boost::iterator_range<ForwardTraversalIterator> & code_point)
{
    auto c = code_point.begin();
    // skip unicode escape sequence (\u)
    std::advance(c, 2);
    int value = 0;
    for (int i = 0; i < 4; i++) {
        char chr = *c++;
        switch (chr) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            value = (value << 4) + chr - '0';
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            value = (value << 4) + 10 + chr - 'a';
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            value = (value << 4) + 10 + chr - 'A';
            break;
        }
    }

    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> utf8_converter;

    return utf8_converter.to_bytes(std::char_traits<wchar_t>::to_char_type(value));
}

struct properties_actor_traits
{
    // type of properties container output
    typedef std::map<std::string, std::string> properties_type;
    // type of a key-value property
    typedef std::pair<properties_type::key_type, properties_type::mapped_type> property_type;
};

/**
 * in this example the struct 'properties' is used as a functor collecting all
 * key-value properties in the analyzed input sequence by identifying the
 * matched tokens as passed from the lexer.
 */
struct properties_visitor
{
    // type of properties container output
    typedef properties_actor_traits::properties_type properties_type;
    // type of a key-value property
    typedef properties_actor_traits::property_type property_type;

    properties_visitor(properties_type & properties_reference) : properties(properties_reference) {}

    // finish to add current as reference.
    properties_type & properties;

    // the temporary property
    property_type property;
};

/**
 * in this example the struct 'properties' is used as a functor collecting all
 * key-value properties in the analyzed input sequence by identifying the
 * matched tokens as passed from the lexer.
 */
class properties_actor
{
  public:
    // this is an implementation detail specific to boost::bind and doesn't show 
    // up in the documentation
    typedef bool result_type;

    // type of properties container output
    typedef properties_actor_traits::properties_type properties_type;
    // type of a key-value property
    typedef properties_actor_traits::property_type property_type;

    properties_actor(properties_type & properties_reference, property_type & property_reference) :
        properties(properties_reference), property(property_reference) {}

    // the function operator gets called for each of the matched tokens
    template <typename Token>
    bool operator()(Token const& token) const
    {
        switch (token.id()) {
        case ID_KEY_CHARS:
            property.first += token.value();
            break;
        case ID_KEY_ESCAPE_CHAR:
            property.first += escape_sequence(token.value());
            break;
        case ID_KEY_UNICODE:
            property.first += to_utf8(token.value());
            break;
        case ID_KEY_CR:
        case ID_KEY_LF:
        case ID_KEY_EOL:
        case ID_SEPARATOR_CR:
        case ID_SEPARATOR_LF:
        case ID_SEPARATOR_EOL:
            properties[property.first] = std::string();
            property.first = std::string();
            break;
        case ID_VALUE_SPACES:
        case ID_VALUE_CHARS:
            property.second += token.value();
            break;
        case ID_VALUE_ESCAPE_CHAR:
            property.second += escape_sequence(token.value());
            break;
        case ID_VALUE_UNICODE:
            property.second += to_utf8(token.value());
            break;
        case ID_VALUE_CR:
        case ID_VALUE_LF:
        case ID_VALUE_EOL:
            properties[property.first] = property.second;
            property.first = std::string();
            property.second = std::string();
            break;
        }

        return true;        // always continue to tokenize
    }

  private:
      properties_type & properties;
      property_type & property;
};

/**
 * helper function to build a property actor based on a visitor fields to hide
 * details.
 */
properties_actor && make_actor(properties_visitor & visitor) {
    return std::move(properties_actor(visitor.properties, visitor.property));
}

/**
 * the main function simply loads the given file into memory (as a
 * `std::string`), instantiates an instance of the token definition template
 * using the correct iterator type (`word_count_tokens<char const*>`),
 * and finally calls `lex::tokenize`, passing an instance of the counter function
 * object. The return value of `lex::tokenize()` will be `true` if the
 * whole input sequence has been successfully tokenized, and `false` otherwise.
 */
int main(int argc, char* argv[])
{
    // read input from the given file
    std::string str(read_from_file(1 == argc ? "full.properties" : argv[1]));

    // create the token definition instance needed to invoke the lexical analyzer
    cpp_properties_lexer<lex::lexertl::lexer<> > lexer;

    // tokenize the given string, the bound functor gets invoked for each of 
    // the matched tokens
    char const* first = str.c_str();
    char const* last = &first[str.size()];

    properties_visitor::properties_type cpp_properties;
    properties_visitor visitor(cpp_properties);
    properties_actor actor = make_actor(visitor);

    bool success = lex::tokenize(first, last, lexer, boost::bind(actor, _1));

    // print results
    if (success) {
        for (auto p : cpp_properties) {
            std::cout << p.first << '=' << p.second << "\n";
        }
    } else {
        std::string rest(first, last);
        std::cout << "Lexical analysis failed\n"
            << "stopped at: \"" << rest << "\"\n";
    }
    return 0;
}
