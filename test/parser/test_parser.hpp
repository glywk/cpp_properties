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
template <typename T, typename S = std::string> struct properties_actor_traits {
  // type of properties container output
  typedef T properties_type;
  // type of key,value property accepted by emplace method
  typedef S string_type;
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

  // type of transient action key-value property
  typedef typename Traits::string_type key_type;
  typedef typename Traits::string_type mapped_type;
  typedef std::pair<key_type, mapped_type> property_type;

public:
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
template <typename T, typename Iterator, typename Traits = properties_actor_traits<T>,
          typename Actor = properties_actor<Traits>, typename Action = properties_action<Actor>>
bool tokenize_and_parse(Iterator first, Iterator last, T &cpp_properties) {
  // create the token definition instance needed to invoke the lexical analyzer
  using lexer_type = lex::lexertl::lexer<lex::lexertl::token<Iterator>>;
  cp::cpp_properties_lexer<lexer_type> lexer;

  Actor actor(cpp_properties);
  Action action = std::move(make_action(actor));

  return lex::tokenize(
      first, last, lexer,
      [&action](const typename cp::cpp_properties_lexer<lexer_type>::token_type &token) { return action(token); });
}

template <typename Map> void parse(string::const_iterator first, string::const_iterator last, Map &cpp_properties) {

  bool success = tokenize_and_parse(first, last, cpp_properties);

  // print results
  if (!success) {
    std::string rest(first, last);
    std::cout << "Lexical analysis failed\n"
              << "stopped at: \"" << rest << "\"\n";
  }
  return 0;
}

std::ostream &operator<<(std::ostream &os, const std::pair<const std::string, std::string> &property) {
  os << std::get<0>(property) << " = " << std::get<1>(property);
  return os;
}

std::ostream &operator<<(std::ostream &os, const std::pair<std::string, std::string> &property) {
  os << std::get<0>(property) << " = " << std::get<1>(property);
  return os;
}

std::ostream &operator<<(std::ostream &os, const std::tuple<std::string, std::string> &property) {
  os << std::get<0>(property) << " = " << std::get<1>(property);
  return os;
}

template <typename T> std::ostream &dump_properties(std::ostream &os, const T &properties) {
  string sep = "";
  for (const auto &property : properties) {
    os << property << sep;
    string sep = ",";
  }
  return os;
}

template <typename T, typename V> bool properties_eq(const string &text, const V &expected) {
  T properties;
  parse(text.begin(), text.end(), properties);
  V differences;
  std::set_symmetric_difference(expected.begin(), expected.end(), properties.begin(), properties.end(),
                                std::inserter(differences, differences.end()));
  if (!differences.empty()) {
    std::cout << "Actual and expected differs on \"";
    dump_properties(std::cout, differences) << "\"\n";
    std::cout << "*** Debug: parse " << properties.size() << " properties\n";
    return false;
  }
  return true;
}

#endif
