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





#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
    {
    public:
        session(tcp::socket socket)
        : m_oSocket(std::move(socket))
        { }

        void start()
            {
            do_read();
            }

    private:
        void do_read()
            {
            auto self(shared_from_this());
            m_oSocket.async_read_some(asio::buffer(m_acBuffer, m_nBufferSize), [&](std::error_code ec, std::size_t length) { if (!ec) { do_write(length); } });
            }

        void do_write(std::size_t length)
            {
            auto self(shared_from_this());
            asio::async_write(m_oSocket, asio::buffer(m_acBuffer, 1024), [this, self](std::error_code ec, std::size_t) { if (!ec) { do_read(); } });
            }

        tcp::socket m_oSocket;
        enum { m_nBufferSize = 1024 };
        char m_acBuffer[m_nBufferSize];
    }; // class sesssion

class server
{
    public:
        server(asio::io_context& io_context, short port)
        : acceptor_(io_context,
          tcp::endpoint(tcp::v4(), port)),
          m_oSocket(io_context)
            {
            do_accept();
            }

    private:
        void do_accept()
            {
            acceptor_.async_accept(m_oSocket, [this](std::error_code ec) { if (!ec) { std::make_shared<session>(std::move(m_oSocket))->start(); } do_accept(); });
            }

    tcp::acceptor acceptor_;
    tcp::socket m_oSocket;
    }; // class server


// Demo main program
int main( int argc, const char* argv[] )
    {
    LoadDB();

    std::cout << '\n';
    std::cout << "---------------- " <<  oOdb.Things().size()     << " things" << '\n';
    std::cout << "---------------- " <<  oOdb.Properties().size() << " properties" << '\n';
    std::cout << "---------------- " <<  oOdb.Reasons().size()    << " reasons" << '\n';
    std::cout << "---------------- " <<  oOdb.Atoms().size()      << " atoms" << '\n';

    try
        {
        if (argc != 2)
            {
            std::cerr << "Usage: odbd <port>\n";
            return 1;
            }

        asio::io_context io_context;

        server s(io_context, std::atoi(argv[1]));

        io_context.run();
        }
    catch (std::exception& e)
        {
        std::cerr << "Exception: " << e.what() << "\n";
        }

    return 0;

    }
