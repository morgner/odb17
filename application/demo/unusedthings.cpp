/**
    @file unusedthings.cpp
 
    @author Manfred Morgner
    @date 15.04.2018

    Demonstrates, that its possible to have orphan elements in the database
    and how to find them.
 */

#include <iostream>

#include "generic.h"
#include "odb.h"
#include "atom.h"
#include "thing.h"

auto oOdb = odb::COdb();


// Demo main program
int main()
    {
    // 3 people
    oOdb.MakeThing("Udo");
    oOdb.MakeThing("Ina");
    oOdb.MakeThing("Book");
    oOdb.MakeThing("Rob");

    // 3 kind of relation
    oOdb.MakeReason("is father of");
    oOdb.MakeReason("knows");
    oOdb.MakeReason("loves");
    
    // 3 bindings
    oOdb.LinkThing2Thing("Udo", "is father of", "Ina");
    oOdb.LinkThing2Thing("Udo", "knows"       , "Rob");
    oOdb.LinkThing2Thing("Ina", "loves"       , "Rob");

    // show us
    std::cout << "---------------- all things" << '\n';
    for ( auto const & a:oOdb.Things() )
        {
        std::cout << *a << '\n';
        }
    
    std::cout << "---------------- unused things" << '\n';
    for ( auto const & a:oOdb.FindUnUsedThings() )
        {
        std::cout << *a << '\n';
        }
    }

