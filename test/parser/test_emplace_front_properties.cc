//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <common_test_properties.hpp>

#include <deque>

typedef std::pair<std::string, std::string> value_type;

template <typename T> struct emplacer<std::deque<T>> { typedef cp::emplace_front_adapter_policy emplacer_policy; };

int main() {

  run_emplace_front_test<std::forward_list<value_type>>();
  run_emplace_front_test<std::deque<value_type>>();

  return boost::report_errors();
}
