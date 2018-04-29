/**
    @file unusednodes.cpp
 
    @author Manfred Morgner
    @date 15.04.2018

    Demonstrates, that its possible to have orphan elements in the database
    and how to find them.
 */

#include <iostream>

#include "generic.h"
#include "odb.h"
#include "atom.h"
#include "node.h"

auto oOdb = odb::COdb();


// Demo main program
int main()
    {
    // 3 people
    oOdb.MakeNode("Udo");
    oOdb.MakeNode("Ina");
    oOdb.MakeNode("Book");
    oOdb.MakeNode("Rob");

    // 3 kind of relation
    oOdb.MakeReason("is father of");
    oOdb.MakeReason("knows");
    oOdb.MakeReason("loves");
    
    // 3 bindings
    oOdb.LinkNode2Node("Udo", "is father of", "Ina");
    oOdb.LinkNode2Node("Udo", "knows"       , "Rob");
    oOdb.LinkNode2Node("Ina", "loves"       , "Rob");

    // show us
    std::cout << "---------------- all nodes" << '\n';
    for ( auto const & a:oOdb.Nodes() )
        {
        std::cout << *a << '\n';
        }
    
    std::cout << "---------------- unused nodes" << '\n';
    for ( auto const & a:oOdb.FindUnUsedNodes() )
        {
        std::cout << *a << '\n';
        }
    }

