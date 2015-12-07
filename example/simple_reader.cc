//=============================================================================
// Copyright (c) 2015 glywk
// https://github.com/glywk
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

// its purpose is to simply read a java like property file and to print the
// key-value pairs on the standard output. This example also substitute escape
// sequence and unicode code point by their utf-8 character representation.  

// #define BOOST_SPIRIT_LEXERTL_DEBUG

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <utils.hpp>
#include <lexer.hpp>

namespace lex = boost::spirit::lex;
namespace cp = cpp_properties;
using namespace cp::token;

// define macro for callback member function
#define CALL_MEMBER_FN(member_function)  ((*this).*(member_function))

/*!
 * helper function to concatenate char range to the given string.
 */
template <
	typename ForwardTraversalIterator,
	typename CharT = boost::iterator_value<ForwardTraversalIterator>::type
>
std::basic_string<CharT> & operator+=(std::string & lhs, const boost::iterator_range<ForwardTraversalIterator> & range) {
    lhs += std::basic_string<CharT>(range.begin(), range.end());
    return lhs;
}

/*!
 * the key-value property traits to provide to visit the abstract syntax tree.
 */
struct properties_actor_traits
{
    // type of properties container output
    typedef std::vector<std::pair<std::string, std::string> > properties_type;
    // type of a key-value property
    typedef properties_type::value_type property_type;
};

/*!
 * in this example the struct 'properties_actor' is used as a functor
 * collecting all key-value properties in the analyzed input sequence by
 * identifying the matched tokens as passed from the lexer.
 */
template<typename Traits>
class properties_actor
{
public:
    // type of properties container output
    typedef typename Traits::properties_type properties_type;
    // type of a key-value property
    typedef typename Traits::property_type property_type;

    properties_actor(properties_type & properties_reference) :
        properties(properties_reference),
        property(none),
        current_reference(&properties_actor::allocate) {}

    /*!
     * store the current property and change state to prepare
     * the next property read.
     */
    void push_back() {
        current_reference = &properties_actor::allocate;
    }

    /*!
     * create the current property pair to populate and change state to
     * return the current reference.
     */
    property_type & current() {
        return CALL_MEMBER_FN(current_reference)();
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

    // the property container to populate
    properties_type & properties;

    // default reference
    property_type none;

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
template <typename Actor>
class properties_action
{
  public:
    // this is an implementation detail specific to boost::bind and doesn't show 
    // up in the documentation
    typedef bool result_type;

    // actor type
    typedef Actor actor_type;
    // type of properties container output
    typedef typename actor_type::properties_type properties_type;
    // type of a key-value property
    typedef typename actor_type::property_type property_type;

    properties_action(Actor & actor_reference) :
        actor(actor_reference) {}

	properties_action(properties_action && action) :
		actor(std::move(action.actor)) {}

    // the function operator gets called for each of the matched tokens
    template <typename Token>
    bool operator()(Token const& token) const
    {
        switch (token.id()) {
        case ID_KEY_CHARS:
            actor.current().first += latin1_to_utf8(token.value());
            break;
        case ID_KEY_ESCAPE_CHAR:
            actor.current().first += escape_sequence_to_utf8(token.value());
            break;
        case ID_KEY_UNICODE:
            actor.current().first += decode_to_utf8(token.value());
            break;
        case ID_KEY_CR:
        case ID_KEY_LF:
        case ID_KEY_EOL:
        case ID_SEPARATOR_CR:
        case ID_SEPARATOR_LF:
        case ID_SEPARATOR_EOL:
            actor.push_back();
            break;
        case ID_SEPARATOR_EQUAL:
        case ID_SEPARATOR_COLON:
            // force initialization for useless empty key/value case
            actor.current();
            break;
        case ID_VALUE_SPACES:
        case ID_VALUE_CHARS:
            actor.current().second += latin1_to_utf8(token.value());
            break;
        case ID_VALUE_ESCAPE_CHAR:
            actor.current().second += escape_sequence_to_utf8(token.value());
            break;
        case ID_VALUE_UNICODE:
            actor.current().second += decode_to_utf8(token.value());
            break;
        case ID_VALUE_CR:
        case ID_VALUE_LF:
        case ID_VALUE_EOL:
            actor.push_back();
            break;
        }

        // always continue to tokenize
        return true;
    }

  private:
      actor_type & actor;
};

/*!
 * helper function to build a property action based on a visitor fields to hide
 * details.
 */
template <typename Actor>
properties_action<Actor> make_action(Actor & actor) {
    return std::move(properties_action<Actor>(actor));
}

/*!
 * tokenize the text and populate the output container
 */
template<
    typename Traits,
    typename Actor = properties_actor<Traits>,
    typename Action = properties_action<Actor>
>
bool tokenize_and_parse(char const* first, char const* last, typename Traits::properties_type & cpp_properties) {
    // create the token definition instance needed to invoke the lexical analyzer
    cp::cpp_properties_lexer<lex::lexertl::lexer<> > lexer;

    Actor actor(cpp_properties);
    Action action = make_action(actor);

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

    properties_actor_traits::properties_type cpp_properties;

    bool success = tokenize_and_parse<properties_actor_traits>(first, last, cpp_properties);

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
