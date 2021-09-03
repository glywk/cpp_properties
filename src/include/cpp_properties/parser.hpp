//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_PARSER_HPP
#define CPP_PROPERTIES_PARSER_HPP

#include <cpp_properties/lexer.hpp>

namespace cpp_properties {
template <typename Actor> class properties_action;
template <typename Traits> class properties_actor;

/*!
 * tokenize and parse the text and populate the output container
 */
template <typename T, typename Iterator, typename Traits = properties_actor_traits<T>,
          typename Actor = properties_actor<Traits>, typename Action = properties_action<Actor>>
bool parse(Iterator first, Iterator last, T &cpp_properties) {
  // create the token definition instance needed to invoke the lexical analyzer
  using lexer_type = lex::lexertl::lexer<lex::lexertl::token<Iterator>>;
  cpp_properties_lexer<lexer_type> lexer;

  Actor actor(cpp_properties);
  Action action = std::move(make_action(actor));

  return lex::tokenize(
      first, last, lexer,
      [&action](const typename cpp_properties_lexer<lexer_type>::token_type &token) { return action(token); });
}

} // namespace cpp_properties
#endif
