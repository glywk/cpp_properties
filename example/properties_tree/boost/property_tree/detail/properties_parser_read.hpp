// ----------------------------------------------------------------------------
// Copyright (C) 2015-2021 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_PROPERTIES_PARSER_READ_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_PROPERTIES_PARSER_READ_HPP_INCLUDED

#include "boost/property_tree/detail/properties_parser_error.hpp"
#include "boost/property_tree/ptree.hpp"
//#include "boost/property_tree/detail/properties_parser_utils.hpp"

#include <cpp_properties/action/properties_action.hpp>

#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>

#include <cctype>
#include <fstream>
#include <iterator>
#include <stack>
#include <string>

// define macro for callback member function
#define CALL_MEMBER_FN(member_function) ((*this).*(member_function))

namespace boost {
namespace property_tree {
namespace properties_parser {

/*!
 * in this example the struct 'properties_actor' is used as a functor
 * collecting all key-value properties in the analyzed input sequence by
 * identifying the matched tokens as passed from the lexer.
 */
template <class Ptree, class Ch> class properties_actor {
public:
  // type of properties container output
  typedef Ptree properties_type;
  // type of a key-value property
  typedef std::pair<std::basic_string<Ch>, std::basic_string<Ch>> property_type;

  properties_actor(properties_type &properties_reference)
      : properties(properties_reference), property(none), current_reference(&properties_actor::allocate) {}

  ~properties_actor() {
    // add last property when it is only a key without eol
    if (current_reference == &properties_actor::get) {
      push_back();
    }
  }

  /*!
   * store the current property and change state to prepare
   * the next property read.
   */
  void push_back() {
    current_reference = &properties_actor::allocate;
    properties.push_back(std::make_pair(property.first, Ptree(property.second)));
  }

  /*!
   * create the current property pair to populate and change state to
   * return the current reference.
   */
  property_type &current() { return CALL_MEMBER_FN(current_reference)(); }

private:
  property_type &get() { return property; };

  property_type &allocate() {
    current_reference = &properties_actor::get;
    property = std::make_pair(std::string(), std::string());
    return property;
  };

  // the property container to populate
  properties_type &properties;

  // default reference
  property_type none;

  // the temporary property
  property_type property;

  // callback for lazy initialization of current property pair
  typedef property_type &(properties_actor::*current_reference_callback)();
  // the callback to retrieve the current property reference
  current_reference_callback current_reference;
};

namespace detail {
template <typename Ch> struct basic_file_iterator {
  typedef boost::spirit::classic::position_iterator2<boost::spirit::multi_pass<std::istreambuf_iterator<Ch>>> type;
};

template <typename Ch>
typename basic_file_iterator<Ch>::type make_file_iterator(std::basic_istream<Ch> &input, const std::string &filename) {
  return typename basic_file_iterator<Ch>::type(spirit::make_default_multi_pass(std::istreambuf_iterator<Ch>(input)),
                                                spirit::multi_pass<std::istreambuf_iterator<Ch>>(), filename);
}

/*!
 * helper function to build a property action based on a visitor fields to hide
 * details.
 */
template <typename Actor> cpp_properties::properties_action<Actor> make_action(Actor &actor) {
  return cpp_properties::properties_action<Actor>(actor);
}

template <class Ch> struct properties_parser {
  template <class ForwardTraversalIterator, class Lexer, class Ptree>
  static bool parse(ForwardTraversalIterator first, ForwardTraversalIterator last, const Lexer &lexer,
                    Ptree &cpp_properties) {
    properties_actor<Ptree, Ch> actor(cpp_properties);
    auto action = std::move(make_action(actor));

    return tokenize(first, last, lexer, [&action](auto token) { return action(token); });
  }
};

} // namespace detail

// Build ptree from properties stream
template <class Ptree, class Ch>
void read_properties_internal(std::basic_istream<Ch> &stream, Ptree &pt, const std::string &filename,
                              int /*include_depth*/) {
  namespace lexertl = boost::spirit::lex::lexertl;
  using boost::spirit::lex::tokenize;
  using cpp_properties::cpp_properties_lexer;

  typedef typename detail::basic_file_iterator<Ch>::type token_iterator_type;

  // create the token definition instance needed to invoke the lexical analyzer
  typedef lexertl::token<token_iterator_type, mpl::vector0<>, mpl::true_, std::size_t> token_t;
  cpp_properties_lexer<lexertl::lexer<token_t>> lexer;

  typedef typename detail::properties_parser<Ch> properties_parser;

  Ptree cpp_properties;
  token_iterator_type first(detail::make_file_iterator(stream, filename));
  token_iterator_type last;
  bool success = properties_parser::parse(first, last, lexer, cpp_properties);

  if (success) {
    pt.swap(cpp_properties);
  }
}

} // namespace properties_parser
} // namespace property_tree
} // namespace boost

#endif
