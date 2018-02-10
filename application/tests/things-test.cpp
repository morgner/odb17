/**
    @file main.cpp
 
    @author Manfred Morgner
    @date 21.01.2018
 */

#include <iostream>
#include <memory>
#include <array>

#include "generic.h"
#include "odb.h"
#include "atom.h"
#include "thing.h"

auto oOdb = odb::COdb();

template<typename ...Args> void mkthings    (Args&&... args) { (oOdb.MakeThing   (args), ...); } 
template<typename... Args> void mkproperties(Args&&... args) { (oOdb.MakeProperty(args), ...); }
template<typename... Args> void mkreasons   (Args&&... args) { (oOdb.MakeReason  (args), ...); }
template<typename... Args> void mkatoms     (Args&&... args) { (oOdb.MakeAtom    (args, "fold" ), ...); }

template<typename... Args> void ap2ts(std::string const & sProperty, Args&&... args) { (oOdb.AppendProperty2Thing(sProperty, args), ...); }

/// @brief Demo main program
int main()
    {
    mkthings    ("Ulli", "Nora", "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha", "Elise");
    mkproperties("person", "male", "female", "driver", "consumer", "contractor");
    mkreasons   ("wrote", "read", "bought", "left", "foundet", "loves", "sells", "works at");
    mkatoms     ( 2.5, "done", 7, std::array{2,1,3}, "go", 89, "sold", "percent");

    for ( size_t n = 0; n < oOdb.Things().size(); ++n )
        {
        oOdb.AppendProperty2Thing( n, 0 );
	}
    ap2ts( "male",   "Ulli", "Peter", "Rudi",  "Arnold" );
    ap2ts( "female", "Nora", "Paula", "Marta", "Bertha", "Elise");

    oOdb.print();
    oOdb.print_json(std::cout);
    }
