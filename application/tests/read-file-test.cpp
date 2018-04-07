/**
    @file read-file-test.cpp
 
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
//#include <cstring>
#include <string>
#include <regex>

#include "generic.h"
#include "odb.h"
#include "atom.h"
#include "thing.h"

using namespace std::string_literals;

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
0=id		1=/		2=/			3=name			4=/	5=p		6=/	7=(a)		8=p+
*/

// Demo main program
int main( int argc, const char* argv[] )
    {
    size_t nReadLimit{1};
    try
        {
	size_t n = std::stoull( argv[1] );
	nReadLimit = n;
	}
    catch(...)
       	{
	nReadLimit = 1000;
	}
      std::cout << nReadLimit << " lines per file\n";
//    std::cout << "Read how many lines per file? : ";
//    std::cin >> nReadLimit;


    int e = 0;
    static std::string sLine;
    std::fstream imdb_tb("../raw-data/title.basics.tsv", std::ifstream::in);

    size_t nId = 0;
    size_t   i = 0;

    std::getline(imdb_tb, sLine );

    while ( std::getline(imdb_tb, sLine ) && (nId++ < nReadLimit /*DEBUG*/) )
        {
//      if ( nId % 1000 == 0 ) { std::cout << nId << '\n'; }
            
        std::string sId;
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
                case 0: sId     = sItem; break; // nId = std::stoull( sItem.substr(2) ); break;
                case 1: sType   = sItem; break;
                case 3: sName   = sItem; break;
                case 5: sYearF  = sItem; break;
                case 7: sAtom   = sItem; break; // runtime
                case 8: sGenres = sItem; // genres1...n = split(sItem, ",")
//                      std::cout << sId << '\n';
                        if ( ""s    == sName ) sName = "Empty title";
                        if ( "\\N"s == sName ) sName = "No title";
                        m = oOdb.FindOrMakeThingByProperty( sName, sId );
//                      std::cout << nId << " - " << m->m_nId << '\n';
/* class */             m->Append( oOdb.FindOrMakeProperty( "class:movie"s ) );
                        if ( (""s != sType ) && ("\\N"s != sType ) ) m->Append( oOdb.FindOrMakeProperty( sType ) );
                        if ( (""s != sYearF) && ("\\N"s != sYearF) ) m->Append( oOdb.FindOrMakeProperty( sYearF ) );
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
            if ( (""s != sGenre) && ("\\N"s != sGenre) ) m->Append( oOdb.FindOrMakeProperty( sGenre ) );
            ptr = strtok(nullptr, ",");
            }
//      std::cout << *m << '\n';
        }
    imdb_tb.close();

/*
 * name.basics.tsv
nconst		primaryName	birthYear	deathYear	primaryProfession		knownForTitles
nm0000001	Fred Astaire	1899		1987		soundtrack,actor,miscellaneous	tt0050419,tt0072308,tt0043044,tt0053137
nm0000002	Lauren Bacall	1924		2014		actress,soundtrack		tt0117057,tt0037382,tt0040506,tt0038355
0=id		1=name		2=Ybirth	3=Ydeath	4=profession+			4=movies+
*/

    std::fstream imdb_nb("../raw-data/name.basics.tsv", std::ifstream::in);
    std::getline(imdb_nb, sLine);
    std::regex r("\\t");
    std::regex k(",");
    std::regex_token_iterator<std::string::iterator> end;
    auto ReasonAI = oOdb.MakeReason( "acts-in" );
    auto brk{false};
    nId = 0;
    while ( std::getline(imdb_nb, sLine) && (nId++ < nReadLimit /*DEBUG*/) )
        {
//      if ( nId % 1000 == 0) { std::cout << nId << '\n'; }

        std::string sId;
        std::string sName;
        std::string sYearB;
        std::string sYearD;
        std::string sProfessions;
        std::string sMovies;

        std::string sItem;
        e = 0;
        odb::PThing m; m = nullptr;
	odb::PThing movie; movie = nullptr;
        std::regex_token_iterator<std::string::iterator> it(sLine.begin(), sLine.end(), r, -1);
//	0=id	1=name	2=Ybirth	3=Ydeath	4=profession	5=movies	
        while ( (it != end) && (!brk) )
            {
            sItem = *it++;
            switch ( e++ )
                {
                case 0: sId          = sItem; break;
                case 1: sName        = sItem;
//                      std::cout << sId << '\n';
//                      m = oOdb.FindOrMakeThingByProperty( sName, sId );
//                      std::cout << "Thing(" << m->m_nId << ") name: " << sName << "\n";
                        break;

                case 2: sYearB       = sItem; break;
                case 3: sYearD       = sItem; break;
                case 4: sProfessions = sItem; break;
                case 5: sMovies      = sItem;
                        std::regex_token_iterator<std::string::iterator> itM(sMovies.begin(), sMovies.end(), k, -1);
                        while (itM != end)
                            {
                            sItem = *itM++;
//                          try { if ( std::stoull( sItem.substr(2) ) > nReadLimit /*DEBUG*/ ) continue; } catch(...) { continue; }
                            if ( ""s    == sItem ) continue;
                            if ( "\\N"s == sItem ) continue;

                            movie = oOdb.FindThingByProperty( sItem ).value_or(nullptr);
			    if ( movie == nullptr ) continue;
                            if ( m     == nullptr ) m = oOdb.FindOrMakeThingByProperty( sName, sId );
//                          odb::PThing movie = oOdb.FindOrMakeThingByProperty( "Linked Movie w/o title"s, sItem );
                            m->Link( movie, ReasonAI );
/* class */                 m->Append( oOdb.FindOrMakeProperty( "class:person" ) );
//                          std::cout << sItem << ": oOdb.LinkThing2Thing( " << m->m_sName << ", " << movie->m_sName << ", " << ReasonAI->m_nId << " );\n";
                            }

			if ( movie != nullptr )
			    {
                            std::regex_token_iterator<std::string::iterator> itP(sProfessions.begin(), sProfessions.end(), k, -1);
                            while (itP != end)
                                {
//                              std::cout << "m(" << m->m_nId << ")->Append( oOdb.FindOrMakeProperty( " << *itP << " ) );\n";
                                sItem = *itP++;
                                if ( ""s    == sItem ) continue;
                                if ( "\\N"s == sItem ) continue;
                                m->Append( oOdb.FindOrMakeProperty( sItem ) );
                                }
			    }
                        break;
                } // switch(e)
            } // while ( it != end )
        } // while ( std::getline(imdb_nb, sLine) ...)
    imdb_nb.close();
/*
 * title.principals.tsv
tconst	    ordering	nconst	        category	job	characters
tt0000001	1	nm1588970	self	\N	["Herself"]
tt0000001	2	nm0005690	director	\N	\N
tt0000001	3	nm0374658	cinematographer	director of photography	\N
tt0000002	1	nm0721526	director	\N	\N
tt0000002	2	nm1335271	composer	\N	\N
tt0000003	1	nm0721526	director	\N	\N
tt0000003	2	nm5442194	producer	producer	\N
*/

    std::fstream imdb_pp("../raw-data/title.principals.tsv", std::ifstream::in);
    std::getline(imdb_pp, sLine);
    nId = 0;
    while ( std::getline(imdb_pp, sLine) )
//  while ( std::getline(imdb_pp, sLine) && (nId++ < nReadLimit /*DEBUG*/) )
        {
//      if ( nId % 1000 == 0) { std::cout << nId << '\n'; }

        std::string sTId;
        std::string sOrder;
        std::string sNId;
        std::string sCategory;
        std::string sJob;
        std::string sCharacters;

        std::string sItem;
        e = 0;
	brk = false;
        odb::PThing tt;
        odb::PThing nm;
        std::regex_token_iterator<std::string::iterator> it(sLine.begin(), sLine.end(), r, -1);
//	0=tid		1=ordering	2=nid		3=category	4=job	5=characters
//	tt0000001	1		nm1588970	self		\N	plays ["Smithson","Man at Broker's"]
        while ( (it != end) && !brk )
            {
            sItem = *it++;
            switch ( e++ )
                {
                case 0: sTId         = sItem; 
//                      std::cout << sId << '\n';
                        tt = oOdb.FindThingByProperty( sTId ).value_or(nullptr);
			if ( tt == nullptr ) { brk = true; continue; }
//                      std::cout << "Thing(" << m->m_nId << ") name: " << sName << "\n";
			break;
                case 1: sOrder       = sItem; break;
                case 2: sNId         = sItem; 
//                      std::cout << sId << '\n';
                        nm = oOdb.FindThingByProperty( sNId ).value_or(nullptr);
			if ( nm == nullptr ) { brk = true; continue; }
/* class */             nm->Append( oOdb.FindOrMakeProperty( "class:person" ) );
//                      std::cout << "Thing(" << m->m_nId << ") name: " << sName << "\n";
			break;
                case 3: sCategory    = sItem;
                        if ( (""s != sItem) && ("\\N"s != sItem) )
			    {
//                          std::cout << "CAT: oOdb.LinkThing2Thing( " << nm->m_sName << ", " << tt->m_sName << ", " << sItem << " );\n";
                            nm->Link( tt, oOdb.FindOrMakeReason( sItem ) );
//                          nm->Append( oOdb.FindOrMakeProperty( sItem ) );
                            }
			break;
                case 4: sJob         = sItem;
                        if ( (""s != sItem) && ("\\N"s != sItem) )
			    {
//                          std::cout << "JOB: oOdb.LinkThing2Thing( " << nm->m_sName << ", " << tt->m_sName << ", " << sItem << " );\n";
                            nm->Link( tt, oOdb.FindOrMakeReason( sItem ) );
//                          nm->Append( oOdb.FindOrMakeProperty( sItem ) );
                            }
			break;
                case 5: sCharacters  = sItem;
                        if ( (""s != sItem) && ("\\N"s != sItem) )
			    {
//                          echo "plays [\"Smithson\",\"Man at Broker's\"]" | sed -e 's/[^\[]*\[.\(.*\).\]/\1/' -e 's/","/\t/g'
//                          => Smithson
//                          => Man at Broker's
//
//                          std::cout << sCharacters << ":";
                            std::string sStep1 = std::regex_replace( sCharacters,
                                                                     std::regex("[\\[]*\\[.(.*).\\]"),
                                                                     "$1");
//                          std::cout << sStep1 << ":";
                                   sCharacters = std::regex_replace( sStep1,
                                                                     std::regex("\",\""),
                                                                     "\t");
//                          std::cout << sCharacters << ":\n";
                            std::regex_token_iterator<std::string::iterator> itC(sCharacters.begin(), sCharacters.end(), r, -1);
                            while (itC != end)
                                {
                                sItem = *itC++;
//                              std::cout << "  " << sItem << "\n";

//                              std::cout << "CHR: oOdb.LinkThing2Thing( " << nm->m_sName << ", " << tt->m_sName << ", " << sItem << " );\n";
                                nm->Link( tt, oOdb.FindOrMakeReason( "role:" + sItem ) );
//                              nm->Append( oOdb.FindOrMakeProperty( sItem ) );
                                }
                            }
                        break;
                } // switch(e)
            } // while ( it != end )
        } // while ( std::getline(imdb_pp, sLine) ...)
    imdb_pp.close();

/*
 * title.crew.tsv
tconst          directors                       writers
tt0000245       \N                              \N
tt0000246       nm0617588                       \N
tt0000247       nm2156608,nm0005690,nm0002504   nm0000636,nm0002504
0=id            1=directors                     2=writers
*/

    std::fstream imdb_cr("../raw-data/title.crew.tsv", std::ifstream::in);
    std::getline(imdb_cr, sLine);
    nId = 0;
    while ( std::getline(imdb_cr, sLine)  )
//  while ( std::getline(imdb_ak, sLine) && (nId++ < nReadLimit /*DEBUG*/) )
        {
//      if ( nId % 1000 == 0) { std::cout << nId << '\n'; }

        std::string sTId;
        std::string sDirectors;
        std::string sWriters;

        std::string sItem;
        e = 0;
	brk = false;
        odb::PThing tt;
        odb::PThing nm;
        std::regex_token_iterator<std::string::iterator> it(sLine.begin(), sLine.end(), r, -1);
//      tconst     directors                       writers
//      tt0000247  nm2156608,nm0005690,nm0002504   nm0000636,nm0002504
//      0=id       1=directors                     2=writers

        while ( (it != end) && !brk )
            {
            sItem = *it++;
            switch ( e++ )
                {
                case 0: sTId         = sItem; 
//                      std::cout << sId << '\n';
                        tt = oOdb.FindThingByProperty( sTId ).value_or(nullptr);
			if ( tt == nullptr ) { brk = true; continue; }
			break;
                case 1: if ( "\\N"s == sItem ) continue;
                        sDirectors = sItem;
                        {
                        std::regex_token_iterator<std::string::iterator> itD(sDirectors.begin(), sDirectors.end(), k, -1);
                        while (itD != end)
                            {
                            sItem = *itD++;
//                          std::cout << "  " << sItem << "\n";
                            nm = oOdb.FindThingByProperty( sItem ).value_or(nullptr);
                            if ( nullptr != nm )
                                {
//                              std::cout << "DIR: oOdb.LinkThing2Thing( " << nm->m_sName << ", " << tt->m_sName << ", " << sItem << " );\n";
                                nm->Link( tt, oOdb.FindOrMakeReason( "director" ) );
                                }
                            }
                        }
                        break;
                case 2: if ( "\\N"s == sItem ) continue;
                        sWriters = sItem;
                        {
                        std::regex_token_iterator<std::string::iterator> itW(sWriters.begin(), sWriters.end(), k, -1);
                        while (itW != end)
                            {
                            sItem = *itW++;
//                          std::cout << "  " << sItem << "\n";
                            nm = oOdb.FindThingByProperty( sItem ).value_or(nullptr);
                            if ( nullptr != nm )
                                {
//                              std::cout << "WRT: oOdb.LinkThing2Thing( " << nm->m_sName << ", " << tt->m_sName << ", " << sItem << " );\n";
                                nm->Link( tt, oOdb.FindOrMakeReason( "writer" ) );
                                }
                            }
                        }
                        break;
                } // switch(e)
            } // while ( it != end )
        } // while ( std::getline(imdb_pp, sLine) ...)
    imdb_cr.close();


/*
 * title.akas.tsv
titleId ordering        title                           region  language        types           attributes      isOriginalTitle
tt0000001       1       Carmencita - spanyol tánc       HU      \N              imdbDisplay     \N                      0
tt0000001       2       Карменсита                      RU      \N              \N              \N                      0
tt0000001       3       Carmencita                      US      \N              \N              \N                      0
tt0000001       4       Carmencita                      \N      \N              original        \N                      1
tt0000002       1       Le clown et ses chiens          \N      \N              original        \N                      1
tt0000002       2       A bohóc és kutyái               HU      \N              imdbDisplay     \N                      0
tt0000002       3       Le clown et ses chiens          FR      \N              \N              \N                      0
tt0000002       4       The Clown and His Dogs          US      \N              \N              literal English title   0
tt0000002       5       Клоун и его собаки              RU      \N              \N              \N                      0
tt0000003       1       Szegény Pierrot                 HU      \N              imdbDisplay     \N                      0
0=id            1=Ord   2=title                         3=reg   4=L             5=type          6=attr                  7=isOrgTitle
*/

    std::fstream imdb_ak("../raw-data/title.akas.tsv", std::ifstream::in);
    std::getline(imdb_ak, sLine);
    nId = 0;
    while ( std::getline(imdb_ak, sLine)  )
//  while ( std::getline(imdb_ak, sLine) && (nId++ < nReadLimit /*DEBUG*/) )
        {
//      if ( nId % 1000 == 0) { std::cout << nId << '\n'; }

        std::string sTId;
        std::string sProperty;

        std::string sItem;
        e = 0;
	brk = false;
        odb::PThing tt;
        odb::PThing nm;
        std::regex_token_iterator<std::string::iterator> it(sLine.begin(), sLine.end(), r, -1);
//      tt0000001  4      Carmencita  \N     \N   original  \N      1
//	0=id       1=Ord  2=title     3=reg  4=L  5=type    6=attr  7=isOrgTitle

//      4:Carmencita:::original::1
        while ( (it != end) && !brk )
            {
            sItem = *it++;
            switch ( e++ )
                {
                case 0: sTId         = sItem; 
//                      std::cout << sId << '\n';
                        tt = oOdb.FindThingByProperty( sTId ).value_or(nullptr);
			if ( tt == nullptr ) { brk = true; continue; }
			break;
                case 1: if ( "\\N"s == sItem ) sItem = ""s; sProperty += sItem + ":"s; break;
                case 2: if ( "\\N"s == sItem ) sItem = ""s; sProperty += sItem + ":"s; break; 
                case 3: if ( "\\N"s == sItem ) sItem = ""s; sProperty += sItem + ":"s; break;
                case 4: if ( "\\N"s == sItem ) sItem = ""s; sProperty += sItem + ":"s; break;
                case 5: if ( "\\N"s == sItem ) sItem = ""s; sProperty += sItem + ":"s; break;
                case 6: if ( "\\N"s == sItem ) sItem = ""s; sProperty += sItem + ":"s; break;
                case 7: if ( "\\N"s == sItem ) sItem = ""s; sProperty += sItem + ":"s;
//                      std::cout << "CHR: oOdb.LinkThing2Thing( " << nm->m_sName << ", " << tt->m_sName << ", " << sItem << " );\n";
//                      nm->Link( tt, oOdb.FindOrMakeReason( sItem ) );
                        tt->Append( oOdb.FindOrMakeProperty( sProperty ) );
//                      std::cout << sTId << " " << sProperty << '\n';
                        break;
                } // switch(e)
            } // while ( it != end )
        } // while ( std::getline(imdb_pp, sLine) ...)
    imdb_ak.close();



/*
 * title.ratins.tsv
tconst  averageRating   numVotes
tt0000001       5.8     1350
tt0000002       6.5     157
tt0000003       6.6     933
tt0000004       6.4     93
tt0000005       6.2     1621
tt0000006       5.7     80
tt0000007       5.5     543
tt0000008       5.6     1441
tt0000009       5.4     62
tt0000010       6.9     4845
tt0000011       5.4     192
tt0000012       7.4     8000
0=id            1=rat   2=numVotes
0=id  1=rating  2=numVotes
*/


//  auto ts = oOdb.FindThing("Cyrus Townsend Brady"); // ...
//  auto ts = oOdb.FindThings("Ed Brady");
//  auto ts = oOdb.FindThings("Richard Adams");
//  auto ts = oOdb.FindThings(std::regex("^.* Anderson$")); std::cout << "'^.* Anderson$' : " << ts.size() << '\n';
//       ts = oOdb.FindThings(std::regex("^.* Adams$"));    std::cout << "'^.* Adams$'    : " << ts.size() << '\n';
//       ts = oOdb.FindThings(std::regex("^Yolanda .*"));   std::cout << "'^Yolanda .*'   : " << ts.size() << '\n';
//       ts = oOdb.FindThings(std::regex(".*Stahl.*"));     std::cout << "'.* Stahl .*'   : " << ts.size() << '\n';

//  ===========================================================================
    std::string sInput;
    char c{'t'};
    do
        {
        std::cout << "---------------- " <<  oOdb.Things().size()     << " things" << '\n';
        std::cout << "---------------- " <<  oOdb.Properties().size() << " properties" << '\n';
        std::cout << "---------------- " <<  oOdb.Reasons().size()    << " reasons" << '\n';
        std::cout << "---------------- " <<  oOdb.Atoms().size()      << " atoms" << '\n';

	std::cout << "\n";
	std::cout << "---------------- Search in (t)hings or (r)easons or (p)roperties or (a)tom or (q)uit or (s)ave&quit: ";
	std::cin >> c;
	switch (c)
            {
            case 't': std::cout << "\n---------------- Search in Things: ";     break;
	    case 'r': std::cout << "\n---------------- Search in Reasons: ";    break;
            case 'p': std::cout << "\n---------------- Search in Properties: "; break;
            case 'a': std::cout << "\n---------------- Search in Atoms: ";      break;
	    default : continue;
            }
//	std::cin >> sInput;
        std::getline (std::cin,sInput); // eat the linebreak
        std::getline (std::cin,sInput);
	odb::CThings     ts;
	odb::CReasons    rs;
	odb::CProperties ps;
	odb::CAtoms      as;

//auto y = oOdb.Find(oOdb.Things(), sInput);
//
//size_t st = 379;
//auto x = oOdb.Find(oOdb.Things(), st);
//if ( x ) std::cout << "--" << '\n' << *x << '\n' << "--" << '\n'; 

	switch (c)
            {
            case 't': ts = oOdb.Find(oOdb.Things(),std::string( sInput )); if (ts.size() == 0) ts = oOdb.Find(oOdb.Things(),std::regex( sInput ));
                      for (auto const & a:ts) { std::cout << '\n' << *a << '\n'; } std::cout << "  total: " << ts.size() << '\n'; 
                      break;

	    case 'r': rs = oOdb.Find(oOdb.Reasons(),std::string( sInput )); if (ts.size() == 0) rs = oOdb.Find(oOdb.Reasons(),std::regex( sInput ));
                      for (auto const & a:rs) { std::cout << '\n' << *a << '\n'; } std::cout << "  total: " << rs.size() << '\n'; 
                      break;

            case 'p': ps = oOdb.Find(oOdb.Properties(),std::string( sInput )); if (ts.size() == 0) ps = oOdb.Find(oOdb.Properties(),std::regex( sInput ));
                      for (auto const & a:ps) { std::cout << '\n' << *a << '\n'; } std::cout << "  total: " << ps.size() << '\n'; 
                      break;

            case 'a': as = oOdb.Find(oOdb.Atoms(),std::string( sInput )); if (ts.size() == 0) as = oOdb.Find(oOdb.Atoms(),std::regex( sInput ));
                      for (auto const & a:as) { std::cout << '\n' << *a << '\n'; } std::cout << "  total: " << as.size() << '\n'; 
                      break;

	    default : continue;
	    }
        } while ( (c != 'q') && (c != 's') );

// sregex_token_iterator
/*
    auto x = oOdb.SelectThingsByProperty( ".ovi.*" );
    for (auto a:x) std::cout << a << '\n';
*/

    if ( c == 's' )
        {
        std::fstream imdb("db.json", std::ifstream::out);
        oOdb.print_json(imdb);
//      oOdb.print_json_stream(imdb);
        imdb.close();
        }
/*
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
*/
    }
