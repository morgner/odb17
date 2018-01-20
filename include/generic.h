#ifndef GENERIC_H
#define GENERIC_H

/**
 * @file generic.h
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include <cxxabi.h> // __cxa_demangle

#include <deque>

namespace odb {

/// Forward decleration to befriend with it in other classes
class CThing;
/// The shared_ptr of the entity
using PThing  = std::shared_ptr<CThing>;
/// A container for the shared_ptr's of the entity
using CThings = std::deque<PThing>;

/// Forward decleration to befriend with it in other classes
class CAtom;
/// The shared_ptr of the entity
using PAtom  = std::shared_ptr<CAtom>;
/// A container for the shared_ptr's of the entity
using CAtoms = std::deque<PAtom>;

/// Forward decleration to befriend with it in other classes
class CReason;
/// The shared_ptr of the entity
using PReason  = std::shared_ptr<CReason>;
/// A container for the shared_ptr's of the entity
using CReasons = std::deque<PReason>;

/// Forward decleration to befriend with it in other classes
class CStrand;
/// The shared_ptr of the entity
using PStrand  = std::shared_ptr<CStrand>;
/// A container for the shared_ptr's of the entity
using CStrands = std::deque<PStrand>;

}

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
    }; // class Identifiable


/**
 * zero cost indentation like: cout << spcr<3> << "indented line";
 * indentation depth is N*g_cnIndent;
 *
 * @tparam T The amount of space units to assemble
 */

/// indentation depth factor
auto const g_cnIndent{4};

/// decollisioner to ensure OUR spaces run through OUR output
template<typename T, std::size_t N>
class CIndentWrapper : public std::array<T, N> {};

/// the maker of spaces
template<char C, std::size_t... I>
constexpr auto make_char_array(std::index_sequence<I...>)
    {
    return CIndentWrapper<char, sizeof...(I)>{((void)I, C)...};
    }

/// our spaces
template<std::size_t N>
constexpr auto spcr = make_char_array<' '>(std::make_index_sequence<N * g_cnIndent>{});

/// print our spaces
template<typename T, std::size_t ...I>
std::ostream & print_array(std::ostream & out, std::array<T, sizeof...(I)> const & arr, std::index_sequence<I...>)
    {
    return (out << ... << arr[I]);
    }

/// send our spaces to cout
template<std::size_t N>
std::ostream & operator<<(std::ostream & out, CIndentWrapper<char, N> const & arr)
    {
    return print_array(out, arr, std::make_index_sequence<N>{});
    }


// GENERIC_H
#endif
