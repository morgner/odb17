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
#include <set>

namespace odb {

/**
 * @brief compare operator for two Identifiable's
 *
 * Universal compare operator comparing class instances derived
 * from class Identifiable by instance member 'id'
 *
 * @tparam T The type of the elements to compare
 */
template<typename T>
struct lessIdentifiableId
    {
    using is_transparent = void;

    /**
     * @brief Compares two objects derived from Identifiable
     * @param p1 The element to compare
     * @param p2 The element to compare with
     */
    bool operator()(T const & p1, T const & p2) const
        {
        return p1->id < p2->id;
        }
    bool operator()(size_t const & id, T const & p) const
        {
        return id < p->id;
        }
    bool operator()(T const & p, size_t const & id) const
        {
        return p->id < id;
        }
    };

template<typename T>
struct lessIdentifiableName
    {
    using is_transparent = void;

    /**
     * @brief Compares two objects derived from Identifiable
     * @param p1 The element to compare
     * @param p2 The element to compare with
     */
    bool operator()(T const & p1, T const & p2) const
        {
        return p1->m_sName < p2->m_sName;
        }
    bool operator()(std::string const & crsName, T const & p) const
        {
        return crsName < p->m_sName;
        }
    bool operator()(T const & p, std::string const & crsName) const
        {
        return p->m_sName < crsName;
        }
    };

/* 2
    auto compare = [](PThing const p1, PThing const p2){return p1->id < p2->id;};
    std::set<PThing, decltype(compare)> m_spoThingsRelating{std::move(compare)};
*/

/* 3
template<typename Type, typename ... Comparator>
auto make_set(Comparator && ... comparator)
  {
  struct Compare : std::decay_t<Comparator>...
    {
    using std::decay_t<Comparator>::operator()...;
    using is_transparent = int;
    };
  return std::set<Type, Compare>{Compare{std::forward<Comparator>(comparator)...}};
  }
auto m_spoThingsRelating = make_set<PThing>(
  [](PThing const p1, PThing const p2){return p1->id < p2->id;}
  [](long   const c , PThing const p2){return c      < p2->id;},
  [](PThing const p1, long   const c ){return p1->id < c;     }
  );
*/


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
class CProperty;
/// The shared_ptr of the entity
using PProperty  = std::shared_ptr<CProperty>;
/// A container for the shared_ptr's of the entity
// using CProperties = std::deque<PProperty>;
using CProperties = std::set<PProperty, lessIdentifiableName<PProperty>>;

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
template<typename T, size_t S = 0>
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
        size_t const id
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
auto constexpr g_cnIndent{4};

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
