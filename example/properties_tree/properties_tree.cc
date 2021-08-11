//=============================================================================
// Copyright (c) 2015-2018 glywk
// https://github.com/glywk
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#include <boost/foreach.hpp>
#include <boost/property_tree/properties_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>
#include <set>
#include <string>

namespace pt = boost::property_tree;

struct configuration {
  void load(const std::string &filename);
  pt::ptree tree;
};

void configuration::load(const std::string &filename) {
  // Parse the properties into the property tree.
  pt::read_properties(filename, tree);
}

/*!
 * the main function simply loads the given file into boost::property_tree structure.
 */
int main(int argc, char *argv[]) {
  try {
    // read input from the given file
    std::string filename(1 == argc ? "full.properties" : argv[1]);
    configuration c;
    c.load(filename);
    for (auto property : c.tree) {
      std::cout << property.first << "=" << property.second.data() << std::endl;
    }
  } catch (std::exception &e) {
    std::cout << "Error: " << e.what() << "\n";
  }
  return 0;
}
