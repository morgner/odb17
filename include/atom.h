#ifndef CATOM_H
#define CATOM_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

//#include <deque>
//#include <string>
#include <memory>

#include <iostream>
#include <vector>
//#include <array>

#include "generic.h"
#include "thing.h"


namespace odb {

using namespace std::string_literals;


/// outputs a std::vector like this: { 1, 2, 3 }
template<typename T>
std::ostream& operator<< (std::ostream & out, std::vector<T> const & crContainer)
    {
    bool b{false};
    out << '{';
    for (auto const & a:crContainer)
        {
        if (b) { out << ","; } else { b = true; }
        out << a;
        }
    out << '}';
    return out;
    } // std::ostream& operator<< (std::ostream& out, const std::vector<T>& v)

/// outputs a std::array like this: { 1, 2, 3 }
template <typename T, std::size_t N>
std::ostream& operator<< (std::ostream & out, std::array<T, N> const & crContainer)
    {
    bool b(false);
    out << '{';
    for (auto const & a:crContainer)
        {
        if (b) { out << ','; } else { b = true; }
        out << a;
        }
    out << '}';
    return out;
    } // std::ostream& operator<< (std::ostream& out, const std::array<T, N>& v)

/// forward declarations to befriend with
class COdb;

/**
 * An Atom, from whitch Things are built from
 */
class CAtom : public std::enable_shared_from_this<CAtom>,
              public Identifiable<CAtom>
    {
    friend COdb;

    public:
        static constexpr auto g_csNameUnnamedAtom{"unnamedAtom"};
        static           bool s_bDebug;
    public:

        template <typename T, typename U>
        struct decay_equiv :
            std::is_same<typename std::decay<T>::type, U>::type {};


        template<typename T>
        CAtom(T tAtomData,
            std::string const & crsName   = ""s,
            std::string const & crsPrefix = ""s,
            std::string const & crsSuffix = ""s,
            std::string const & crsFormat = ""s)
            : m_pAtomData(new SAtomData<T>(std::move(tAtomData))),
              m_sName  (crsName.length() ? crsName : g_csNameUnnamedAtom),
              m_sPrefix(crsPrefix),
              m_sSuffix(crsSuffix),
              m_sFormat(crsFormat)
            {
// not bad
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
/*
// research
            std::cout << "new atom for ";
            if (std::is_integral<T>::value)                      std::cout << "integral";
            else if (std::is_floating_point<T>::value)           std::cout << "floating";
            else if (std::is_array<T>::value)                    std::cout << "array";
//            else if (std::is_vector<T>::value)                   std::cout << "vector";
            else if (std::is_convertible<T, const char*>::value) std::cout << "const char*";
            else if (std::is_convertible<T, std::string>::value) std::cout << "string";
            else if (std::is_class<T>::value)                    std::cout << "class";
            else std::cout << "UNKNOWN TYPE";
            std::cout << ": " << *this << " (" << tAtomData << ')' << "\tname: " << m_sName << '\n';
*/
            }

    virtual ~CAtom() = default;

    void clear()
        {
        m_spoThingsRelating.clear();
        }

    friend void print(CAtoms const & crContainer)
//void print(odb::CAtoms & crContainer)
    {
    for (auto const & e:crContainer)
        {
        std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->m_sName << '\t' << " data: " << *e << " odbrep:";
        e->print_atom_data_formated(std::cout);
        std::cout  << " (" << e.use_count() << ")\n";
//        std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << '\n';
        }
    }

    friend std::ostream& operator << (std::ostream & out, CAtom const & croAtom)
        {
        croAtom.m_pAtomData->ToStream(out);
/*
        for ( auto const & e:croAtom.m_qpoThingsRelating )
          {
          out << '\n' << " used by thing: " << e;
          }
*/
        return out;
        } // operator << (...)

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

    void print_atom_data_formated(std::ostream& out) const
        {
        if (m_sPrefix.length() > 0) out << m_sPrefix << " ";
        out << *this;
        if (m_sSuffix.length() > 0) out << " " << m_sSuffix;
        }

    auto RelatingThingAdd(PThing poThing)
        {
//        m_qpoThingsRelating.push_back(poThing);
        m_spoThingsRelating.insert(poThing);
        return poThing;
        }

    auto RelatingThingRemove(PThing poThing)
        {
        return m_spoThingsRelating.erase(poThing);
        }

    /// todo: decide which way:
    public:
        std::string m_sName  {g_csNameUnnamedAtom};
    protected:
        std::string m_sFormat{""s};
        std::string m_sPrefix{""s};
        std::string m_sSuffix{""s};

        struct lessPThing
            {
            bool operator()(PThing const &, PThing const & ) const;
            };
        std::set<PThing, lessPThing> m_spoThingsRelating;

    /// start of data implementation --------------------------
    struct SAtomDataConcept
        {
        virtual ~SAtomDataConcept() = default;
        virtual void ToStream(std::ostream&) const = 0;
        }; // struct SAtomDataConcept

    template<typename T>
    struct SAtomData : SAtomDataConcept
        {
        SAtomData(T tData) : m_tData(std::move(tData))
            {
            static_assert
                (
                    (
                    !std::is_pointer<decltype(tData)>::value || std::is_convertible<T, const char*>::value
                    ), "arrays and pointers not supported"
                );
            }

        void ToStream(std::ostream & roOut) const
            {
            roOut << m_tData;
            }

        T m_tData;
        }; // struct SAtomData

    std::unique_ptr<const SAtomDataConcept> m_pAtomData;
    }; // class CAtom

} // namespace odb

// CATOM_H
#endif
