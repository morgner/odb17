#ifndef GENERIC_H
#define GENERIC_H

/**
 * @file generic.h
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <cxxabi.h> // __cxa_demangle

#include <deque>
#include <set>
#include <regex>
#include <optional>

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
 * @brief Generate IDs
 *
 * Recognizes given ID to prevent ID collition
 *
 * @tparam T The type with which to associate the ID counter
 * @tparam S The starting ID (per T)
 */
template<typename T, size_t S = 0>
static auto idForObjectOf(size_t const nLoad = 0, bool const bLoadNull = false)
    {
    static auto s_nId = S;
    // if nLoad is given > 0, 'return s_nId++' must not be done
    if ((nLoad > 0) || ((nLoad == 0)&&(bLoadNull)))
      {
      if (nLoad >= s_nId) s_nId = nLoad+1;
      return nLoad;
      }
    return s_nId++;
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
        /// @brief Default constructor to let m_nId counting
                 Identifiable() = default;
        
        /// @brief Default copy constructor
        /// @param "" The Source
        explicit Identifiable(Identifiable const &) = default;
        
        /// @brief Default move constructor
        /// @param "" The Source
        explicit Identifiable(Identifiable &&) noexcept = default;
        
        /// @brief Constructor initializing predefined ID (load operation)
        /// @param nId A predefined ID, for loading data sets with IDs
        explicit Identifiable(size_t nId)                              : m_nId(nId)                   { idForObjectOf<T>(nId, nId == 0); }
        
        /// @brief Constructor initializing predefined ID (load operation) with NAME
        /// @param nId A predefined ID, for loading data sets with IDs
        /// @param crsName The name for the object
                 Identifiable(size_t nId, std::string const & crsName) : m_nId(nId), m_sName(crsName) { idForObjectOf<T>(nId, nId == 0); }
        
        /// @brief Constructor initializing NAME, let ID autocount
        /// @param crsName The name for the object
        explicit Identifiable(            std::string const & crsName) :             m_sName(crsName) { }

        /// The unique ID of the instance (counter per type)
        size_t m_nId { idForObjectOf<T>() };
        
        /// The type of the instance
        std::string const m_sType { demangle(typeid(T)) };
        
        /// The human interpretable name of the instance
        std::string m_sName {""};
    }; // class Identifiable


namespace odb {

/**
 * @brief compare operator for Identifiable's ID
 *
 * Universal compare operator comparing class instances derived
 * from class Identifiable by instance member 'ID'
 *
 * @tparam T The type of the elements to compare
 */
template<typename T>
struct lessIdentifiableId
    {
    /// makes the operators to appear polymorph
    using is_transparent = void;

    /**
     * @brief Compares two objects derived from Identifiable
     * @param p1 The element to compare
     * @param p2 The element to compare with
     */
    bool operator()(T const * p1, T const * p2) const
        {
        return p1->m_nId < p2->m_nId;
        }

    /**
     * @brief Compares two objects derived from Identifiable
     * @param p1 The element to compare
     * @param p2 The element to compare with
     */
    bool operator()(T const & p1, T const & p2) const
        {
        return p1->m_nId < p2->m_nId;
        }
    };

/**
 * @brief compare operator for Identifiable's NAME
 *
 * Universal compare operator comparing class instances derived
 * from class Identifiable by instance member 'NAME'
 *
 * @tparam T The type of the elements to compare
 */
template<typename T>
struct lessIdentifiableName
    {
    /// makes the operators to appear polymorphic
    using is_transparent = void;

    /**
     * @brief Compares two objects derived from Identifiable
     * @param p1 The element to compare
     * @param p2 The element to compare with
     */
    bool operator()(T const * p1, T const * p2) const
        {
        return p1->m_sName < p2->m_sName;
        }

    /**
     * @brief Compares two objects derived from Identifiable
     * @param p1 The element to compare
     * @param p2 The element to compare with
     */
    bool operator()(T const & p1, T const & p2) const
        {
        return p1->m_sName < p2->m_sName;
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

/// tag for accessing the corresponding index ID of Identifiable
struct id{};
/// tag for accessing the corresponding index NAME of Identifiable
struct name{};
/// boost multicontainer
using boost::multi_index_container;
/// namespace for boost multi-index
using namespace boost::multi_index;

/// Shared pointer of T (PT = Pointer to T)
template<typename T> using PT = std::shared_ptr<T>;
/// From Identifiable derived class (IT = Inherited for T)
template<typename T> using IT = Identifiable<T>;
/// Multi indexed container of PT<T> (CT = Container of T)
template<typename T> using CT = multi_index_container<
  PT<T>,
  indexed_by<
    /// sort by less<int> on ID
    ordered_unique    <tag<id>,  member<IT<T>, size_t,      &IT<T>::m_nId>>,
    /// sort by less<string> on NAME
    ordered_non_unique<tag<name>,member<IT<T>, std::string, &IT<T>::m_sName>> >>;

/// Forward decleration to befriend with it in other classes
class CThing;
/// The shared_ptr of the entity
using PThing  = PT<CThing>;
/// The type to inherite from (Identifiable<>)
using IThing  = IT<CThing>;
/// A return value for 'optional' returns
using OThing = std::optional<PThing>;
/// A set of PThing's
using SThings = std::set<PThing>;
/// A container for the shared_ptr's of the entity
using CThings = CT<CThing>; 

/// Forward decleration to befriend with it in other classes
class CProperty;
/// The shared_ptr of the entity
using PProperty   = PT<CProperty>;
/// The type to inherite from (Identifiable<>)
using IProperty   = IT<CProperty>;
/// A container for the shared_ptr's of the entity
using CProperties = CT<CProperty>;

/// Forward decleration to befriend with it in other classes
class CReason;
/// The shared_ptr of the entity
using PReason  = PT<CReason>;
/// The type to inherite from (Identifiable<>)
using IReason  = IT<CReason>;
/// A container for the shared_ptr's of the entity
using CReasons = CT<CReason>;

/// Forward decleration to befriend with it in other classes
class CAtom;
/// The shared_ptr of the entity
using PAtom  = PT<CAtom>;
/// The type to inherite from (Identifiable<>)
using IAtom  = IT<CAtom>;
/// A container for the shared_ptr's of the entity
using CAtoms = CT<CAtom>;

/// Forward decleration to befriend with it in other classes
class CStrand;
/// The shared_ptr of the entity
using PStrand  = std::shared_ptr<CStrand>;
/// The type to inherite from (Identifiable<>)
using IStrand  = Identifiable<CStrand>;
/// A container for the shared_ptr's of the entity
using CStrands = std::deque<PStrand>;

} // namespace odb


/**
 * @brief A zero cost spacer
 * 
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

/// OUR spaces
template<std::size_t N>
constexpr auto spcr = make_char_array<' '>(std::make_index_sequence<N * g_cnIndent>{});

/// print OUR spaces
template<typename T, std::size_t ...I>
std::ostream & print_array(std::ostream & out, std::array<T, sizeof...(I)> const & arr, std::index_sequence<I...>)
    {
    return (out << ... << arr[I]);
    }

/// send OUR spaces to std::cout
template<std::size_t N>
std::ostream & operator<<(std::ostream & out, CIndentWrapper<char, N> const & arr)
    {
    return print_array(out, arr, std::make_index_sequence<N>{});
    }

// GENERIC_H
#endif
