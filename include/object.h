#ifndef COBJECT_H
#define COBJECT_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <deque>
#include <string>
#include <memory>

#include <iostream>
#include <vector>
#include <array>

#include "atom.h"
#include "generic.h"


namespace odb {

// using namespace std::string_literals;


class CObject;

class CReason : public Identifiable<CReason>
  {
  public:
    static constexpr auto g_csNameUnnamedReason("unnamedReason");
  public:
	  CReason();
	  CReason(std::string const & crsName)
	    : m_sName(crsName)
	    {
	    }

	  std::string const & operator = (std::string const & sName)
	    {
	    return m_sName = sName;
	    }

	  operator std::string const & ()
	    {
	    return m_sName;
	    }

  protected:
    std::string m_sName{ g_csNameUntypedReason };

  }; // class CReason

using PReason = std::shared_ptr<CReason>;
using CReasons = std::deque<PReason>;

using PObject = std::shared_ptr<CObject>;
using CObjects = std::deque<PObject>;

class CLink : public Identifiable<CLink>
  {
  public:
    CLink() = delete;
    CLink(PObject poObject,
	  PReason poReason)
    : m_poLinkedTo(poObject),
      m_poReason  (poReason)
    {}

  protected:
    PObject m_poLinkedTo = nullptr;
    PReason m_poReason   = nullptr;

}; // class CLink

using PLink = std::shared_ptr<CLink>;
using CLinks = std::deque<PLink>;



class CObject : public Identifiable<CObject>
  {
  public:
    static constexpr auto g_csNameUnnamedObject("unnamedObject");
  public:

             CObject() = default;
             CObject(const std::string& crsName)
               : m_sName(crsName)
               {
	       }

  const std::string& NameGet() { return m_sName; } const

  bool Link(PObject poObject,
            PReason poReason)
  {
  m_vpoLinksTo.emplace_back(CLink(poObject, poReason));
  return true;
  }

protected:
  std::string        m_sName{g_csNameUnnamedObject};
  std::vector<PAtom> m_vpoAtoms;
  std::vector<CLink> m_vpoLinksTo;

}; // class CObject



void print_xml(const CObject& x, std::ostream& out, const size_t cnDepth)
{
  out << std::string(cnDepth, ' ') << "<object name=\"" << x.m_sName << "\">" << std::endl;
  for (const auto& e : x) { e.print_xml(out, cnDepth+2, true); }
  out << std::string(cnDepth, ' ') << "</object>" << std::endl;
} // draw document_t()


/*
int main(int argc, const char * argv[])
{
  CObject oObject("flyer");

  const std::string cs("constness");
  int a[10] = {1, 2, 3, 4, 5, 6};
  std::vector<int> v{1, 2, 3};
  std::array<char, 3> w{'a','b','c'};

  oObject.emplace_back(0, "size", "V", "dl");
  oObject.emplace_back("hallo", "world");
  oObject.emplace_back(1, "mass", "", "kg");
  oObject.emplace_back(std::string(8, '-'), "line");
  oObject.emplace_back(100.2, "round", "", "%");
  oObject.emplace_back(cs);
  //  oObject.emplace_back(a);
  oObject.emplace_back(v, "rasenhalme");
  oObject.emplace_back(w, "letters");

  print_xml(oObject, std::cout, 0);

  std::shared_ptr<CObject> po00(new CObject("Terry Pratchett"));
  std::shared_ptr<CObject> po01(new CObject("Nigel Planer"));

  CObject o10("The Colour of Magic");
  o10.emplace_back("9780753107089", "Six Audio-CD");
  o10.emplace_back("9780753140246", "One MP3-CD");

  o10.Link(po00, std::shared_ptr<CReason>(new CReason("Author" )));
  o10.Link(po01, std::shared_ptr<CReason>(new CReason("Read by")));

  const CAtom o(0, "null");
  std::cout << o << std::endl;

  //  Copy-Contructor ???
  //  CAtom c = o;

  return 0;
}
*/



} // namespace odb

// COBJECT_H
#endif

