/**
    @file main.cpp
 
    @author Manfred Morgner
    @date 21.01.2018
 */

#include <iostream>
#include <memory>
#include <array>

#include <vector>
#include <algorithm>
#include <iterator>

#include "generic.h"
#include "odb.h"
#include "atom.h"
#include "thing.h"

auto oOdb = odb::COdb();

template<typename ...Args> void mkthings    (Args&&... args) { (oOdb.MakeThing   (args), ...); } 
template<typename... Args> void mkproperties(Args&&... args) { (oOdb.MakeProperty(args), ...); }
template<typename... Args> void mkreasons   (Args&&... args) { (oOdb.MakeReason  (args), ...); }
template<typename... Args> void mkatoms     (Args&&... args) { (oOdb.MakeAtom    (args, "fold" ), ...); }

// append a property to a group of things, if property does not exists and
// 'cbForce' is 'true', the property will be added to the DB
template<typename... Args>
void ap2ts(std::string const & crsProperty, // name of the property
		  bool const   cbForce,     // create it if not existent?
		     Args&&... args)        // pack of names of 'things'
    {
    (oOdb.AppendProperty2Thing(crsProperty, cbForce, args), ...);
    }

// Demo main program
int main()
    {
    // filling in some data
    // ==========================================================================================
    mkthings    ("Ulli", "Nora", "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha", "Elise");
    mkproperties("person", "male", "female", "driver", "consumer", "contractor");
    mkreasons   ("wrote", "read", "bought", "left", "foundet", "loves", "sells", "works at");
    mkatoms     ( 2.5, "done", 7, std::array{2,1,3}, "go", 89, "sold", "percent");
    // ==========================================================================================

    // give all 'things' the property 'person'
    for ( size_t n = 0; n < oOdb.Things().size(); ++n )
        {
        oOdb.AppendProperty2Thing( n, 0 );
	}

    // assign 'properties' to groups of 'things' (supported by fold expressions)
    // ========================================================================================================
    ap2ts( "person",     false, "Ulli", "Nora", "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha", "Elise");
    ap2ts( "male",       false, "Ulli",         "Peter",          "Rudi",          "Arnold"                   );
    ap2ts( "female",     false,         "Nora",          "Paula",         "Marta",           "Bertha", "Elise");
    ap2ts( "driver",     false, "Ulli",                  "Paula", "Rudi", "Marta",           "Bertha"         );
    ap2ts( "consumer",   false, "Ulli", "Nora", "Peter",                                               "Elise");
    ap2ts( "contractor", false,                 "Peter", "Paula", "Rudi",                              "Elise");
    ap2ts( "artist",     true,  "Ulli",                                            "Arnold", "Bertha", "Elise");
    ap2ts( "builder",    true,                  "Peter", "Paula",                  "Arnold"                   );
    // ========================================================================================================

    auto px = oOdb.MakeThing("Ulli");
    oOdb.AppendProperty2Thing( px->id, 0 );
    oOdb.AppendProperty2Thing( px->id, 6 );
    oOdb.AppendProperty2Thing( px->id, 7 );

    // INTERSECTION
    // collect all drivers
    auto drivers = oOdb.SelectThingsByProperty( "driver" );
    // collect all females
    auto females = oOdb.SelectThingsByProperty( "female" );
    // intersect 'drivers' with 'females', resulting in an aggregate of 'female drivers'
    std::vector<size_t> vFemaleDrivers;
    std::set_intersection(drivers.begin(), drivers.end(),
                          females.begin(), females.end(),
                          std::back_inserter(vFemaleDrivers));

    // UNION
    // collect all artists
    auto artists = oOdb.SelectThingsByProperty( "artist" );
    // collect all builder
    auto builder = oOdb.SelectThingsByProperty( "builder" );
    // sum 'artists' and 'builder', resulting in an aggregate of 'artist or builder'
    std::set<size_t> vArtistOrBuilder;
    for ( auto const & a:artists ) vArtistOrBuilder.insert(a);
    for ( auto const & a:builder ) vArtistOrBuilder.insert(a);

    // REGEX
    // collect all having properties with names begining with 'con'
    auto vSelection = oOdb.SelectThingsByProperty( "^con.*" );

    std::cout << "---------------- selected 'things' having properties 'driver' and 'female'" << '\n';
    for ( auto const & a:vFemaleDrivers )
        {
        std::cout << *(oOdb.Things()[a]) << "\tid: " << (oOdb.Things()[a])->id << '\n';
        }

    std::cout << "---------------- selected 'things' having properties 'artist' or 'builder'" << '\n';
    for ( auto const & a:vArtistOrBuilder )
        {
        std::cout << *(oOdb.Things()[a]) << '\n';
        }

    std::cout << "---------------- selected 'things' having properties with '^con'" << '\n';
    for ( auto const & a:vSelection )
        {
        std::cout << *(oOdb.Things()[a]) << '\n';
        }

    std::cout << "---------------- all properties" << '\n';
    for ( auto const & a:oOdb.Properties() )
        {
        std::cout << *a << '\n';
        for ( auto b:a->m_oRelations )
            std::cout << "  " << b->m_sName << '\n';
        }

    std::cout << "---------------- all things" << '\n';
    for ( auto const & a:oOdb.Things() )
        {
        std::cout << *a << '\n';
        }
    }
