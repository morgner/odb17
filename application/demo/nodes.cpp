/**
    @file nodes.cpp
 
    @author Manfred Morgner
    @date 15.04.2018

    Demonstration of how Nodes are made and how they can be connected to
    each other using specific unidirectional reasons.
 */

#include <iostream>

#include "odb.h"

auto oOdb = odb::COdb();


// Demo main program
int main()
    {
    // 3 people
    oOdb.MakeNode("Udo");
    oOdb.MakeNode("Ina");
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
    }

