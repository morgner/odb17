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
template<typename T>
//17 auto print(std::vector<T> const & vec)
void print(std::vector<T> const & vec)
  {
  for (auto && e : vec)
    {
    std::cout << e.type << " id: " << e.id << '\n';
    }
  }

/**
 * Demo main program
 */
int main()
  {
  modb::CThings things;
  things.push_back( std::make_shared<modb::CThing>() );
  things.push_back( std::make_shared<modb::CThing>() );

  print(std::vector<modb::CThing>{ 3 });
  print(std::vector<modb::CAtom> { 5 });
  }

