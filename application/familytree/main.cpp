/*  main.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <iostream> // cout

#include "thing.h"
#include "atom.h"

#include "odb.h"

#include "generic.h"

/**
 * Helper function to print a vector of objects
 */
auto print(std::vector<odb::CAtom> const & vec)
  {
  for (auto && e : vec)
    {
    std::cout << e.type << " id: " << e.id << " data: " << e.m_tData << '\n';
    }
  }

template<typename T>
auto print(std::vector<T> const & vec)
  {
  for (auto && e : vec)
    {
    std::cout << e.type << " id: " << e.id << '\n';
    }
  }

template<typename T>
auto print(std::deque<T> const & deq)
  {
  for (auto && e : deq)
    {
//  std::cout << e->type << " id: " << e->id << " data: " << e->m_tData << '\n';
    std::cout << e->type << " id: " << e->id << '\n';
    }
  }

odb::COdb g_oOdb{};

/**
 * Demo main program
 */
int main()
  {
/*
  odb::CThings things;
  things.push_back( std::make_shared<odb::CThing>() );
  things.push_back( std::make_shared<odb::CThing>() );

  auto a = odb::CAtom<int>(3);
  print(std::vector<odb::CThing>{ 3 });
  print(std::vector<odb::CAtom<int>>{3,2,1,2,3});
  print(std::vector<odb::CAtom<char>>{'h','e','l','o',' ','m','y',' ','o','d','b'});
*/

  odb::CAtoms  atoms;
  atoms.push_back( std::make_shared<odb::CAtom>(4) );
  atoms.push_back( g_oOdb.MakeAtom(3) );
  atoms.push_back( g_oOdb.MakeAtom(2) );
  atoms.push_back( g_oOdb.MakeAtom(1) );
  atoms.push_back( g_oOdb.MakeAtom(2) );
  atoms.push_back( g_oOdb.MakeAtom(3) );

/*
  atomsChar.push_back( std::make_shared<odb::CAtom<char>>('Q') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('Q') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('u') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('i') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('r') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('k') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('s') );
*/
  odb::CThings things;
  things.push_back( std::make_shared<odb::CThing>() );
  things.push_back( g_oOdb.MakeThing("Wundertüte") );
  things.push_back( g_oOdb.MakeThing("Kinderüberraschung") );
  things.push_back( g_oOdb.MakeThing("No Poduct") );
  things.push_back( g_oOdb.MakeThing("Harmlos Tüte") );
  
/*
  print(atomsInt);
  print(atomsChar);
  print(things);
*/

  g_oOdb.Dump();
  }

