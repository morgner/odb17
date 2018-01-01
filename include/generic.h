#ifndef GENERIC_H
#define GENERIC_H

/*  generic.h
 *
 *  Created on: 26.12.2017
 *      Author: felix morgner
 */

// #include <vector>   // vector
// #include <string>   // string
// #include <typeinfo> // type_info, required for typeid to be valid
#include <cxxabi.h> // __cxa_demangle
// #include <memory>   // unique_ptr

/**
 * Demangle C++ types into something human readable
 *
 * @param type A std::type_info object holding a C++ type
 */
inline auto demangle(std::type_info const & type)
  {
  auto status = 0;
  auto name = std::unique_ptr<char, void (*)(void *)>(
      abi::__cxa_demangle(type.name(), 0, 0, &status), std::free);
  return std::string{ name.get() };
  }

/**
 * Generate IDs
 *
 * @tparam T The type with which to associate the ID counter
 * @tparam S The starting ID
 */
template<typename T, unsigned long long S = 0>
static auto idForObjectOf()
  {
  static auto id = S;
  return id++;
  }

/**
 * A baseclass for identifiable objects
 *
 * @tparam T The type which should receive an ID
 */
template<typename T>
class Identifiable
  {
  public:
    /**
     * The 'per-type' object ID
     */
    unsigned long long const id
      {
      idForObjectOf<T>()
      };

    /**
     * The name of the actual type
     */
    std::string const type
      {
      demangle(typeid(T))
      };
  };


// GENERIC_H
#endif
