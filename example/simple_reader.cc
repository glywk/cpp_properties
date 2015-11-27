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
#include <boost/spirit/include/lex_lexertl.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <vector>
#include <functional>

#include <lexer.hpp>

/*!
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

/*!
 * helper function to concatenate char range to the given string.
 */
template <typename ForwardTraversalIterator>
std::string & operator+=(std::string & lhs, const boost::iterator_range<ForwardTraversalIterator> & range) {
    lhs += std::string(range.begin(), range.end());
    return lhs;
}

/*!
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

/*!
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

/*!
 * the key-value property traits to provide to the abstract syntax tree visitor
 */
struct properties_action_traits
{
    // type of properties container output
    typedef std::vector<std::pair<std::string, std::string> > properties_type;
    // type of a key-value property
    typedef properties_type::value_type property_type;
};

/*!
 * in this example the struct 'properties' is used as a functor collecting all
 * key-value properties in the analyzed input sequence by identifying the
 * matched tokens as passed from the lexer.
 */
class properties_actor
{
public:
    // type of properties container output
    typedef properties_action_traits::properties_type properties_type;
    // type of a key-value property
    typedef properties_action_traits::property_type property_type;

    properties_actor(properties_type & properties_reference) :
        properties(properties_reference),
        property(null_property),
        current_reference(&properties_actor::allocate) {}

    void push_back() {
        current_reference = &properties_actor::allocate;
    }

    property_type & current() {
        return ((*this).*(current_reference))();
    }

private:
    property_type & get() {
            return property;
        };

    property_type & allocate() {
            current_reference = &properties_actor::get;
            properties.emplace_back(std::make_pair(std::string(), std::string()));
            property = properties.back();
            return property;
        };

    // finish to add current as reference.
    properties_type & properties;

    // the initial value
    property_type null_property;

    // the temporary property
    std::reference_wrapper<property_type> property;

    // callback for lazy initialization of current property pair 
    typedef property_type & (properties_actor::*current_reference_callback)();
    // the callback to retrieve the current property reference
    current_reference_callback current_reference;
};

/*!
 * in this example the class 'properties_action' is used as a functor for 
 * collecting all key-value properties in the analyzed input sequence by
 * identifying the matched tokens as passed from the lexer.
 */
class properties_action
{
  public:
    // this is an implementation detail specific to boost::bind and doesn't show 
    // up in the documentation
    typedef bool result_type;

    // type of properties container output
    typedef properties_action_traits::properties_type properties_type;
    // type of a key-value property
    typedef properties_action_traits::property_type property_type;

    properties_action(properties_actor & actor_reference) :
        actor(actor_reference) {}

    // the function operator gets called for each of the matched tokens
    template <typename Token>
    bool operator()(Token const& token) const
    {
        switch (token.id()) {
        case ID_KEY_CHARS:
            actor.current().first += token.value();
            break;
        case ID_KEY_ESCAPE_CHAR:
            actor.current().first += escape_sequence(token.value());
            break;
        case ID_KEY_UNICODE:
            actor.current().first += to_utf8(token.value());
            break;
        case ID_KEY_CR:
        case ID_KEY_LF:
        case ID_KEY_EOL:
        case ID_SEPARATOR_CR:
        case ID_SEPARATOR_LF:
        case ID_SEPARATOR_EOL:
            actor.push_back();
            break;
        case ID_VALUE_SPACES:
        case ID_VALUE_CHARS:
            actor.current().second += token.value();
            break;
        case ID_VALUE_ESCAPE_CHAR:
            actor.current().second += escape_sequence(token.value());
            break;
        case ID_VALUE_UNICODE:
            actor.current().second += to_utf8(token.value());
            break;
        case ID_VALUE_CR:
        case ID_VALUE_LF:
        case ID_VALUE_EOL:
            actor.push_back();
            break;
        }

        return true;        // always continue to tokenize
    }

  private:
      properties_actor & actor;
};

/*!
 * helper function to build a property action based on a visitor fields to hide
 * details.
 */
properties_action && make_action(properties_actor & actor) {
    return std::move(properties_action(actor));
}

/*!
 * tokenize the text and populate the out
 */
bool tokenize_and_parse(char const* first, char const* last, properties_actor::properties_type & cpp_properties) {
    // create the token definition instance needed to invoke the lexical analyzer
    cpp_properties_lexer<lex::lexertl::lexer<> > lexer;

    properties_actor actor(cpp_properties);
    properties_action action = make_action(actor);

    return lex::tokenize(first, last, lexer, [&action](auto token) {
        return action(token);
    });
}

/*!
 * the main function simply loads the given file into memory (as a
 * `std::string`), instantiates an instance of the token definition template
 * using the correct iterator type and finally calls `lex::tokenize`, passing
 * an instance of the properties collector action object. The return value of
 * `lex::tokenize()` will be `true` if the whole input sequence has been
 * successfully tokenized, and `false` otherwise.
 */
int main(int argc, char* argv[])
{
    // read input from the given file
    std::string str(read_from_file(1 == argc ? "full.properties" : argv[1]));

    // tokenize the given string, the bound functor gets invoked for each of 
    // the matched tokens
    char const* first = str.c_str();
    char const* last = &first[str.size()];

    properties_actor::properties_type cpp_properties;

    bool success = tokenize_and_parse(first, last, cpp_properties);

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
