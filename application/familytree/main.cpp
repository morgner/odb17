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
auto print(std::vector<modb::CAtom<int>> const & vec)
  {
  for (auto && e : vec)
    {
//    std::cout << e.type << " id: " << e.id << " data: " << e.m_tData << '\n';
    std::cout << e.type << " id: " << e.id << '\n';
    }
  }

auto print(std::vector<modb::CAtom<char>> const & vec)
  {
  for (auto && e : vec)
    {
//    std::cout << e.type << " id: " << e.id << " data: " << e.m_tData << '\n';
    std::cout << e.type << " id: " << e.id << '\n';
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

modb::COdb g_oOdb{};

/**
 * Demo main program
 */
int main()
  {
/*
  modb::CThings things;
  things.push_back( std::make_shared<modb::CThing>() );
  things.push_back( std::make_shared<modb::CThing>() );

  auto a = modb::CAtom<int>(3);
  print(std::vector<modb::CThing>{ 3 });
  print(std::vector<modb::CAtom<int>>{3,2,1,2,3});
  print(std::vector<modb::CAtom<char>>{'h','e','l','o',' ','m','y',' ','o','d','b'});
*/

  modb::CAtoms<int>  atomsInt;
  modb::CAtoms<char> atomsChar;
  atomsInt.push_back( std::make_shared<modb::CAtom<int>>(4) );
  atomsInt.push_back( g_oOdb.MakeAtomInt(3) );
  atomsInt.push_back( g_oOdb.MakeAtomInt(2) );
  atomsInt.push_back( g_oOdb.MakeAtomInt(1) );
  atomsInt.push_back( g_oOdb.MakeAtomInt(2) );
  atomsInt.push_back( g_oOdb.MakeAtomInt(3) );

  atomsChar.push_back( std::make_shared<modb::CAtom<char>>('Q') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('Q') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('u') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('i') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('r') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('k') );
  atomsChar.push_back( g_oOdb.MakeAtomChar('s') );

  modb::CThings things;
  things.push_back( std::make_shared<modb::CThing>() );
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

