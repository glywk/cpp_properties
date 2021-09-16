//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <common_test_properties.hpp>

#include <boost/container/flat_set.hpp>

#include <set>

typedef std::tuple<std::string, std::string> value_type;

struct less_key {
  bool operator()(const value_type &lhs, const value_type &rhs) const { return std::get<0>(lhs) < std::get<0>(rhs); }
};

int main() {

  run_set_test<std::set<value_type, less_key>>();
  run_set_test<boost::container::flat_set<value_type, less_key>>();

  return boost::report_errors();
}
