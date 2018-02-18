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

#include <fstream>
#include <stdlib.h> // atol
#include <cstring>
#include <regex>

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

/*
../raw-data/title.basics.tsv 
tconst		titleType	primaryTitle		originalTitle		isAdult	startYear	endYear	runtimeMinutes	genres
tt0000001	short		Carmencita		Carmencita		0	1894		\N	1		Documentary,Short
tt0000002	short		Le clown et ses chiens	Le clown et ses chiens	0	1892		\N	5		Animation,Short
0=id		1=/		2=/			3=name			4=/	5=p		6=/	7=(a)		8=p
*/

// Demo main program
int main()
    {
/*
    string s = "readline";
    regex e("\\t");
    regex_token_iterator<string::iterator> it(s.begin(), s.end(), e, -1);
    regex_token_iterator<string::iterator> end;
    while (it != end)
        cout << " [" << *it++ << "]";	
*/
    int e = 0;
    static std::string sLine;
    std::fstream imdb_tb("../raw-data/title.basics.tsv", std::ifstream::in);

    size_t nId = 0;
    size_t   i = 0;

    std::getline(imdb_tb, sLine );

    while ( std::getline(imdb_tb, sLine ))
        {
        std::string sName;
        std::string sType;
        std::string sYearF;
        std::string sAtom;
        std::string sGenres;

	char * ptr = std::strtok(&*sLine.begin(), "\t");
        std::string sItem;
	e = 0;
	odb::PThing m;
//      0=id	1=/	2=/	3=name	4=/	5=p	6=/	7=(a)	8=p+
	while ( ptr != nullptr )
	    {

	    sItem = ptr;
            switch ( e )
	        {
	        case 0: nId = std::stoull( sItem.substr(2) ); break;
	        case 1: sType   = sItem; break;
	        case 3: sName   = sItem; break;
	        case 5: sYearF  = sItem; break;
	        case 7: sAtom   = sItem; break; // runtime
                case 8: sGenres = sItem; // gengre1...n = split(sItem, ",")
		        while ( i < nId )
		            {
			    ++i;
			    oOdb.MakeThing("filler"); // std::cout << 'f';
		       	    }
                        m = oOdb.MakeThing(sName);
//		        std::cout << nId << " - " << m->id << '\n';
		        ++i;
		        m->Append( oOdb.FindOrAddProperty( sType ) );
		        m->Append( oOdb.FindOrAddProperty( sYearF ) );
		        break;
	        } //switch()
	    ptr = strtok(nullptr, "\t");
	    ++e;
	    }
	ptr = std::strtok(&*sGenres.begin(), ",");
	std::string sGenre;
	while ( ptr != nullptr )
	    {
	    sGenre = ptr;
	    m->Append( oOdb.FindOrAddProperty( sGenre ) );
	    ptr = strtok(nullptr, ",");
	    }
        }

    imdb_tb.close();

/*
 * name.basics.tsv
nconst		primaryName	birthYear	deathYear	primaryProfession		knownForTitles
nm0000001	Fred Astaire	1899		1987		soundtrack,actor,miscellaneous	tt0050419,tt0072308,tt0043044,tt0053137
nm0000002	Lauren Bacall	1924		2014		actress,soundtrack		tt0117057,tt0037382,tt0040506,tt0038355
0=id		1=name		2=Ybirth	3=Ydeath	4=profession			4=movies	
*/

    std::cout << "START" << '\n';
    std::fstream imdb_nb("../raw-data/name.basics.tsv", std::ifstream::in);
    std::getline(imdb_nb, sLine);
    std::cout << sLine << '\n';
    std::regex r("\\t");
    std::regex k(",");
    std::regex_token_iterator<std::string::iterator> end;
    auto ReasonAI = oOdb.MakeReason( "acts-in" );

    while ( std::getline(imdb_nb, sLine))
        {
        std::string sName;
        std::string sYearB;
        std::string sYearD;
        std::string sProfessions;
        std::string sMovies;

        std::string sItem;
	e = 0;
	odb::PThing m;
	std::regex_token_iterator<std::string::iterator> it(sLine.begin(), sLine.end(), r, -1);
//	0=id	1=name	2=Ybirth	3=Ydeath	4=profession	5=movies	
        while (it != end)
	    {
	    sItem = *it++;
            switch ( e )
	        {
	        case 0: nId          = std::stoull( sItem.substr(2) ); break;
	        case 1: sName        = sItem; break;
	        case 2: sYearB       = sItem; break;
	        case 3: sYearD       = sItem; break;
	        case 4: sProfessions = sItem; break; // runtime
                case 5: sMovies      = sItem; // gangre1...n = split(sItem, ",")
			if ( sName == "" ) sName = "empty";
                        m = oOdb.MakeThing(sName);
			std::cout << "Thing(" << m->id << ") name: " << sName << "\n";

	                std::regex_token_iterator<std::string::iterator> itP(sProfessions.begin(), sProfessions.end(), k, -1);
                        while (itP != end)
			    {
//			    std::cout << "m(" << m->id << ")->Append( oOdb.FindOrAddProperty( " << *itP << " ) );\n";
			    if ( *itP == "" ) break;
			    m->Append( oOdb.FindOrAddProperty( *itP++ ) );
			    }

	                std::regex_token_iterator<std::string::iterator> itM(sMovies.begin(), sMovies.end(), k, -1);
                        while (itM != end)
			    {
			    if ( *itM == "" ) break;
			    sItem = *itM++;
			    try
			    {
	                    size_t nToId = std::stoull( sItem.substr(2) );
			    oOdb.LinkThing2Thing( m->id, nToId, ReasonAI->id );
//			    std::cout << "oOdb.LinkThing2Thing( " << m->id << ", " << nToId << ", " << ReasonAI->id << " );\n";
			    }
			    catch(...) { std::cerr << "ERROR: not a XYnumber - " << sItem << '\n'; }
			    }
//			std::cout << *(oOdb.Things()[m->id]) << '\n';

		        break;
	        } //switch()
	    ++e;
	    }
        }
    imdb_nb.close();



//    mkthing (name, id);
//    aps2t(prop1...propn, name);

/*	
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
    oOdb.AppendProperty2Thing( 0, px->id );
    oOdb.AppendProperty2Thing( 6, px->id );
    oOdb.AppendProperty2Thing( 7, px->id );

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
*/
/*
    std::cout << "---------------- all properties" << '\n';
    for ( auto const & a:oOdb.Properties() )
        {
        std::cout << *a << '\n';
        for ( auto b:a->m_oRelations )
            std::cout << "  " << b->m_sName << '\n';
        }
*/
    std::cout << "---------------- all things" << '\n';
    for ( auto const & a:oOdb.Things() )
        {
        std::cout << *a << '\n';
        }
    }
