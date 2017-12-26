/*  main.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <iostream> // cout

#include "thing.h"
#include "atom.h"

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
  atomsInt.push_back( std::make_shared<modb::CAtom<int>>(3) );
  atomsInt.push_back( std::make_shared<modb::CAtom<int>>(2) );
  atomsInt.push_back( std::make_shared<modb::CAtom<int>>(1) );
  atomsInt.push_back( std::make_shared<modb::CAtom<int>>(2) );
  atomsInt.push_back( std::make_shared<modb::CAtom<int>>(3) );

  atomsChar.push_back( std::make_shared<modb::CAtom<char>>('Q') );
  atomsChar.push_back( std::make_shared<modb::CAtom<char>>('u') );
  atomsChar.push_back( std::make_shared<modb::CAtom<char>>('i') );
  atomsChar.push_back( std::make_shared<modb::CAtom<char>>('r') );

  modb::CThings things;
  things.push_back( std::make_shared<modb::CThing>() );
  things.push_back( std::make_shared<modb::CThing>() );
  
  print(atomsInt);
  print(atomsChar);
  print(things);
  }

