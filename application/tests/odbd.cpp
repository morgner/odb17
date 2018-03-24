/**
    @file main.cpp
 
    @author Manfred Morgner
    @date 22.03.2018
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

#include <json/json.h>

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

void SaveDB( )
    {
    std::fstream imdb("wdb.json", std::ifstream::out);
    oOdb.print_json(imdb);
//  oOdb.print_json_stream(imdb);
    imdb.close();
    }
            
void LoadDB( )
    {            
    Json::Value json;
    std::ifstream db_dump("wdb.json", std::ifstream::binary);
    db_dump >> json;

    const Json::Value & properties = json["Object Database Dump"]["Properties"];
//    std::cout << "odb read in " << properties.size() << " properties.\n";
    for ( size_t index = 0; index < properties.size(); ++index )
        {
        auto nId   = properties[(int)index].get("id",    0).asUInt();
        auto sName = properties[(int)index].get("name", "").asString();
        oOdb.LoadProperty(nId, sName);
        }

    const Json::Value & atoms = json["Object Database Dump"]["Atoms"];
//    std::cout << "odb read in " << atoms.size() << " atoms.\n";
    for ( size_t index = 0; index < atoms.size(); ++index )
        {
        auto nId     = atoms[(int)index].get("id",      0).asUInt();
        auto sName   = atoms[(int)index].get("name",   "").asString();
        auto sPrefix = atoms[(int)index].get("prefix", "").asString();
        auto sSuffix = atoms[(int)index].get("suffix", "").asString();
        auto sFormat = atoms[(int)index].get("format", "").asString();
        auto sData   = atoms[(int)index].get("data",   "").asString();
        oOdb.LoadAtom(nId, sData, sName, sPrefix, sSuffix, sFormat);
        }

    const Json::Value & reasons = json["Object Database Dump"]["Reasons"];
//    std::cout << "odb read in " << reasons.size() << " reasons.\n";
    for ( size_t index = 0; index < reasons.size(); ++index )
        {
        auto nId   = reasons[(int)index].get("id",    0).asUInt();
        auto sName = reasons[(int)index].get("name", "").asString();
        oOdb.LoadReason(nId, sName);
        }

    const Json::Value & things = json["Object Database Dump"]["Things"];
//    std::cout << "odb read in " << things.size() << " things.\n";
    for ( size_t index = 0; index < things.size(); ++index )
        {
        auto nId   = things[(int)index].get("id",    0).asUInt();
        auto sName = things[(int)index].get("name", "").asString();
        oOdb.LoadThing(nId, sName);
        }
    for ( size_t index = 0; index < things.size(); ++index )
        {
        auto nId   = things[(int)index].get("id",    0).asUInt();

        const Json::Value & p = things[(int)index]["properties"];
        for ( size_t i = 0; i < p.size(); ++i )
            {
            auto nPId = p[(int)i].get("id", 0).asUInt();
            oOdb.AppendProperty2Thing( nPId, nId );
            }

        const Json::Value & a = things[(int)index]["atoms"];
        for ( size_t i = 0; i < a.size(); ++i )
            {
            auto nAId = a[(int)i].get("id", 0).asUInt();
            oOdb.AppendAtom2Thing( nAId, nId );
            }

        const Json::Value & l = things[(int)index]["links"];
        for ( size_t i = 0; i < l.size(); ++i )
            {
            auto nTId = l[(int)i].get("thing-id",  0).asUInt();
            auto nRId = l[(int)i].get("reason-id", 0).asUInt();
            oOdb.LinkThing2Thing( nId, nTId, nRId );
            }
        }
    } // LoadDB(...)    

// Transport
// ~~~~~~~~~
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio/ip/tcp.hpp>
#include <asio/io_service.hpp>

using asio::ip::tcp;

bool Answer(std::string const & crsQuery, tcp::iostream & ros)
    {
    ros << "< " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char c = crsQuery[0];
    std::string sInput = crsQuery.substr(2, std::string::npos);

    odb::CThings     ts;
    odb::CReasons    rs;
    odb::CProperties ps;
    odb::CAtoms      as;

    switch (c)
        {
        case 't': ts = oOdb.Find(oOdb.Things(),std::string( sInput )); if (ts.size() == 0) ts = oOdb.Find(oOdb.Things(),std::regex( sInput ));
                  for (auto const & a:ts) { ros << '\n' << *a << '\n'; } ros << "  total: " << ts.size() << '\n';
                  break;

        case 'r': rs = oOdb.Find(oOdb.Reasons(),std::string( sInput )); if (ts.size() == 0) rs = oOdb.Find(oOdb.Reasons(),std::regex( sInput ));
                  for (auto const & a:rs) { ros << '\n' << *a << '\n'; } ros << "  total: " << rs.size() << '\n';
                  break;

        case 'p': ps = oOdb.Find(oOdb.Properties(),std::string( sInput )); if (ts.size() == 0) ps = oOdb.Find(oOdb.Properties(),std::regex( sInput ));
                  for (auto const & a:ps) { ros << '\n' << *a << '\n'; } ros << "  total: " << ps.size() << '\n';
                  break;

        case 'a': as = oOdb.Find(oOdb.Atoms(),std::string( sInput )); if (ts.size() == 0) as = oOdb.Find(oOdb.Atoms(),std::regex( sInput ));
                  for (auto const & a:as) { ros << '\n' << *a << '\n'; } ros << "  total: " << as.size() << '\n';
                  break;

        default : ros << ": empty-result";
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

    LoadDB();

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
            if ( sQuery == "help" )
                {
                stream << "help    - this help page\n";
                stream << " \n";
                stream << "t:regex - search for \n";
                stream << "p:regex - search for \n";
                stream << "r:regex - search for \n";
                stream << "a:regex - search for \n";
                stream << "t:regex - search for \n";
                stream << " \n";
                stream << "Example\n";
                stream << " \n";
                stream << "t:Star Trek.*\n";
                }
            else
                {
                if ( not Answer(sQuery, stream) ) stream << "\n";
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

