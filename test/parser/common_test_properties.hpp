//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_COMMON_TEST_PARSER_HPP
#define CPP_PROPERTIES_COMMON_TEST_PARSER_HPP

#include <test_parser.hpp>

#include <boost/config/warning_disable.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <vector>

template <typename T, typename V> void run_common_test() {

  // empty
  { BOOST_TEST((properties_eq<T, V>("", {}))); }

  // just a key
  { BOOST_TEST((properties_eq<T, V>("k1", {{"k1", ""}}))); }

  // value
  { BOOST_TEST((properties_eq<T, V>("k1::v:", {{"k1", ":v:"}}))); }
}

template <typename T, typename V = T> void run_map_test() {

  run_common_test<T, V>();

  // duplicate keep first key insertion
  {
    BOOST_TEST((properties_eq<T, V>(R"(
                                 k=v
                                 k=v2
                                 )",
                                    {{"k", "v"}})));
  }
}

template <typename T, typename V = std::vector<typename T::key_type>> void run_set_test() {

  run_common_test<T, V>();

  // duplicate keep first key insertion
  {
    BOOST_TEST((properties_eq<T, V>(R"(
                                 k=v
                                 k=v2
                                 )",
                                    {{"k", "v"}})));
  }
}

#endif
