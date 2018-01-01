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


namespace odb {

using namespace std::string_literals;


template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v)
{
  bool b(false);
  out << '{';
  for (const auto& value : v)
    {
    if (b) out << ","; else b = true;
    out << value;
    }
  out << '}';
  return out;
} // std::ostream& operator<< (std::ostream& out, const std::vector<T>& v)

template <typename T, std::size_t N>
std::ostream& operator<< (std::ostream& out, const std::array<T, N>& v)
{
  bool b(false);
  out << '{';
  for (const auto& value : v)
    {
    if (b) out << ","; else b = true;
    out << value;
    }
  out << '}';
  return out;
} // std::ostream& operator<< (std::ostream& out, const std::array<T, N>& v)


/**
 * An Atom, from whitch Things are built from
 */
class CAtom : public Identifiable<CAtom>
{
  public:
    static constexpr auto g_csNameUnnamedAtom{"unnamedAtom"};
  public:

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
      std::cout << "new atom for ";
      if (std::is_integral<T>::value)                      std::cout << "integral";
      else if (std::is_floating_point<T>::value)           std::cout << "floating";
      else if (std::is_array<T>::value)                    std::cout << "array";
//    else if (std::is_vector<T>::value)                   std::cout << "vector";
      else if (std::is_convertible<T, const char*>::value) std::cout << "const char*";
      else if (std::is_convertible<T, std::string>::value) std::cout << "string";
      else if (std::is_class<T>::value)                    std::cout << "class";
      else std::cout << "UNKNOWN TYPE";
      std::cout << ": " << *this << " (" << tAtomData << ')' << "\tname: " << m_sName << '\n';
      }

      std::string const & NameGet() { return m_sName; }


      friend std::ostream& operator << (std::ostream& out, CAtom const & croAtom)
        {
        croAtom.m_pAtomData->ToStream(out);
/*
        for ( auto const & e:croAtom.m_qpoThingsRelating )
          {
          out << '\n' << " use by thing: " << e;
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
      m_qpoThingsRelating.push_back(poThing);
      return poThing;
      }

  protected:
    std::string m_sName  {g_csNameUnnamedAtom};
    std::string m_sFormat{""s};
    std::string m_sPrefix{""s};
    std::string m_sSuffix{""s};

    CThings m_qpoThingsRelating;


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

    void ToStream(std::ostream& roOut) const
      {
      roOut << m_tData;
      }
    T m_tData;
    }; // struct SAtomData

  std::unique_ptr<const SAtomDataConcept> m_pAtomData;
  }; // class CAtom

inline auto print(CAtoms & container)
  {
  for (auto && e : container)
    {
    std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << " odbrep:";
    e->print_atom_data_formated(std::cout);
    std::cout << '\n';
//    std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << '\n';
    }
  }


} // namespace odb

// CATOM_H
#endif

