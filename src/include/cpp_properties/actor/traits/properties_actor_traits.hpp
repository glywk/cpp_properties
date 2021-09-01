//=============================================================================
// Copyright (c) 2015-2021 glywk
// https://github.com/glywk
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================
#ifndef CPP_PROPERTIES_PROPERTIES_ACTOR_TRAITS_HPP
#define CPP_PROPERTIES_PROPERTIES_ACTOR_TRAITS_HPP

namespace cpp_properties {

struct emplace_policy {
public:
  template <typename T, typename K, typename V> static void emplace(T &properties, K &&key, V &&value) {
    properties.emplace(std::forward<K>(key), std::forward<V>(value));
  }
};

struct emplace_back_adapter_policy {
public:
  template <typename T, typename K, typename V> static void emplace(T &properties, K &&key, V &&value) {
    properties.emplace_back(std::forward<K>(key), std::forward<V>(value));
  }
};

struct emplace_front_adapter_policy {
public:
  template <typename T, typename K, typename V> static void emplace(T &properties, K &&key, V &&value) {
    properties.emplace_front(std::forward<K>(key), std::forward<V>(value));
  }
};

/*!
 * the key-value property traits to provide to visit the abstract syntax tree.
 * \tparam T type of properties output container
 * \tparam S must be assignable with std::basic_string<>.
 * \tparam E functor to emplace key, value in output properties.
 */
template <typename T, typename S = std::string, typename E = emplace_policy> struct properties_actor_traits {
  /// type of properties output container
  typedef T properties_type;
  /// type of key property accepted by emplace method
  typedef S key_type;
  /// type of value property accepted by emplace method
  typedef S mapped_type;
  /// functor to emplace key, value in output properties.
  typedef E emplacer_policy;
};

} // namespace cpp_properties

#endif
