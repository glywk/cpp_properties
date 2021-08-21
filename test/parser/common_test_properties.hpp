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

template <typename T> void run_test() {

  // empty
  { BOOST_TEST(properties_eq<T>("", {})); }

  // value
  { BOOST_TEST(properties_eq<T>("k1::v:", {{"k1", ":v:"}})); }

  // duplicate keep first
  {
    BOOST_TEST(properties_eq<T>(R"(
                                 k=v
                                 k=v2
                                 )",
                                {{"k", "v"}}));
  }
}
#endif
