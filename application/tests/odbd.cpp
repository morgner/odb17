/**
    @file main.cpp
 
    @author Manfred Morgner
    @date 22.03.2018

    to use this server:

    Save a odb to file (JSON) and name it wodb.json. Put this file in the
    same directory as you run the server in and start the server. E.g. by:

        ./odbd 1025 &

    After loading the database a statistic will be print to the console and
    the tcp server starts listening for client conections.

 */

#include <iostream>
#include <memory>
#include <array>

#include <vector>
#include <algorithm>
#include <iterator>

#include <fstream>
#include <json/json.h>

#include "generic.h"
#include "odb.h"
#include "atom.h"
#include "thing.h"

using namespace std::string_literals;

auto oOdb = odb::COdb();
/*
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
*/
            

#include <asio/ip/tcp.hpp>
#include <asio/io_service.hpp>

using asio::ip::tcp;


bool Insert(std::string const & crsQuery, tcp::iostream & ros)
    {
    ros << "+ " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char c = crsQuery[0];
    std::string sInput = crsQuery.substr(2);

    odb::PThing    t;
    odb::PReason   r;
    odb::PProperty p;
    odb::PAtom     a;

    switch (c)
        {
        case 't': t = oOdb.MakeThing(sInput);    ros << " \n" << ":" << t->m_nId << ":" << *t << " \n";
                  break;

        case 'r': r = oOdb.MakeReason(sInput);   ros << " \n" << ":" << r->m_nId << ":" << *r << " \n";
                  break;

        case 'p': p = oOdb.MakeProperty(sInput); ros << " \n" << ":" << p->m_nId << ":" << *p << " \n";
                  break;

        case 'a': a = oOdb.MakeAtom(sInput);     ros << " \n" << ":" << a->m_nId << ":" << *a << " \n";
                  break;

        default : ros << ": no insert";
        }

    return true;
    }

bool Answer(std::string const & crsQuery, tcp::iostream & ros)
    {
    ros << "? " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char c = crsQuery[0];
    char d = crsQuery[1];
    std::string sInput = crsQuery.substr(2);

    odb::CThings     ts;
    odb::CReasons    rs;
    odb::CProperties ps;
    odb::CAtoms      as;

    switch (c)
        {
        case 't': if ( (d=='p') || (d=='P') )
                    {
                    ts = oOdb.FindThingsByProperty(std::string( sInput )); if (ts.size() == 0) ts = oOdb.FindThingsByProperty(std::regex( sInput ));
                    }
                  else
                    {
                    ts = oOdb.Find(oOdb.Things(),std::string( sInput )); if (ts.size() == 0) ts = oOdb.Find(oOdb.Things(),std::regex( sInput ));
                    }
                  if ( d=='s' )
                    {
                    ros << "  total: " << ts.size() << " \n";
                    }
                  else
                    {
                    for (auto const & a:ts) { ros << " \n:" << a->m_nId << ":"; if ((d!='.')&&(d!='p')) ros << *a << " \n"; else ros << (*a).m_sName;  } 
                                            if ((d=='.')||(d=='p')) ros << " \n"; ros << " \n  total: " << ts.size() << " \n";
                    }
                  break;

        case 'r': rs = oOdb.Find(oOdb.Reasons(),std::string( sInput )); if (rs.size() == 0) rs = oOdb.Find(oOdb.Reasons(),std::regex( sInput ));
//                for (auto const & a:rs) { ros << " \n" << ":" << a->m_nId << ":" << *a << " \n"; } ros << "  total: " << rs.size() << " \n";
                  for (auto const & a:rs) { ros << " \n" << ":" << a->m_nId << ":"; if (d==':') ros << *a << " \n"; else ros << (*a).m_sName;  }
                                          if (d=='.') ros << " \n"; ros << " \n  total: " << rs.size() << " \n";
                  break;

        case 'p': ps = oOdb.Find(oOdb.Properties(),std::string( sInput )); if (ps.size() == 0) ps = oOdb.Find(oOdb.Properties(),std::regex( sInput ));
//                for (auto const & a:ps) { ros << " \n" << ":" << a->m_nId << ":" << *a << " \n"; } ros << "  total: " << ps.size() << " \n";
                  for (auto const & a:ps) { ros << " \n" << ":" << a->m_nId << ":"; if (d==':') ros << *a << " \n"; else ros << (*a).m_sName;  }
                                          if (d=='.') ros << " \n"; ros << " \n  total: " << ps.size() << " \n";
                  break;

        case 'a': as = oOdb.Find(oOdb.Atoms(),std::string( sInput )); if (as.size() == 0) as = oOdb.Find(oOdb.Atoms(),std::regex( sInput ));
//                for (auto const & a:as) { ros << " \n" << ":" << a->m_nId << ":" << *a << " \n"; } ros << "  total: " << as.size() << " \n";
                  for (auto const & a:as) { ros << " \n" << ":" << a->m_nId << ":"; if (d==':') ros << *a << " \n"; else ros << (*a).m_sName;  }
                                          if (d=='.') ros << " \n"; ros << " \n  total: " << as.size() << " \n";
                  break;

        default : ros << ": no result";
        }

    return true;
    } // bool Answer(std::string const & crsQuery, tcp::iostream & ros)


int main(int argc, char* argv[])
    {
    if (argc != 2)
        {
        std::cout << "Usage: " << argv[0] << " <ip> <port>\n";
        std::cout << "Example:\n";
        std::cout << "  " << argv[0] << " 1025 &\n";
        return 1;
        }

    auto const sFilename = "wdb.json";

    oOdb.LoadDB(sFilename);

    std::cout << '\n';
    std::cout << "---------------- " <<  oOdb.Things().size()     << " things" << '\n';
    std::cout << "---------------- " <<  oOdb.Properties().size() << " properties" << '\n';
    std::cout << "---------------- " <<  oOdb.Reasons().size()    << " reasons" << '\n';
    std::cout << "---------------- " <<  oOdb.Atoms().size()      << " atoms" << '\n';

    try
        {
        asio::io_service io_service;

        tcp::endpoint endpoint(tcp::v4(), std::stoul(argv[1]));
        tcp::acceptor acceptor(io_service, endpoint);

        for (;;)
            {
            tcp::iostream stream;
            asio::error_code ec;
            acceptor.accept(*stream.rdbuf(), ec);
            std::string sQuery;
            std::getline(stream, sQuery);
            if ( sQuery == "stat" )
                {
                stream << "---------------- " <<  oOdb.Things().size()     << " things" << " \n";
                stream << "---------------- " <<  oOdb.Properties().size() << " properties" << " \n";
                stream << "---------------- " <<  oOdb.Reasons().size()    << " reasons" << " \n";
                stream << "---------------- " <<  oOdb.Atoms().size()      << " atoms" << " \n";
                }
            else
            if ( sQuery == "help" )
                {
                stream << "help    - this help page\n";
                stream << "stat    - db statistics\n";
                stream << "save    . saves the DB to disk\n";
                stream << " \n";
                stream << "t:regex - search for a thing\n";
                stream << "p:regex - search for a property\n";
                stream << "r:regex - search for a reason\n";
                stream << "a:regex - search for an atom\n";
                stream << "tPregex - search for a thing having the given property\n";
                stream << " \n";
                stream << "t.regex - search for a thing (short result)\n";
                stream << "p.regex - search for a property (short result)\n";
                stream << "r.regex - search for a reason (short result)\n";
                stream << "a.regex - search for an atom (short result)\n";
                stream << "tpregex - search for a thing having the given property (short result)\n";
                stream << " \n";
                stream << "Example\n";
                stream << " \n";
                stream << "t:Star (Trek|Wars).*\n";
                stream << "t.Star (Trek|Wars).*\n";
                stream << " \n";
                stream << "Searches for all 'things' named \"Star Trek\" or \"Star Wars\"\n";
                stream << " \n";
                stream << "t+name  - insert a thing\n";
                stream << "p+name  - insert a property\n";
                stream << "r+name  - insert a reason\n";
                stream << "a+name  - insert an atom\n";
                }
            else
                {
                if ( sQuery == "save" )
                    {
                    oOdb.SaveDB(sFilename);
                    }
                else if ( sQuery.length() < 2 )
                        {
                        stream << "try: 'help' to get help\n";
                        }
                else if ( (sQuery[1] == ':') || (sQuery[1] == '.') || (sQuery[1] == 's') || (sQuery.substr(0,2) == "tP") || (sQuery.substr(0,2) == "tp") )
                        {
                        if ( not Answer(sQuery, stream) ) stream << ": no result\n";
                        }
                else if ( sQuery[1] == '+' )
                        {
                        if ( not Insert(sQuery, stream) ) stream << ": not inserted\n";
                        }
                else
                    {
                    stream << ": did't understand";
                    }
                }
            stream << ".\n";
            }
        }
    catch (std::exception& e)
        {
        std::cerr << e.what() << std::endl;
        }

    return 0;
    }

