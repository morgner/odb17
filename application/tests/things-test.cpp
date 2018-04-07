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

template<typename... Args>
void lt2t(std::string const & crsNameTo, // name of the property
          std::string const & crsReason, // create it if not existent?
                    Args&&... args)      // pack of names of 'things'
    {
    (oOdb.LinkThing2Thing(args, crsNameTo, crsReason), ...);
    }

// Demo main program
int main()
    {
    // filling in some data
    // ==========================================================================================
    mkthings    ("Ulli", "Nora", "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha", "Elise", "Jack");
    mkthings    ("Emerald woods", "Madix", "Skoda", "Trombone", "Lecho", "SilentOS", "Insurance");
    mkproperties("person", "male", "female", "driver", "consumer", "contractor");
    mkreasons   ("wrote", "read", "bought", "left", "foundet", "loves", "sells", "works at", "uses", "plays");
    mkatoms     ( 2.5, "done", 7, std::array{2,1,3}, "go", 89, "sold", "percent");
    // ==========================================================================================

    // ================================================================================================
    lt2t("Emerald woods", "wrote",    "Ulli"                                                         );
    lt2t("Emerald woods", "read",     "Nora",   "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha");
    lt2t("Trombone",      "bought",   "Peter"                                                        );
    lt2t("Rudi",          "left",     "Paula",  "Elise", "Marta"                                     );
    lt2t("Lecho",         "foundet",  "Rudi"                                                         );
    lt2t("Peter",         "loves",    "Marta",  "Jack"                                               );
    lt2t("Insurance",     "sells",    "Arnold"                                                       );
    lt2t("Skoda",         "works at", "Bertha", "Ulli", "Nora", "Arnold"                             );
    lt2t("SilentOS",      "uses",     "Elise"                                                        );
    lt2t("Trombone",      "plays",    "Jack",  "Peter"                                               );
    // ================================================================================================

    // give all 'things' the property 'person'
    for ( size_t n = 0; n < oOdb.Things().size(); ++n )
        {
        oOdb.AppendProperty2Thing( 0, n );
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
    oOdb.AppendProperty2Thing( 0, px->m_nId );
    oOdb.AppendProperty2Thing( 6, px->m_nId );
    oOdb.AppendProperty2Thing( 7, px->m_nId );

    // INTERSECTION
    // collect all drivers
    auto drivers = oOdb.FindThingsByProperty( "driver" );
    // collect all females
    auto females = oOdb.FindThingsByProperty( "female" );
    // intersect 'drivers' with 'females', resulting in an aggregate of 'female drivers'
    std::vector<odb::PThing> vFemaleDrivers;
    std::set_intersection(drivers.begin(), drivers.end(),
                          females.begin(), females.end(),
                          std::back_inserter(vFemaleDrivers));

    // UNION
    // collect all artists
    auto artists = oOdb.FindThingsByProperty( "artist" );
    // collect all builder
    auto builder = oOdb.FindThingsByProperty( "builder" );
    // sum 'artists' and 'builder', resulting in an aggregate of 'artist or builder'
    odb::CThings vArtistOrBuilder;
    for ( auto const & a:artists ) vArtistOrBuilder.insert(a);
    for ( auto const & a:builder ) vArtistOrBuilder.insert(a);

//    // REGEX
//    // collect all having properties with names begining with 'con'
//    auto vSelection = oOdb.FindThingsByProperty( "^con.*" );

    std::cout << "---------------- selected 'things' having properties 'driver' and 'female'" << '\n';
    for ( auto const & a:vFemaleDrivers )
        {
        std::cout << *a << "\tid: " << a->m_nId << '\n';
        }

    std::cout << "---------------- selected 'things' having properties 'artist' or 'builder'" << '\n';
    for ( auto const & a:vArtistOrBuilder )
        {
        std::cout << *a << "\tid: " << a->m_nId << '\n';
        }

//    std::cout << "---------------- selected 'things' having properties with '^con'" << '\n';
//    for ( auto const & a:vSelection )
//        {
//        std::cout << a << '\n';
//        }

    std::cout << "---------------- all properties" << '\n';
    for ( auto const & a:oOdb.Properties() )
        {
        std::cout << *a << '\n';
        for ( auto const & b:a->Relations() )
            std::cout << "  " << b->m_sName << '\n';
        }

    std::cout << "---------------- all things" << '\n';
    for ( auto const & a:oOdb.Things() )
        {
        std::cout << *a << '\n';
        }
    }
