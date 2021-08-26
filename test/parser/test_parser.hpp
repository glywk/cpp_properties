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
#include <cpp_properties/actor/properties_actor.hpp>
#include <cpp_properties/lexer.hpp>

#include <forward_list>

using std::map;
using std::string;

namespace lex = boost::spirit::lex;
namespace cp = cpp_properties;
using namespace cp::token;

struct emplace_policy {
public:
  template <typename T, typename K, typename V> static void emplace(T &properties, K &&key, V &&value) {
    properties.emplace(std::forward<K>(key), std::forward<V>(value));
  }
};

struct emplace_back_adapter_policy {
public:
  template <typename T, typename K, typename V> static void emplace(T &properties, K &&key, V &&value) {
    properties.emplace_back(std::forward<K>(key), std::forward<V>(value));
  }
};

struct emplace_front_adapter_policy {
public:
  template <typename T, typename K, typename V> static void emplace(T &properties, K &&key, V &&value) {
    properties.emplace_front(std::forward<K>(key), std::forward<V>(value));
  }
};

/*!
 * the key-value property traits to provide to visit the abstract syntax tree.
 */
template <typename T, typename S = std::string, typename E = emplace_policy> struct properties_actor_traits {
  // type of properties container output
  typedef T properties_type;
  // type of key,value property accepted by emplace method
  typedef S string_type;
  typedef E emplacer_policy;
};

/*!
 * tokenize the text and populate the output container
 */
template <typename T, typename Iterator, typename Traits = properties_actor_traits<T>,
          typename Actor = cp::properties_actor<Traits>, typename Action = cp::properties_action<Actor>>
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
template <typename T, typename Iterator, typename Traits = properties_actor_traits<T>>
void parse(Iterator first, Iterator last, T &cpp_properties) {

  bool success = tokenize_and_parse<T, Iterator, Traits>(first, last, cpp_properties);

  // print results
  if (!success) {
    std::string rest(first, last);
    std::cout << "Lexical analysis failed\n"
              << "stopped at: \"" << rest << "\"\n";
  }
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
    os << sep << property;
    sep = ",";
  }
  return os;
}

template <typename T> struct emplacer { typedef emplace_policy emplacer_policy; };

template <typename T> struct emplacer<std::list<T>> { typedef emplace_back_adapter_policy emplacer_policy; };

template <typename T> struct emplacer<std::vector<T>> { typedef emplace_back_adapter_policy emplacer_policy; };

template <typename T> struct emplacer<std::forward_list<T>> { typedef emplace_front_adapter_policy emplacer_policy; };

template <typename T> typename T::size_type size(T &properties) { return properties.size(); }

template <typename T> typename std::forward_list<T>::size_type size(std::forward_list<T> &properties) {
  return std::distance(properties.begin(), properties.end());
}

template <typename T, typename V> bool properties_eq(const string &text, const V &expected) {
  typedef std::string::const_iterator Iterator;
  typedef typename emplacer<T>::emplacer_policy emplacer_policy;
  typedef properties_actor_traits<T, string, emplacer_policy> Traits;
  T properties;
  parse<T, Iterator, Traits>(text.begin(), text.end(), properties);
  V differences;
  std::set_symmetric_difference(expected.begin(), expected.end(), properties.begin(), properties.end(),
                                std::inserter(differences, differences.end()));
  if (!differences.empty()) {
    std::cout << "Actual and expected differs on \"";
    dump_properties(std::cout, differences) << "\"\n";
    std::cout << "*** Debug: parse " << size(properties) << " properties\n";
    return false;
  }
  return true;
}

#endif
