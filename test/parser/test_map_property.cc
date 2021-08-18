//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <test_parser.hpp>

int main(int argc, char *argv[]) {

  // empty
  { BOOST_TEST(properties_eq("", {})); }

  // value
  { BOOST_TEST(properties_eq("k1::v:", {{"k1", ":v:"}})); }

  // duplicate keep first
  {
    BOOST_TEST(properties_eq(R"(
                                 k=v
                                 k=v2
                                 )",
                             {{"k", "v"}}));
  }
  return boost::report_errors();
}
