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

#include "generic.h"


namespace odb {

using namespace std::string_literals;

std::string const g_csNameUntypedObject("unnamedObject");

// using CObject = std::vector<CAtom>;



using CReason = std::string;

class CObject;

class CLink
{
public:

  CLink(std::shared_ptr<CObject> poObject,
        std::shared_ptr<CReason> poReason)
  : m_poLinkedTo(poObject),
  m_poReason  (poReason)
  {}

  std::shared_ptr<CObject> m_poLinkedTo = nullptr;
  std::shared_ptr<CReason> m_poReason   = nullptr;
};



class CObject : public std::vector<CAtom>
{
public:
  CObject() = default;
  CObject(const std::string& crsName)
  : m_sName(crsName)
  {}

  const std::string& NameGet() { return m_sName; } const

  bool Link(std::shared_ptr<CObject> poObject,
            std::shared_ptr<CReason> poReason)
  {
  m_voLinksTo.emplace_back(CLink(poObject, poReason));
  return true;
  }

  std::string m_sName = "unnamed";

private:
  std::vector<CLink> m_voLinksTo;
};

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

