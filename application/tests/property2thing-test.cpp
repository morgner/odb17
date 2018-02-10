/**
 *  @file main.cpp
 *
 *  @author Manfred Morgner
 *  @date 10.02.2018
 */

#include <iostream>

#include "odb.h"
#include "thing.h"
#include "property.h"

/// @brief Demo main program for "property in thing"
int main()
   {
   auto oOdb = odb::COdb();
   auto thing = oOdb.MakeThing( "Tree" );
   auto property = oOdb.MakeProperty( "Acorn" );
   thing->Append(property);
   std::cout << "thing: " << *thing;
   std::cout << '\n';
   }
