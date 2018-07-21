/**
    @file odbd.cpp
 
    @author Manfred Morgner
    @date 22.03.2018

    to use this server:

    Save a odb to file (JSON) and name it wodb.json. Put this file in the
    same directory as you run the server in and start the server. E.g. by:

        ./odbd 1025 &

    After loading the database a statistic will be print to the console and
    the tcp server starts listening for client connections.

 */

#include <iostream>
#include <memory>
#include <array>

#include <vector>
#include <algorithm>
#include <iterator>

#include <fstream>
#include <json/json.h>

#include "odb.h"

#include "../../../te/include/te.h"


TRenderData const g_oHead{
                     {"title",  { {"", "odb Interactor"} } },
	             {"server", { {"", "odb" }, {"release", "odb.0.9.0" }, {"message", "Welcome" } } },
                   };


using namespace std::string_literals;

std::string g_sTemplate = "nodes.html";
std::string g_sTemplatePath = "../templates/";

auto poOdb = std::make_unique<odb::COdb>();


#include <asio/ip/tcp.hpp>
#include <asio/io_service.hpp>

using asio::ip::tcp;

template<typename T>
size_t CollectDataForTemplate( T const & roContainer, TRenderData & roData, std::string const & crsName)
    {
    TRenderItem oSubM{};
    std::set<size_t> oReasonCollector;
    for ( auto const & a:roContainer )
	{
	if constexpr ( std::is_same<T, odb::MLinkets>() )
	    {
	    oReasonCollector.emplace(a.second->m_nId);
	    oSubM. emplace("",         a.first->m_sName);
	    oSubM. emplace("id",       std::to_string(a.first->m_nId));
	    oSubM. emplace("type",     a.first->m_sType.substr(6));
	    oSubM. emplace("rsn",      a.second->m_sName);
	    oSubM. emplace("rsn-id",   std::to_string(a.second->m_nId));
	    oSubM. emplace("rsn-type", a.second->m_sType.substr(6));
	    }
	else if constexpr ( std::is_same<T, odb::MLinks>() )
	    {
	    oSubM. emplace("",            a.first->m_sName);
	    oSubM. emplace("id",          std::to_string(a.first->m_nId));
	    oSubM. emplace("type",        a.first->m_sType.substr(6));
	    oSubM. emplace("linker",      a.second->m_sName);
	    oSubM. emplace("linker-id",   std::to_string(a.second->m_nId));
	    oSubM. emplace("linker-type", a.second->m_sType.substr(6));
	    }
	else
	    {
	    oSubM. emplace("",      a->m_sName);
	    oSubM. emplace("id",    std::to_string(a->m_nId));
	    oSubM. emplace("type",  a->m_sType.substr(6));
	    }
	roData.emplace(crsName, oSubM);
	oSubM.clear();
	}
    if ( roContainer.size() )
	roData.emplace(crsName + "-hits", TRenderItem{ {"", std::to_string(roContainer.size())} } );
    if constexpr ( std::is_same<T, odb::MLinkets>() )
	{
	if ( oReasonCollector.size() )
	    roData.emplace("Reason-hits", TRenderItem{ {"", std::to_string(oReasonCollector.size())} } );
	}
    return roContainer.size();
    }

template<typename T>
size_t SortDataForTemplate( T const & roContainer, TRenderData & roData, std::string const & crsName )
    {
    size_t nHits = CollectDataForTemplate( roContainer, roData, crsName);
    if ( 1 == nHits )
	{
	for ( auto const & a:roContainer )
	    {
	    if constexpr ( std::is_same<T, odb::SNodes>() || std::is_same<T, odb::CNodes>())
		{
		g_sTemplate = "node.html";
		CollectDataForTemplate( a->Linkets(),    roData, "link-to");
		CollectDataForTemplate( a->Nodes(),      roData, "link-from");
		CollectDataForTemplate( a->Properties(), roData, "Property");
		CollectDataForTemplate( a->Atoms(),      roData, "Atom");
		}
	    if constexpr ( std::is_same<T, odb::SProperties>() || std::is_same<T, odb::CProperties>() )
		{
		g_sTemplate = "property.html";
		CollectDataForTemplate( a->Relations(),  roData, "Node");
		}
	    if constexpr ( std::is_same<T, odb::SReasons>() || std::is_same<T, odb::CReasons>())
		{
		g_sTemplate = "reason.html";
   		CollectDataForTemplate( a->Relations(),  roData, "Node");
		}
	    if constexpr ( std::is_same<T, odb::SAtoms>() || std::is_same<T, odb::CAtoms>())
		{
		g_sTemplate = "atom.html";
   		CollectDataForTemplate( a->Relations(),  roData, "Node");
		}
	    }
	}
    else
	{
	roData.emplace( "result-matches", TRenderItem{ {"", std::to_string(nHits)} } );
	}
    roData.emplace( "result-class",   TRenderItem{ {"", crsName} } );
    return roContainer.size();
    }

void SendError( long nError, std::ostream & ros )
    {
    TRenderData oHead{g_oHead};

    Cte const ote(oHead, "error.html", g_sTemplatePath);
    ros << "HTTP/1.1 " << nError << " Not found" << '\n';
    ros << "Server: odb/0.9.0 (Linux) CPP" << '\n';
    ros << "Content-Length: " << ote.length() << '\n';
    ros << "Content-Language: en" << '\n';
    ros << "Connection: close" << '\n';
    ros << "Content-Type: text/html" << '\n';
    ros << '\n';
    ros << ote;
    }

template<typename T>
void SendResultPage( T const & croContainer, std::ostream & ros )
    {
//  std::cerr << "======================================" << '\n' << croT << '\n';
    ros << "HTTP/1.1 200 OK"                           << '\n';
    ros << "Server: odb/0.9.0 (Linux) C++"             << '\n';
    ros << "Content-Length: " << croContainer.length() << '\n';
    ros << "Content-Language: en"                      << '\n';
    ros << "Connection: close"                         << '\n';
    ros << "Content-Type: text/html"                   << '\n';
    ros << '\n';
    ros << croContainer;
    }

template<typename T>
void SendResult(T const & croContainer, std::iostream & ros, char const ccSwitch, std::string const & crsBadQuery = ""s)
    {
    TRenderData oData{g_oHead};
    if (crsBadQuery > ""s)
	{
	return;
	std::ostringstream oss;
	oss << "ERROR: '" << crsBadQuery << "' invalid expression";
        SendResultPage(oss.str(), ros);
        return;
	}
    if (ccSwitch == 'j')
        {
	std::ostringstream oss;
        oss << " \n { \n";
        poOdb->print_json(croContainer, oss);
        oss << " \n } \n";
        SendResultPage(oss.str(), ros);
        return;
        }

    if (ccSwitch == 'c')
        {
        // ros << " \n";
        }
    else
        {
	std::string sPrimKey = "none"; g_sTemplate = "none.html";

        if constexpr ( std::is_same<T, odb::CNodes>() )
	    {
            sPrimKey = "Node"; g_sTemplate = "nodes.html";
	    }
        if constexpr ( std::is_same<T, odb::CProperties>() )
	    {
            sPrimKey = "Property"; g_sTemplate = "properties.html";
	    }
        if constexpr ( std::is_same<T, odb::CReasons>() )
	    {
            sPrimKey = "Reason"; g_sTemplate = "reasons.html";
	    }
        if constexpr ( std::is_same<T, odb::CAtoms>() )
	    {
            sPrimKey = "Atom"; g_sTemplate = "atoms.html";
	    }

        SortDataForTemplate( croContainer, oData, sPrimKey );
        std::cout << g_sTemplate << '\n';
        Cte ote(oData, g_sTemplate, g_sTemplatePath);
        SendResultPage( ote, ros );
/*
        // prints raw data to console
        for (auto & a:o)
            {
            std::cout << a.first << ", [ ";
            for (auto & b:a.second)
                {
                std::cout << "('" << b.first << "', '" << b.second << "') ";
                }
            std::cout << "]\n";
            }
*/

/*
        for (auto const & a:croData)
            {
            ros << " \n" << ":" << a->m_nId << ":";
            if ( (ccSwitch == ':') || ((ccSwitch >= 'A')&&(ccSwitch <= 'Z')) )
                {
                ros << *a << " \n";
                }
            else
                {
                ros << (*a).m_sName;
                }
            }
*/
        } // else if (ccSwitch == 'c')
/*
    if (ccSwitch == '.')
        {
        ros << " \n";
        }

    ros << " \n  total: " << croData.size() << " \n";
*/
    }

void SendStatistics(std::ostream & ros)
    {
    TRenderData oData{g_oHead};

    oData.emplace( "Nodes",      TRenderItem{ { "count", std::to_string(poOdb->Nodes().size())      } }  );
    oData.emplace( "Properties", TRenderItem{ { "count", std::to_string(poOdb->Properties().size()) } }  );
    oData.emplace( "Reasons",    TRenderItem{ { "count", std::to_string(poOdb->Reasons().size())    } }  );
    oData.emplace( "Atoms",      TRenderItem{ { "count", std::to_string(poOdb->Atoms().size())      } }  );

    Cte ote(oData, "statistic.html", g_sTemplatePath);
    SendResultPage(ote, ros);
    }



bool FindUnuseds(std::string const & crsQuery, tcp::iostream & ros)
    {
//    std::cout << "? " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

//  char c = crsQuery[1];
    char d = crsQuery[0];
    std::string sInput = crsQuery.substr(2);

    odb::CNodes      ns;
    odb::CReasons    rs;
    odb::CProperties ps;
    odb::CAtoms      as;
    switch (d)
        {
	case 'n': ns = poOdb->FindUnUsedNodes();      SendResult(ns, ros, d); break;
        case 'r': rs = poOdb->FindUnUsedReasons();    SendResult(rs, ros, d); break;
        case 'p': ps = poOdb->FindUnUsedProperties(); SendResult(ps, ros, d); break;
        case 'a': as = poOdb->FindUnUsedAtoms();      SendResult(as, ros, d); break;
        default : SendError( 494, ros ); // std::cout << ": no result";
                  return false;
        }

    return true;
    }

bool Insert(std::string const & crsQuery, std::iostream & ros)
    {
//    std::cout << "a " + crsQuery;

    if ( crsQuery.length() < 2 ) { SendError(409, ros); return false; }

    char c = crsQuery[0];
    std::string sInput = crsQuery.substr(2);

    odb::CNodes      ns;
    odb::CReasons    rs;
    odb::CProperties ps;
    odb::CAtoms      as;

    switch (c)
        {
        case 'n': ns.emplace(poOdb->FindOrMakeNode(sInput));     SendResult(ns, ros, 'a'); break;
        case 'r': rs.emplace(poOdb->FindOrMakeReason(sInput));   SendResult(rs, ros, 'a'); break;
        case 'p': ps.emplace(poOdb->FindOrMakeProperty(sInput)); SendResult(ps, ros, 'a'); break;
        case 'a': as.emplace(poOdb->FindOrMakeAtom(sInput));     SendResult(as, ros, 'a'); break;

        default : SendError(409, ros);
        }

    return true;
    }

bool LinkNAppend(std::string const & crsQuery, std::ostream & ros)
    {
    /*
    stream << "ltidT:idT:idR        - links two Nodes by ID\n";
    stream << "lTnameT:nameT:nameR  - links two Nodes by Name\n";
    stream << "lpidP:idT            - links Property to Node\n";
    stream << "lPnameP:nameT        - links Property to Node\n";
    stream << "laidA:idT            - links Atom to Node\n";
    stream << "lAnameA:nameT        - links Atom to Node\n";
    */
    ros << "L " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char d = crsQuery[1];
    std::string sInput = crsQuery.substr(2);
    
    bool  bNumeric = (d > 'Z');
    short nArgs    = ( (d == 't') || (d == 'T') ) ? 3 : 2;
    
    std::regex r(":");
    std::regex_token_iterator<std::string::iterator> end;
    std::regex_token_iterator<std::string::iterator> it(sInput.begin(), sInput.end(), r, -1);

    std::string sVal1;
    std::string sVal2;
    std::string sVal3;
    short       nVals{0};
    if (it != end) { sVal1 = *it++; nVals++; }
    if (it != end) { sVal2 = *it++; nVals++; }
    if (it != end) { sVal3 = *it;   nVals++; }

    size_t nVal1{0};
    size_t nVal2{0};
    size_t nVal3{0};
    if (nVals != nArgs)
        {
        ros << " \nE invalid count of parameters\n";
        return false;
        }
    if (bNumeric)
        {
        try
            {
            nVal1 = stoull(sVal1);
            nVal2 = stoull(sVal2);
            if (nVals==3) nVal3 = stoull(sVal3);
            }
        catch (...)
            {
            ros << " \nE invalid type of parameters\n";
            return false;
            }
        }
    /*
    stream << "ltidT:idT:idR        - links two Nodes by ID\n";
    stream << "lTnameT:nameT:nameR  - links two Nodes by Name\n";
    stream << "lpidP:idT            - links Property to Node\n";
    stream << "lPnameP:nameT        - links Property to Node\n";
    stream << "laidA:idT            - links Atom to Node\n";
    stream << "lAnameA:nameT        - links Atom to Node\n";
    */
    bool b{false};
    switch (d)
        {
        case 'n': b = poOdb->LinkNode2Node(nVal1, nVal2, nVal3);       break;
        case 'N': b = poOdb->LinkNode2Node(sVal1, sVal2, sVal3);       break;
        case 'p': b = poOdb->AppendProperty2Node(nVal1, nVal2);        break;
        case 'P': b = poOdb->AppendProperty2Node(sVal1, false, sVal2); break;
        case 'a': b = poOdb->AppendAtom2Node(nVal2, nVal1);            break;
        case 'A': ros << " \n! Not implemented\n";
                  break;
        }
    if (b)
        ros << " \n! Linked\n";
    else
        ros << " \nE could not assign objects\n";
    return b;
    }


bool Answer(std::string const & crsQuery, tcp::iostream & ros)
    {
//    ros << "? " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char c = crsQuery[0];
    char d = crsQuery[1];
    std::string sInput = crsQuery.substr(2);

    odb::CNodes      ns;
    odb::CReasons    rs;
    odb::CProperties ps;
    odb::CAtoms      as;
    auto             b = false;
    long nInput        = 1;
    if ( (d=='i') || (d=='n') )
	{
	try { nInput = stol(sInput); } catch(...) {  }
	}
    switch (c)
        {
        case 't':
        case 'n': if ( (d=='p') || (d=='P') )
                    {
                    ns = poOdb->FindNodesByProperty(sInput);
                    if (ns.size() == 0) { try { ns = poOdb->FindNodesByProperty(std::regex(sInput)); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
                    d = (d=='p') ? ':' : '.';
                    }
                  else if (d=='i')
                    {
                    auto oop = poOdb->FindNode(nInput);
                    if ( oop.has_value() )
                	{
                	ns.insert( oop.value() );
                	}
                    }
                  else
		    {
		    ns = poOdb->FindNodes(sInput);
		    if (ns.size() == 0)  { try { ns = poOdb->FindNodes(std::regex( sInput )); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
		    }
                  SendResult(ns, ros, d, (b)?sInput:""s);
                  break;
        case 'r': if (d=='i')
                    {
                    auto oop = poOdb->FindReason(nInput);
                    if ( oop.has_value() )
                	{
                	rs.insert( oop.value() );
                	}
                    }
                  else
		    {
		    rs = poOdb->FindReasons(std::string( sInput ));
		    if (rs.size() == 0) { try { rs = poOdb->FindReasons(std::regex( sInput )); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
                    }
                  SendResult(rs, ros, d, (b)?sInput:""s);
                  break;
        case 'p': if (d=='i')
                    {
                    auto oop = poOdb->FindProperty(nInput);
                    if ( oop.has_value() )
                	{
                	ps.insert( oop.value() );
                	}
                    }
                  else if (d=='n')
                      {
                      auto oon = poOdb->FindNode(nInput);
                      if ( oon.has_value() )
                  	{
//                	std::cout << *oon.value() << " " << nInput << '\n';
                    	for ( auto const & a:oon.value()->Properties() )
                    	    {
//                    	    std::cout << *a << '\n';
                    	    ps.insert( a );
                    	    }
                  	}
                      }
                  else
		    {
		    ps = poOdb->FindProperties(std::string( sInput ));
                    if (ps.size() == 0) { try { ps = poOdb->FindProperties(std::regex( sInput )); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
                    }
                  SendResult(ps, ros, d, (b)?sInput:""s);
                  break;
        case 'a': if (d=='i')
                    {
                    auto oop = poOdb->FindAtom(nInput);
                    if ( oop.has_value() )
                	{
                	as.insert( oop.value() );
                	}
                    }
                  else
		    {
		    as = poOdb->FindAtoms(std::string( sInput ));
                    if (as.size() == 0) { try { as = poOdb->FindAtoms(std::regex( sInput )); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
                    }
                  SendResult(as, ros, d, (b)?sInput:""s);
                  break;
        default : SendError( 404, ros);
                  return false;
        }

    return true;
    } // bool Answer(std::string const & crsQuery, tcp::iostream & ros)

//

int main(int argc, char* argv[])
    {
    if (argc != 2)
        {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        std::cout << "Example:\n";
        std::cout << "  " << argv[0] << " 1025 &\n";
        return 1;
        }

    auto const sFilename =  "wwwdb.json"; // "test_out2.json"; // "wwwdb.json";

    poOdb->LoadDB(sFilename);

    std::cout << '\n';

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
            std::string sLine{};
            std::getline(stream, sLine);
            std::string sQuery = sLine;

std::string sMoz = R"(
GET /?ai=4 HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:59.0) Gecko/20100101 Firefox/59.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: de,en-US;q=0.7,en;q=0.3
Accept-Encoding: gzip, deflate
Referer: http://localhost:8080/?pi=2
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Pragma: no-cache
Cache-Control: no-cache
)";

	    std::string sQI{};

//	    std::cout << "QI: |" << sQI << "|Query|" << sQuery << "|\n";

	    std::regex const reSpace("%20");
	    sQI = std::regex_replace(sQuery, reSpace, " ");
	    sQuery = sQI;

//	    std::cout << "QI: |" << sQI << "|Query|" << sQuery << "|\n";

	    std::regex const reSplus("\\+");
	    sQI = std::regex_replace(sQuery, reSplus, " ");
	    sQuery = sQI;

//	    std::cout << "QI: |" << sQI << "|Query|" << sQuery << "|\n";

	    std::regex const reStar ("%2A");
	    sQI = std::regex_replace(sQuery, reStar, "*");
	    sQuery = sQI;

	    std::regex const reColon ("%3A");
	    sQI = std::regex_replace(sQuery, reColon, ":");
	    sQuery = sQI;

	    std::cout << "QI: |" << sQI << "|Query|" << sQuery << "|\n";
            if (sQuery.substr(0, 6) != R"(GET /?)" )
        	{
                sQI = "GET /?st=at HTTP/1.1"s;
         	}

	    std::smatch      sm{};
//	    std::regex const re(R"(/?(..)=([^ ]+))");
	    std::regex const re(R"(/?(..)=([^&]+) HTTP.*)");
	    std::regex_search(sQI, sm, re);
	    if ( sm.size() > 2 )
		{
		sQuery  = sm[1];
		sQuery += sm[2];
		}
	    else
		{
		sQuery = "";
                std::regex const re(R"(/?([^ ]+))");
                std::regex_search(sQI, sm, re);
                if ( sm.size() > 1 )
                    {
                    sQuery  = sm[1];
                    }
		}
            std::cout << "Qy: |" << sQuery << "|\n";


            if ( sQuery == "stat" )
                {
                SendStatistics(stream);
                }
            else if ( sQuery == "clean" )
                {
//                stream << "Emptying DB, old statistics:\n";
//                SendStatistics(stream);
//                stream << "Cleaning, please wait\n";
                poOdb = std::make_unique<odb::COdb>();
                SendStatistics(stream);
//                stream << "Done\n";
                }
/*
            else if ( sQuery == "fillr" )
                {
                stream << "Fill DB with small set of data, old statistics:\n";
                SendStatistics(stream);
                stream << "Cleaning, please wait\n";
                poOdb = std::make_unique<odb::COdb>();
                stream << "Filling, please wait\n";
                FillInSomeData();
                SendStatistics(stream);
                stream << "Done\n";
                }
*/
            else if ( sQuery == "load" )
                {
//                stream << "Replacing DB, old statistics:\n";
//                SendStatistics(stream);
                poOdb = std::make_unique<odb::COdb>();
//                stream << "Loading, please wait\n";
                poOdb->LoadDB(sFilename);
                SendStatistics(stream);
//                stream << "Done\n";
                }
            else if ( sQuery == "save" )
                {
                stream << "Saving DB, statistics:\n";
                SendStatistics(stream);
                poOdb->SaveDB(sFilename);
                stream << "Done\n";
                }
            else if ( sQuery == "help" )
                {
                TRenderData oData(g_oHead);
                oData.emplace("help", TRenderItem{{ "", "help"    },{ "query", "/?he=lp"                 },{ "text", "shows this help page" }});
                oData.emplace("help", TRenderItem{{ "", "stat"    },{ "query", "/?st=at"                 },{ "text", "shows db statistics" }});
                oData.emplace("help", TRenderItem{{ "", "clean"   },{ "query", "/?cl=ean"                },{ "text", "deletes all entries from DB" }});
                oData.emplace("help", TRenderItem{{ "", "fillr"   },{ "query", "/?fi=llr"                },{ "text", "fills the DB with test data" }});
                oData.emplace("help", TRenderItem{{ "", "load"    },{ "query", "/?lo=ad"                 },{ "text", "loads the DB from disk" }});
                oData.emplace("help", TRenderItem{{ "", "save"    },{ "query", "/?sa=ve"                 },{ "text", "saves the DB to disk" }});
                oData.emplace("help", TRenderItem{{ "", "t:regex" },{ "query", "/?t:=.*Star.*"           },{ "text", "search for a node" }});
                oData.emplace("help", TRenderItem{{ "", "p:regex" },{ "query", "/?p:=.*dire.*"           },{ "text", "search for a property" }});
                oData.emplace("help", TRenderItem{{ "", "r:regex" },{ "query", "/?r:=.*know.*"           },{ "text", "search for a reason" }});
                oData.emplace("help", TRenderItem{{ "", "a:regex" },{ "query", "/?a:=.*size.*"           },{ "text", "search for an atom" }});
                oData.emplace("help", TRenderItem{{ "", "tpregex" },{ "query", "/?tp=.*role.*"           },{ "text", "search for a node having a specific property" }});
                oData.emplace("help", TRenderItem{{ "", "t.regex" },{ "query", "/?t.=.*Star.*"           },{ "text", "search for a node" }});
                oData.emplace("help", TRenderItem{{ "", "p.regex" },{ "query", "/?p.=.*dire.*"           },{ "text", "search for a property (short result)" }});
                oData.emplace("help", TRenderItem{{ "", "r.regex" },{ "query", "/?r.=.*know.*"           },{ "text", "search for a reason (short result)" }});
                oData.emplace("help", TRenderItem{{ "", "a.regex" },{ "query", "/?a.=Star (Trek|Wars).*" },{ "text", "search for an atom (short result)" }});
                oData.emplace("help", TRenderItem{{ "", "tcregex" },{ "query", "/?tc=Star (Trek|Wars).*" },{ "text", "search for nodes, returns only the result count" }});
                oData.emplace("help", TRenderItem{{ "", "tjregex" },{ "query", "/?tj=Star (Trek|Wars).*" },{ "text", "search for nodes, returns result in JSON format" }});
                oData.emplace("help", TRenderItem{{ "", "tPregex" },{ "query", "/?tP=director"           },{ "text", "search for a node having a specific property (short result)" }});
                oData.emplace("help", TRenderItem{{ "", "naname"  },{ "query", "/?na=name"               },{ "text", "insert a node" }});
                oData.emplace("help", TRenderItem{{ "", "paname"  },{ "query", "/?pa=name"               },{ "text", "insert a property" }});
                oData.emplace("help", TRenderItem{{ "", "raname"  },{ "query", "/?ra=name"               },{ "text", "insert a reason" }});
                oData.emplace("help", TRenderItem{{ "", "aaname"  },{ "query", "/?aa=name"               },{ "text", "insert an atom" }});
                oData.emplace("help", TRenderItem{{ "", "pnid"    },{ "query", "/?pn=2"                  },{ "text", "find properties for a node by id" }});
                oData.emplace("help", TRenderItem{{ "", "ne0"     },{ "query", "/?en=0"                  },{ "text", "find unused nodes" }});
                oData.emplace("help", TRenderItem{{ "", "pe0"     },{ "query", "/?ep=0"                  },{ "text", "find unused properties" }});
                oData.emplace("help", TRenderItem{{ "", "re0"     },{ "query", "/?er=0"                  },{ "text", "find unused reasons" }});
                oData.emplace("help", TRenderItem{{ "", "ae0"     },{ "query", "/?ea=0"                  },{ "text", "find unused atoms" }});
                oData.emplace("help", TRenderItem{{ "", "ee0"     },{ "query", "/?ee=0"                  },{ "text", "find unused elements" }});
                oData.emplace("help", TRenderItem{{ "", "" }, { "query", "" },{ "text", "" }});
                Cte ote(oData, "help.html", g_sTemplatePath);
                SendResultPage( ote, stream );

                stream << "ltidT:idR:idT        - links two Nodes by ID\n";
                stream << "lTnameT:nameR:nameT  - links two Nodes by Name\n";
                stream << "lpidP:idT            - links Property to Node\n";
                stream << "lPnameP:nameT        - links Property to Node\n";
                stream << "laidA:idT            - links Atom to Node\n";
                stream << "lAnameA:nameT        - links Atom to Node\n";
                }
            else
                {
                if ( sQuery.length() < 2 )
		    {
		    stream << "try: 'help' to get help\n";
		    }
                else if ( sQuery[1] == 'e' )
		    {
		    if ( not FindUnuseds(sQuery, stream) ) stream << ": not bound\n";
		    }
                else if ( (sQuery[1] == ':') || (sQuery[1] == '.') ||
                	  (sQuery[1] == 'c') || (sQuery[1] == 'i') || (sQuery[1] == 'j') ||
			  (sQuery.substr(0,2) == "tP") ||
			  (sQuery.substr(0,2) == "tp") ||
			  (sQuery.substr(0,2) == "nP") ||
			  (sQuery.substr(0,2) == "np") ||
			  (sQuery.substr(0,2) == "pn") ||
			  (sQuery.substr(0,2) == "pn") )
		    {
		    if ( not Answer(sQuery, stream) ) stream << ": no result\n";
		    }
                else if ( sQuery[1] == 'a' )
		    {
		    if ( not Insert(sQuery, stream) ) stream << ": not inserted\n";
		    }
                else if ( sQuery[0] == 'l' )
		    {
		    if ( not LinkNAppend(sQuery, stream) ) stream << ": not bound\n";
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

