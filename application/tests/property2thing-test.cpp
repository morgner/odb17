/**
 *  @file main.cpp
 *
 *  @author Manfred Morgner
 *  @date 10.02.2018
 */

#include <iostream>

#include "odb.h"


/// @brief Demo main program for "property in node"
int main()
   {
   auto oOdb = odb::COdb();
   auto node = oOdb.MakeNode( "Tree" );
   auto property = oOdb.MakeProperty( "Acorn" );
   node->Append(property);
   std::cout << "node: " << *node;
   std::cout << '\n';
   }
