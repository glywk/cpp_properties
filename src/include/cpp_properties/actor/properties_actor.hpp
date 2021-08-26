//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_PROPERTIES_ACTOR_HPP
#define CPP_PROPERTIES_PROPERTIES_ACTOR_HPP

namespace cpp_properties {

// define macro for callback member function
#define CALL_MEMBER_FN(member_function) ((*this).*(member_function))

/*!
 * the properties actor is used as a functor
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

private:
  typedef typename Traits::emplacer_policy emplacer_policy;

public:
  ~properties_actor() {
    if (property) {
      emplacer_policy::emplace(properties, std::move(property->first), std::move(property->second));
    }
  }
  properties_actor(properties_type &properties_reference)
      : properties(properties_reference), property(), current_reference(&properties_actor::allocate) {}

  /*!
   * store the current property and change state to prepare
   * the next property read.
   */
  void push_back() {
    emplacer_policy::emplace(properties, std::move(property->first), std::move(property->second));
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

} // namespace cpp_properties

#endif