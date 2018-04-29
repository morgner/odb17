#ifndef CATOM_H
#define CATOM_H

/**
 * @file atom.h
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include <memory>

#include <iostream>
#include <vector>
#include <forward_list> //
#include <list>         //
#include <set>          //

#include "generic.h"
#include "node.h"


namespace odb {

using namespace std::string_literals;

/**
    @brief  Template specification: Outputs a std::forward_list like this: {1,2,3}
    @tparam T The type of the collected objects
    @param  ros The output stream so send the objects to
    @param  crContainer The container holding the objects to output
 */
template<typename T>
std::ostream& operator<< (std::ostream & ros, std::forward_list<T> const & crContainer)
    {
    bool b{false};
    ros << '{';
    for (auto const & a:crContainer)
        {
        if (b) { ros << ","; } else { b = true; }
        ros << a;
        }
    ros << '}';
    return ros;
    }

/**
    @brief  Template specification: Outputs a std::list like this: {1,2,3}
    @tparam T The type of the collected objects
    @param  ros The output stream so send the objects to
    @param  crContainer The container holding the objects to output
 */
template<typename T>
std::ostream& operator<< (std::ostream & ros, std::list<T> const & crContainer)
    {
    bool b{false};
    ros << '{';
    for (auto const & a:crContainer)
        {
        if (b) { ros << ","; } else { b = true; }
        ros << a;
        }
    ros << '}';
    return ros;
    }

/**
    @brief  Template specification: Outputs a std::set like this: {1,2,3}
    @tparam T The type of the collected objects
    @param  ros The output stream so send the objects to
    @param  crContainer The container holding the objects to output
 */
template<typename T>
std::ostream& operator<< (std::ostream & ros, std::set<T> const & crContainer)
    {
    bool b{false};
    ros << '{';
    for (auto const & a:crContainer)
        {
        if (b) { ros << ","; } else { b = true; }
        ros << a;
        }
    ros << '}';
    return ros;
    }

/**
    @brief  Template specification: Outputs a std::set like this: {1,2,3}
    @tparam T The type of the collected objects
    @param  ros The output stream so send the objects to
    @param  crContainer The container holding the objects to output
 */
template<typename T>
std::ostream& operator<< (std::ostream & ros, std::multiset<T> const & crContainer)
    {
    bool b{false};
    ros << '{';
    for (auto const & a:crContainer)
        {
        if (b) { ros << ","; } else { b = true; }
        ros << a;
        }
    ros << '}';
    return ros;
    }

/**
    @brief  Template specification: Outputs a std::deque like this: {1,2,3}
    @tparam T The type of the collected objects
    @param  ros The output stream so send the objects to
    @param  crContainer The container holding the objects to output
 */
template<typename T>
std::ostream& operator<< (std::ostream & ros, std::deque<T> const & crContainer)
    {
    bool b{false};
    ros << '{';
    for (auto const & a:crContainer)
        {
        if (b) { ros << ","; } else { b = true; }
        ros << a;
        }
    ros << '}';
    return ros;
    } // std::ostream& operator<< (std::ostream& out, const std::deque<T>& v)

/**
    @brief  Template specification: Outputs a std::vector like this: {1,2,3}
    @tparam T The type of the collected objects
    @param  ros The output stream so send the objects to
    @param  crContainer The container holding the objects to output
 */
template<typename T>
std::ostream& operator<< (std::ostream & ros, std::vector<T> const & crContainer)
    {
    bool b{false};
    ros << '{';
    for (auto const & a:crContainer)
        {
        if (b) { ros << ","; } else { b = true; }
        ros << a;
        }
    ros << '}';
    return ros;
    } // std::ostream& operator<< (std::ostream& out, const std::vector<T>& v)

/**
    @brief  Template specification: Outputs a std::array like this: {1,2,3}
    @tparam T The type of the collected objects
    @param  ros The output stream so send the objects to
    @param  crContainer The container holding the objects to output
 */
template <typename T, std::size_t N>
std::ostream& operator<< (std::ostream & ros, std::array<T, N> const & crContainer)
    {
    bool b(false);
    ros << '{';
    for (auto const & a:crContainer)
        {
        if (b) { ros << ','; } else { b = true; }
        ros << a;
        }
    ros << '}';
    return ros;
    } // std::ostream& operator<< (std::ostream& out, const std::array<T, N>& v)

/// forward declarations to befriend with
class COdb;

using IAtom = Identifiable<CAtom>;

/**
 @brief An Atom is a data field for a CThing

 @par Sample Code goes here
 @rst
 .. code-block:: cpp

    #include <odb>
 @endrst
 */
class CAtom : public std::enable_shared_from_this<CAtom>,
              public IAtom
    {
    friend COdb;

    public:
        /// The name of an unnamed atom
        static constexpr auto s_csNameUnnamedAtom{"unnamedAtom"};
        /// Switch to enable/disable debug information output, regarding CAtom
        static constexpr bool s_bDebug{false};
    public:
        /// Compares the type of a variable with a chosen type for similarity,
        /// e.g:
        ///  - if ( decay_equiv<T, int>::value ) ...
        template <typename T, typename U>
        struct decay_equiv :
            std::is_same<typename std::decay<T>::type, U>::type {};

        /**
            @brief Constructor able to receive data of maany types
            @param tAtomData The data unit to encapsulate
            @param crsName The name for the atom
            @param crsPrefix The prefix for user output
            @param crsSuffix The suffix for user output
            @param crsFormat The format for user output
         */
        template<typename T>
        CAtom(T tAtomData,
#ifdef __DOXYGEN__
            std::string const & crsName   = "",
            std::string const & crsPrefix = "",
            std::string const & crsSuffix = "",
            std::string const & crsFormat = "")
#else
            std::string const & crsName   = ""s,
            std::string const & crsPrefix = ""s,
            std::string const & crsSuffix = ""s,
            std::string const & crsFormat = ""s)
#endif
            : m_pAtomData(new SAtomData<T>(std::move(tAtomData))),
              IAtom(crsName.length() ? crsName : s_csNameUnnamedAtom),
              m_sPrefix(crsPrefix),
              m_sSuffix(crsSuffix),
              m_sFormat(crsFormat)
            {
            if ( CAtom::s_bDebug )
                {
                std::cout << "new atom for ";
                if      ( decay_equiv<T, char>::value )                { std::cout << "char "; }
                else if ( decay_equiv<T, short>::value )               { std::cout << "short "; }
                else if ( decay_equiv<T, int>::value )                 { std::cout << "int "; }
                else if ( decay_equiv<T, long>::value )                { std::cout << "long "; }
                else if ( decay_equiv<T, unsigned>::value )            { std::cout << "unsigned "; }
                else if ( decay_equiv<T, long long>::value )           { std::cout << "long_long "; }
                else if ( decay_equiv<T, std::string>::value )         { std::cout << "std::string "; }
                else if ( decay_equiv<T, float>::value )               { std::cout << "float "; }
                else if ( decay_equiv<T, double>::value )              { std::cout << "double "; }

                else if ( std::is_convertible<T, const char*>::value)  { std::cout << "const char*"; }
                else if ( std::is_class<T>::value)                     { std::cout << "class"; }

                else                                                   { std::cout << "UNKNOWN TYPE"; }

                std::cout << ": " << *this << " (" << tAtomData << ')' << "\tname: " << m_sName << '\n';
                }
            }

        /**
            @brief Constructor able to receive data of maany types
            @param nId The predefiined ID if loading a dataset
            @param tAtomData The data unit to encapsulate
            @param crsName The name for the atom
            @param crsPrefix The prefix for user output
            @param crsSuffix The suffix for user output
            @param crsFormat The format for user output
         */
        template<typename T>
        CAtom(size_t nId,
            T tAtomData,
#ifdef __DOXYGEN__
            std::string const & crsName   = "",
            std::string const & crsPrefix = "",
            std::string const & crsSuffix = "",
            std::string const & crsFormat = "")
#else
            std::string const & crsName   = ""s,
            std::string const & crsPrefix = ""s,
            std::string const & crsSuffix = ""s,
            std::string const & crsFormat = ""s)
#endif
            : m_pAtomData(new SAtomData<T>(std::move(tAtomData))),
              IAtom(nId, crsName.length() ? crsName : s_csNameUnnamedAtom),
              m_sPrefix(crsPrefix),
              m_sSuffix(crsSuffix),
              m_sFormat(crsFormat)
            {
            if ( CAtom::s_bDebug )
                {
                std::cout << "new atom for ";
                if      ( decay_equiv<T, char>::value )                { std::cout << "char "; }
                else if ( decay_equiv<T, short>::value )               { std::cout << "short "; }
                else if ( decay_equiv<T, int>::value )                 { std::cout << "int "; }
                else if ( decay_equiv<T, long>::value )                { std::cout << "long "; }
                else if ( decay_equiv<T, unsigned>::value )            { std::cout << "unsigned "; }
                else if ( decay_equiv<T, long long>::value )           { std::cout << "long_long "; }
                else if ( decay_equiv<T, std::string>::value )         { std::cout << "std::string "; }
                else if ( decay_equiv<T, float>::value )               { std::cout << "float "; }
                else if ( decay_equiv<T, double>::value )              { std::cout << "double "; }

                else if ( std::is_convertible<T, const char*>::value)  { std::cout << "const char*"; }
                else if ( std::is_class<T>::value)                     { std::cout << "class"; }

                else                                                   { std::cout << "UNKNOWN TYPE"; }

                std::cout << ": " << *this << " (" << tAtomData << ')' << "\tname: " << m_sName << '\n';
                }
            }

             /// Destruction as usual (=default)
    virtual ~CAtom() noexcept = default;

    /**
        @brief Remove all links between all objects

        This is necessary to enable freeing of all memory ressources. So we
        become able to put valgrind to use
     */
    void clear()
        {
        m_spoThingsRelating.clear();
        }

    /// friend function to print the atom instance in an inforamtional manner
    friend void print(CAtoms const & crContainer);

    /// sends the data of the atom to the given ostream
    friend std::ostream& operator << (std::ostream & ros, CAtom const & croAtom)
        {
        croAtom.m_pAtomData->ToStream(ros);
/*
        for ( auto const & e:croAtom.m_qpoThingsRelating )
          {
          ros << '\n' << " used by thing: " << e;
          }
*/
        return ros;
        } // operator << (...)

    /// todo: output the instance xml formated
    void print_xml(std::ostream& out, size_t const cnDepth, bool bFormated = false) const
        {
        out << std::string(cnDepth, ' ') << "<" << m_sName << ">";

        if (bFormated)
            {
            print_atom_data_formated(out);
            }
        else
            {
            out << *this;
            }

        out << "</" << m_sName << ">" << std::endl;
        } // void print_xml(...)

    /// Prints the content of the instance for UI use (well formated)
    void print_atom_data_formated(std::ostream& out) const
        {
        if (m_sPrefix.length() > 0) out << m_sPrefix << " ";
        out << *this;
        if (m_sSuffix.length() > 0) out << " " << m_sSuffix;
        }

    /// Adds the backlink from the atom to a thing
    /// @param poThing Inform a thing about being linked from another thing
    auto RelatingThingAdd(PNode poThing)
        {
        m_spoThingsRelating.insert(poThing);
        return poThing;
        }

    /// Removes the backlink from the atom to a thing
    /// @param poThing Inform a thing about no more being linked from another thing
    auto RelatingThingSub(PNode poThing)
        {
        return m_spoThingsRelating.erase(poThing);
        }

    /// returns if the instance is 'free'
    auto IsUnUsed()
	    {
	    return (0 == m_spoThingsRelating.size());
	    }

    protected:
        /// The UI output format for the atom
        std::string m_sFormat{""s};
        /// The UI prefix (if any) for the atom
        std::string m_sPrefix{""s};
        /// The UI suffix (if any) for the atom
        std::string m_sSuffix{""s};
        /// CThing's Relating to this CAtom
        std::set<PNode, lessIdentifiableId<PNode>>  m_spoThingsRelating;

    /// start of data implementation
    struct SAtomDataConcept
        {
        virtual ~SAtomDataConcept() = default;
        /// Will send the data of the atom to the given stream
        virtual void ToStream(std::ostream&) const = 0;
        }; // struct SAtomDataConcept

    /// The templated data structure to hold an arbitrary data element
    template<typename T>
    struct SAtomData : SAtomDataConcept
        {
        /**
            @brief The function to deal with the arbitrary data element
            @param tData The data element to hold
         */
        SAtomData(T tData)
            : m_tData(std::move(tData))
            {
            static_assert
                (
                    (
                    !std::is_pointer<decltype(tData)>::value ||
                     std::is_convertible<T, const char*>::value
                    ), "arrays and pointers not supported"
                );
            }

        /**
            @brief Send the data element to std::ostream

            Helper function to break the boundary between non uniform data
            content of the atom instance and the uniform output expectation
         */
        void ToStream(std::ostream & ros) const
            {
            ros << m_tData;
            }

        /// @brief The decalartion of the data element of type T
        T m_tData;
        }; // struct SAtomData

    /// @brief The pointer and holder of the data element of type T
    std::unique_ptr<const SAtomDataConcept> m_pAtomData;
    }; // class CAtom

} // namespace odb

// CATOM_H
#endif
