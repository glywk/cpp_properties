//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_TEST_PARSER_HPP
#define CPP_PROPERTIES_TEST_PARSER_HPP

#include <cpp_properties/action/properties_action.hpp>
#include <cpp_properties/lexer.hpp>

#include <boost/config/warning_disable.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>

#include <map>

using std::map;
using std::string;

namespace lex = boost::spirit::lex;
namespace cp = cpp_properties;
using namespace cp::token;

// define macro for callback member function
#define CALL_MEMBER_FN(member_function) ((*this).*(member_function))

/*!
 * the key-value property traits to provide to visit the abstract syntax tree.
 */
struct properties_actor_traits {
  // type of properties container output
  typedef std::map<std::string, std::string> properties_type;
  // type of a key-value property
  typedef properties_type::value_type property_type;
};

/*!
 * in this example the struct 'properties_actor' is used as a functor
 * collecting all key-value properties in the analyzed input sequence by
 * identifying the matched tokens as passed from the lexer.
 */
template <typename Traits> class properties_actor {
public:
  // type of properties container output
  typedef typename Traits::properties_type properties_type;
  // type of a key-value property
  typedef std::pair<typename properties_type::key_type, typename properties_type::mapped_type> property_type;

  ~properties_actor() {
    if (property) {
      properties.emplace(std::move(property->first), std::move(property->second));
    }
  }
  properties_actor(properties_type &properties_reference)
      : properties(properties_reference), property(), current_reference(&properties_actor::allocate) {}

  /*!
   * store the current property and change state to prepare
   * the next property read.
   */
  void push_back() {
    properties.emplace(std::move(property->first), std::move(property->second));
    property = boost::none;
    current_reference = &properties_actor::allocate;
  }

  /*!
   * create the current property pair to populate and change state to
   * return the current reference.
   */
  property_type &current() { return CALL_MEMBER_FN(current_reference)(); }

private:
  property_type &get() { return *property; };

  property_type &allocate() {
    current_reference = &properties_actor::get;
    property = property_type();
    return *property;
  };

  // the property container to populate
  properties_type &properties;

  // the temporary property
  boost::optional<property_type> property;

  // callback for lazy initialization of current property pair
  typedef property_type &(properties_actor::*current_reference_callback)();

  // the callback to retrieve the current property reference
  current_reference_callback current_reference;
};

/*!
 * tokenize the text and populate the output container
 */
template <typename Iterator, typename Traits, typename Actor = properties_actor<Traits>,
          typename Action = properties_action<Actor>>
bool tokenize_and_parse(Iterator first, Iterator last, typename Traits::properties_type &cpp_properties) {
  // create the token definition instance needed to invoke the lexical analyzer
  using lexer_type = lex::lexertl::lexer<lex::lexertl::token<Iterator>>;
  cp::cpp_properties_lexer<lexer_type> lexer;

  Actor actor(cpp_properties);
  Action action = std::move(make_action(actor));

  return lex::tokenize(
      first, last, lexer,
      [&action](const typename cp::cpp_properties_lexer<lexer_type>::token_type &token) { return action(token); });
}

void parse(string::const_iterator first, string::const_iterator last,
           properties_actor_traits::properties_type &cpp_properties) {

  bool success = tokenize_and_parse<std::string::const_iterator, properties_actor_traits>(first, last, cpp_properties);

  // print results
  if (!success) {
    std::string rest(first, last);
    std::cout << "Lexical analysis failed\n"
              << "stopped at: \"" << rest << "\"\n";
  }
  return 0;
}

std::ostream &print_map(std::ostream &os, const std::map<std::string, std::string> &m) {
  string sep = "";
  for (const auto &property : m) {
    os << property.first << " = " << property.second << sep;
    string sep = ",";
  }
  return os;
}

bool properties_eq(const string &text, const map<string, string> &expected) {
  map<string, string> map_properties;
  parse(text.begin(), text.end(), map_properties);
  map<string, string> differences;
  std::set_symmetric_difference(expected.begin(), expected.end(), map_properties.begin(), map_properties.end(),
                                std::inserter(differences, differences.end()));
  if (!differences.empty()) {
    std::cout << "Actual and expected differs on \"";
    print_map(std::cout, differences) << "\"\n";
    std::cout << "*** Debug: parse " << map_properties.size() << " properties\n";
    return false;
  }
  return true;
}

#endif
