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
#include <cpp_properties/actor/traits/properties_actor_traits.hpp>
#include <cpp_properties/parser.hpp>

#include <forward_list>

using std::map;
using std::string;

namespace lex = boost::spirit::lex;
namespace cp = cpp_properties;
using namespace cp::token;

template <typename T, typename Iterator, typename Traits = cp::properties_actor_traits<T>>
void parse(Iterator first, Iterator last, T &cpp_properties) {

  bool success = cp::parse<T, Iterator, Traits>(first, last, cpp_properties);

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

template <typename T> struct emplacer { typedef cp::emplace_policy emplacer_policy; };

template <typename T> struct emplacer<std::list<T>> { typedef cp::emplace_back_adapter_policy emplacer_policy; };

template <typename T> struct emplacer<std::vector<T>> { typedef cp::emplace_back_adapter_policy emplacer_policy; };

template <typename T> struct emplacer<std::forward_list<T>> {
  typedef cp::emplace_front_adapter_policy emplacer_policy;
};

template <typename T> typename T::size_type size(T &properties) { return properties.size(); }

template <typename T> typename std::forward_list<T>::size_type size(std::forward_list<T> &properties) {
  return std::distance(properties.begin(), properties.end());
}

template <typename ForwardTraversalIterator,
          typename T = typename std::remove_cv<typename ForwardTraversalIterator::value_type>::type>
std::list<T> make_sorted_collection(const boost::iterator_range<ForwardTraversalIterator> &sequence) {
  std::list<T> collection;
  for (auto &&item : sequence) {
    collection.insert(std::upper_bound(collection.begin(), collection.end(), item), item);
  }
  return collection;
}

template <typename T, typename V> bool properties_eq(const string &text, const V &expected) {
  typedef std::string::const_iterator Iterator;
  typedef typename emplacer<T>::emplacer_policy emplacer_policy;
  typedef cp::properties_actor_traits<T, string, emplacer_policy> Traits;

  T properties;
  parse<T, Iterator, Traits>(text.begin(), text.end(), properties);

  const auto sorted_properties = make_sorted_collection(boost::make_iterator_range(properties));
  const auto sorted_expected = make_sorted_collection(boost::make_iterator_range(expected));
  V differences;
  std::set_symmetric_difference(sorted_expected.begin(), sorted_expected.end(), sorted_properties.begin(),
                                sorted_properties.end(), std::inserter(differences, differences.end()));

  if (!differences.empty()) {
    std::cout << "Actual and expected differs on \"";
    dump_properties(std::cout, differences) << "\"\n";
    std::cout << "*** Debug: parse " << size(properties) << " properties\n";
    return false;
  }
  return true;
}

#endif
