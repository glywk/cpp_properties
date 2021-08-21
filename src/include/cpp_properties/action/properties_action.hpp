//=============================================================================
// Copyright (c) 2015-2018 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_PROPERTIES_ACTION_HPP
#define CPP_PROPERTIES_PROPERTIES_ACTION_HPP

#include <cpp_properties/encoding/encoding.hpp>

#include <cpp_properties/lexer.hpp>

/*!
 * helper function to concatenate char range to the given string.
 */
template <typename ForwardTraversalIterator,
          typename CharT = typename boost::iterator_value<ForwardTraversalIterator>::type>
std::basic_string<CharT> &operator+=(std::string &lhs, const boost::iterator_range<ForwardTraversalIterator> &range) {
  lhs += std::basic_string<CharT>(range.begin(), range.end());
  return lhs;
}

/*!
 * the class 'properties_action' is used as a functor for collecting all
 * key-value properties in the analyzed input sequence by identifying the
 * matched tokens as passed from the lexer.
 *
 * Actor have to provide the following structure
 *
 * \code
 *   struct Actor
 *   {
 *       // type of properties container output
 *       typedef *implementation defined* properties_type;
 *       // type of a key-value property
 *       typedef *implementation defined* property_type;
 *
 *       // store the current property and change state to prepare
 *       // the next property read.
 *       void push_back();
 *
 *       // create the current property pair to populate and change state to
 *       // return the current reference.
 *       property_type &current() { return CALL_MEMBER_FN(current_reference)(); }
 *   };
 * \endcode
 */
template <typename Actor> class properties_action {
public:
  // this is an implementation detail specific to boost::bind and doesn't show
  // up in the documentation
  typedef bool result_type;

  // actor type
  typedef Actor actor_type;
  // type of properties container output
  typedef typename actor_type::properties_type properties_type;
  // type of a key-value property
  typedef typename actor_type::property_type property_type;

  properties_action(Actor &actor_reference) : actor(actor_reference) {}

  properties_action(properties_action &&other) : actor(other.actor) {}

  // the function operator gets called for each of the matched tokens
  template <typename Token> bool operator()(Token const &token) const {
    using namespace cpp_properties::token;
    switch (token.id()) {
    case ID_KEY_CHARS:
      actor.current().first += latin1_to_utf8(token.value());
      break;
    case ID_KEY_ESCAPE_CHAR:
      actor.current().first += escape_sequence_to_utf8(token.value());
      break;
    case ID_KEY_UNICODE:
      actor.current().first += decode_to_utf8(token.value());
      break;
    case ID_KEY_CR:
    case ID_KEY_LF:
    case ID_KEY_EOL:
    case ID_SEPARATOR_CR:
    case ID_SEPARATOR_LF:
    case ID_SEPARATOR_EOL:
      actor.push_back();
      break;
    case ID_SEPARATOR_EQUAL:
    case ID_SEPARATOR_COLON:
      // force initialization for useless empty key/value case
      actor.current();
      break;
    case ID_VALUE_SPACES:
    case ID_VALUE_CHARS:
      actor.current().second += latin1_to_utf8(token.value());
      break;
    case ID_VALUE_ESCAPE_CHAR:
      actor.current().second += escape_sequence_to_utf8(token.value());
      break;
    case ID_VALUE_UNICODE:
      actor.current().second += decode_to_utf8(token.value());
      break;
    case ID_VALUE_CR:
    case ID_VALUE_LF:
    case ID_VALUE_EOL:
      actor.push_back();
      break;
    }

    // always continue to tokenize
    return true;
  }

private:
  actor_type &actor;
};

/*!
 * helper function to build a property action based on a visitor fields to hide
 * details.
 */
template <typename Actor> properties_action<Actor> make_action(Actor &actor) { return properties_action<Actor>(actor); }

#endif
