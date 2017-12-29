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
    std::cout << e.type << " id: " << e.id << " data: " << e << '\n';
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
    std::cout << e->type << " id: " << e->id << '\n';
    }
  }

odb::COdb g_oOdb{};

using namespace std::string_literals;

/**
 * Demo main program
 */
int main()
  {
/*
  odb::CThings things;
  things.emplace_back( std::make_shared<odb::CThing>() );
  things.emplace_back( std::make_shared<odb::CThing>() );

  auto a = odb::CAtom<int>(3);
  print(std::vector<odb::CThing>{ 3 });
  print(std::vector<odb::CAtom<int>>{3,2,1,2,3});
  print(std::vector<odb::CAtom<char>>{'h','e','l','o',' ','m','y',' ','o','d','b'});
*/

  std::string const   cs("constness"s);
  std::string         ns("non-constness"s);
  int                 a[10] = {1, 2, 3, 4, 5, 6};
  std::vector<int>    v{1, 2, 3};
  std::array<char, 3> w{'a','b','c'};
  
  odb::CAtoms  atoms;
/*
  atoms.emplace_back("hallo", "world");
  atoms.emplace_back(1, "mass", "", "kg");
  atoms.emplace_back(std::string(8, '-'), "line");
*/
  atoms.emplace_back( g_oOdb.MakeAtom(100.2, "round", "", "%") );
  atoms.emplace_back( g_oOdb.MakeAtom(0.9, "size", "V", "dl") );
  atoms.emplace_back( g_oOdb.MakeAtom(1.2) );
  atoms.emplace_back( g_oOdb.MakeAtom(std::vector<int>   {3, 2, 1}, "vector") );
  atoms.emplace_back( g_oOdb.MakeAtom(std::array <int, 3>{3, 2, 1}, "array") );
  atoms.emplace_back( g_oOdb.MakeAtom(cs, "const string"s) );
  atoms.emplace_back( g_oOdb.MakeAtom(ns, "non-const string") );
//  atoms.emplace_back( g_oOdb.MakeAtom(a) );
  atoms.emplace_back( g_oOdb.MakeAtom(v, "vector of 3 int") );
  atoms.emplace_back( g_oOdb.MakeAtom(w, "array of 3 char") );

//  atoms.emplace_back( std::make_shared<odb::CAtom>(4) );
  atoms.emplace_back( g_oOdb.MakeAtom(3, "Three") );
  atoms.emplace_back( g_oOdb.MakeAtom(2, "Two") );
  atoms.emplace_back( g_oOdb.MakeAtom(1, "One") );

  atoms.emplace_back( g_oOdb.MakeAtom(std::string(8, '-'), "Line"s) );

//  atoms.emplace_back( std::make_shared<odb::CAtom>('Q') );
  atoms.emplace_back( g_oOdb.MakeAtom('H', "Letter"s) );
  atoms.emplace_back( g_oOdb.MakeAtom('e', "Letter"s) );
  atoms.emplace_back( g_oOdb.MakeAtom('l', "Letter"s) );
  atoms.emplace_back( g_oOdb.MakeAtom('o', "Letter"s) );
  atoms.emplace_back( g_oOdb.MakeAtom("Quirks & Quarks", "Radio Cast"s) );
  atoms.emplace_back( g_oOdb.MakeAtom("Quirks & Quarks"s, "Podcast"s) );

  atoms.emplace_back( g_oOdb.MakeAtom("---------"s, "Line"s) );

  odb::CThings things;
//  things.emplace_back( std::make_shared<odb::CThing>() );
  things.emplace_back( g_oOdb.MakeThing("Wundertüte"s) );
  things.emplace_back( g_oOdb.MakeThing() );
  things.emplace_back( g_oOdb.MakeThing("No Poduct"s) );
  things.emplace_back( g_oOdb.MakeThing("Harmlos Tüte"s) );
  
  print(atoms);
  print(things);

//  g_oOdb.Dump();
  }

