//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <common_test_properties.hpp>

#include <boost/container/flat_map.hpp>

#include <map>

int main() {

  run_map_test<std::map<std::string, std::string>>();
  run_map_test<boost::container::flat_map<std::string, std::string>>();

  return boost::report_errors();
}
