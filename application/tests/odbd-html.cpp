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


using namespace std::string_literals;

std::string g_sTemplate = "nodes.html";
std::string g_sTemplatePath = "../templates/";

auto poOdb = std::make_unique<odb::COdb>();

template<typename ...Args> void mknodes     (Args&&... args) { (poOdb->MakeNode    (args), ...); }
template<typename... Args> void mkproperties(Args&&... args) { (poOdb->MakeProperty(args), ...); }
template<typename... Args> void mkreasons   (Args&&... args) { (poOdb->MakeReason  (args), ...); }
template<typename... Args> void mkatoms     (Args&&... args) { (poOdb->MakeAtom    (args, "fold" ), ...); }

// append a property to a group of nodes, if property does not exists and
// 'cbForce' is 'true', the property will be added to the DB
template<typename... Args>
void ap2ts(std::string const & crsProperty, // name of the property
                  bool const   cbForce,     // create it if not existent?
                     Args&&... args)        // pack of names of 'nodes'
    {
    (poOdb->AppendProperty2Node(crsProperty, cbForce, args), ...);
    }
        
template<typename... Args>
void lt2t(std::string const & crsNameTo, // name of the property
          std::string const & crsReason, // create it if not existent?
                    Args&&... args)      // pack of names of 'nodes'
    {
    (poOdb->LinkNode2Node(args, crsReason, crsNameTo), ...);
    }

void FillInSomeData()
    {
    // filling in some data
    // ================================================================================================
    mknodes    ("Ulli", "Nora", "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha", "Elise", "Jack");
    mknodes    ("Emerald woods", "Madix", "Skoda", "Trombone", "Lecho", "SilentOS", "Insurance");
    mkproperties("person", "male", "female", "driver", "consumer", "contractor");
    mkreasons   ("wrote", "read", "bought", "left", "foundet", "loves", "sells", "works at", "uses", "plays");
    mkatoms     ( 2.5, "done", 7, std::array{2,1,3}, "go", 89, "sold", "percent");
    // ================================================================================================

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
    lt2t("Trombone",      "plays",    "Jack",   "Peter"                                              );
    // ================================================================================================


    // give all 'nodes' the property 'person'
    for ( size_t n = 0; n < poOdb->Nodes().size(); ++n )
        {
        poOdb->AppendProperty2Node( 0, n );
        }

    // assign 'properties' to groups of 'nodes' (supported by fold expressions)
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

    auto px = poOdb->MakeNode("Ulli");
    poOdb->AppendProperty2Node( 0, px->m_nId );
    poOdb->AppendProperty2Node( 6, px->m_nId );
    poOdb->AppendProperty2Node( 7, px->m_nId );

    } // void FillInSomeData()


#include <asio/ip/tcp.hpp>
#include <asio/io_service.hpp>

using asio::ip::tcp;




template<typename T>
size_t CollectDataForTemplate( T const & roContainer, TMapS2M & roData, std::string const & crsName)
    {
    TSubMap oSubM{};
    for ( auto const & a:roContainer )
	{
	if constexpr ( std::is_same<T, odb::MLinkets>() )
	    {
	    oSubM. emplace("",         a.first->m_sName);
	    oSubM. emplace("id",       std::to_string(a.first->m_nId));
	    oSubM. emplace("type",     a.first->m_sType.substr(6));
	    oSubM. emplace("rsn",      a.second->m_sName);
	    oSubM. emplace("rsn-id",   std::to_string(a.second->m_nId));
	    oSubM. emplace("rsn-type", a.second->m_sType.substr(6));
	    }
	else if constexpr ( std::is_same<T, odb::MLinks>() )
	    {
	    oSubM. emplace("",         a.first->m_sName);
	    oSubM. emplace("id",       std::to_string(a.first->m_nId));
	    oSubM. emplace("type",     a.first->m_sType.substr(6));
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
    return roContainer.size();
    }

template<typename T>
size_t SortDataForTemplate( T const & roContainer, TMapS2M & roData, std::string const & crsName )
    {
    if ( 1 == CollectDataForTemplate( roContainer, roData, crsName) )
	{
	for ( auto const & a:roContainer )
	    {
	    if constexpr ( std::is_same<T, odb::SNodes>() || std::is_same<T, odb::CNodes>())
		{
		g_sTemplate = "node.html";
		CollectDataForTemplate( a->GetLinkets(),    roData, "link-to");
		CollectDataForTemplate( a->GetNodes(),      roData, "link-from");
		CollectDataForTemplate( a->GetProperties(), roData, "Property");
		CollectDataForTemplate( a->GetAtoms(),      roData, "Atom");
		}
	    if constexpr ( std::is_same<T, odb::SProperties>() || std::is_same<T, odb::CProperties>() )
		{
		g_sTemplate = "property.html";
		CollectDataForTemplate( a->Relations(),     roData, "Node");
		}
	    if constexpr ( std::is_same<T, odb::SReasons>() || std::is_same<T, odb::CReasons>())
		{
		g_sTemplate = "reason.html";
   		CollectDataForTemplate( a->Relations(),     roData, "Node");
		}
	    }
	}
    return roContainer.size();
    }

void SendError( long nError, std::ostream & ros )
    {
    TMapS2M oErrorData{ {"title",   { {"", "odb Interactor"} } },
		        {"message", { {"", "ERROR 404"}      } },
		        {"text",    { {"", "Not found"}      } } };

    Cte const ote(oErrorData, "error.html", g_sTemplatePath);
    ros << "HTTP/1.1 404 Not found" << '\n';
    ros << "Server: odb/0.9.0 (Linux) CPP" << '\n';
    ros << "Content-Length: " << ote.length() << '\n';
    ros << "Content-Language: en" << '\n';
    ros << "Connection: close" << '\n';
    ros << "Content-Type: text/html" << '\n';
    ros << '\n';
    ros << ote;
    }

void SendResultPage( Cte const & ote, std::ostream & ros )
    {
//  std::cerr << "======================================" << '\n' << ote << '\n';
    ros << "HTTP/1.1 200 OK" << '\n';
    ros << "Server: odb/0.9.0 (Linux) C++" << '\n';
    ros << "Content-Length: " << ote.length() << '\n';
    ros << "Content-Language: en" << '\n';
    ros << "Connection: close" << '\n';
    ros << "Content-Type: text/html" << '\n';
    ros << '\n';
    ros << ote;
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
        case 't': b = poOdb->LinkNode2Node(nVal1, nVal2, nVal3);
                  break;
        case 'T': b = poOdb->LinkNode2Node(sVal1, sVal2, sVal3);
                  break;
        case 'p': b = poOdb->AppendProperty2Node(nVal1, nVal2);
                  break;
        case 'P': b = poOdb->AppendProperty2Node(sVal1, false, sVal2);
                  break;
        case 'a': b = poOdb->AppendAtom2Node(nVal2, nVal1);
                  break;
        case 'A': ros << " \n! Not implemented\n";
                  break;
        }
    if (b)
        ros << " \n! Linked\n";
    else
        ros << " \nE could not assign objects\n";
    return b;
    }

bool Insert(std::string const & crsQuery, std::ostream & ros)
    {
    ros << "+ " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char c = crsQuery[0];
    std::string sInput = crsQuery.substr(2);

    odb::PNode     t;
    odb::PReason   r;
    odb::PProperty p;
    odb::PAtom     a;

    switch (c)
        {
        case 't': t = poOdb->MakeNode(sInput);     ros << " \n" << ":" << t->m_nId << ":" << *t << " \n"; break;
        case 'r': r = poOdb->MakeReason(sInput);   ros << " \n" << ":" << r->m_nId << ":" << *r << " \n"; break;
        case 'p': p = poOdb->MakeProperty(sInput); ros << " \n" << ":" << p->m_nId << ":" << *p << " \n"; break;
        case 'a': a = poOdb->MakeAtom(sInput);     ros << " \n" << ":" << a->m_nId << ":" << *a << " \n"; break;

        default : ros << ": no insert";
        }

    return true;
    }

template<typename T>
void SendResult(T const & croData, std::iostream & ros, char const ccSwitch, std::string const & crsBadQuery = ""s)
    {
    if (crsBadQuery > ""s)
	{
	ros << " \nE: '" << crsBadQuery << "' invalid expression\n";
	return;
	}
    if (ccSwitch == 'j') 
        {
        ros << " \n { \n";
        poOdb->print_json(croData, ros);
        ros << " \n } \n";
        return;
        }

    if (ccSwitch == 'c') 
        {
        ros << " \n";
        }
    else
        {
	TMapS2M o;
	std::string sPrimKey = "Node";
        g_sTemplate = "nodes.html";

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
/*
        if constexpr ( std::is_same<T, odb::MLinkets>() )
	    {
            sPrimKey = "Node"; g_sTemplate = "nodes.html";
	    }
	if constexpr ( std::is_same<T, odb::MLinks>() )
	    {
	    sPrimKey = "Reason"; g_sTemplate = "reasons.html";
	    }
*/
        SortDataForTemplate( croData, o, sPrimKey );
        std::cout << g_sTemplate << '\n';
        Cte ote(o, g_sTemplate, g_sTemplatePath);
        SendResultPage( ote, ros );
//        SendResultPage( ote, std::cout );

                    for (auto & a:o)
                	{
                	std::cout << a.first << ", [";
                	for (auto & b:a.second)
                	    {
                	    std::cout << "(" << b.first << ", " << b.second << ") ";
                	    }
                	std::cout << "]\n";
                	}


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

bool Answer(std::string const & crsQuery, tcp::iostream & ros)
    {
//    ros << "? " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char c = crsQuery[0];
    char d = crsQuery[1];
    std::string sInput = crsQuery.substr(2);

    odb::CNodes      ts;
    odb::CReasons    rs;
    odb::CProperties ps;
    odb::CAtoms      as;
    auto             b = false;
    long nInput        = 1;
    if ( d=='i' )
	{
	try { nInput = stol(sInput); } catch(...) {  }
	}
    switch (c)
        {
        case 't':
        case 'n': if ( (d=='p') || (d=='P') )
                    {
                    ts = poOdb->FindNodesByProperty(sInput);
                    if (ts.size() == 0) { try { ts = poOdb->FindNodesByProperty(std::regex(sInput)); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
                    d = (d=='p') ? ':' : '.';
                    }
                  else if ( (d=='i') )
                    {
                    auto oop = poOdb->FindNode(nInput);
                    if ( oop.has_value() )
                	{
                	ts.insert( oop.value() );
                	}
                    }
                  else
		    {
		    ts = poOdb->FindNodes(sInput);
		    if (ts.size() == 0)  { try { ts = poOdb->FindNodes(std::regex( sInput )); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
		    }
                  SendResult(ts, ros, d, (b)?sInput:""s);
                  break;
        case 'r': if ( (d=='i') )
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
        case 'p': if ( (d=='i') )
                    {
                    auto oop = poOdb->FindProperty(nInput);
                    if ( oop.has_value() )
                	{
                	ps.insert( oop.value() );
                	}
                    }
                  else
		    {
		    ps = poOdb->FindProperties(std::string( sInput ));
                    if (ps.size() == 0) { try { ps = poOdb->FindProperties(std::regex( sInput )); } catch(...) { b=true; std::cerr << "E: '" << sInput << "' invalid expression\n"; } }
                    }
                  SendResult(ps, ros, d, (b)?sInput:""s);
                  break;
        case 'a': if ( (d=='i') )
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

bool FindUnuseds(std::string const & crsQuery, tcp::iostream & ros)
    {
    ros << "? " + crsQuery;

    if ( crsQuery.length() < 2 ) return false;

    char c = crsQuery[0];
    char d = crsQuery[1];
    std::string sInput = crsQuery.substr(2);

    odb::CNodes     ts;
    odb::CReasons    rs;
    odb::CProperties ps;
    odb::CAtoms      as;
    switch (d)
        {
        case 't': ts = poOdb->FindUnUsedNodes();      SendResult(ts, ros, d); break;
        case 'r': rs = poOdb->FindUnUsedReasons();    SendResult(rs, ros, d); break;
        case 'p': ps = poOdb->FindUnUsedProperties(); SendResult(ps, ros, d); break;
        case 'a': as = poOdb->FindUnUsedAtoms();      SendResult(as, ros, d); break;
        default : ros << ": no result";
                  return false;
        }

    return true;
    } //

void SendStatistics(std::ostream & ros)
    {
    ros << "---------------- " <<  poOdb->Nodes().size()      << " nodes" << " \n";
    ros << "---------------- " <<  poOdb->Properties().size() << " properties" << " \n";
    ros << "---------------- " <<  poOdb->Reasons().size()    << " reasons" << " \n";
    ros << "---------------- " <<  poOdb->Atoms().size()      << " atoms" << " \n";
    }



int main(int argc, char* argv[])
    {
    if (argc != 2)
        {
        std::cout << "Usage: " << argv[0] << " <ip> <port>\n";
        std::cout << "Example:\n";
        std::cout << "  " << argv[0] << " 1025 &\n";
        return 1;
        }

    auto const sFilename = "wwwdb.json";

    poOdb->LoadDB(sFilename);

    std::cout << '\n';
    SendStatistics(std::cout);

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

            if (sQuery.substr(0, 6) != R"(GET /?)" )
        	{
                std::cout << "Qi: |" << sQuery << "|\n";
        	if (sQuery.substr(0, 4) != R"(GET )" )
		    {
		    SendError( 404, stream );
		    }
        	continue;
        	}

	    std::string sQI{sQuery};
	    std::smatch      sm{};
	    std::regex const re(R"(/?(..)=([^ ]+))");
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
            std::cout << "Qo: |" << sQuery << "|\n";

            TMapS2M oHead{
			   {"title",   { {"", "odb Interactor"} } },
			   {"message", { {"", "Welcome"}        } } };
            TMapS2M o = oHead;
            TSubMap u;


/*
            for (auto & a:o)
        	{
        	std::cout << a.first << ", [";
        	for (auto & b:a.second)
        	    {
        	    std::cout << "(" << b.first << ", " << b.second << ") ";
        	    }
        	std::cout << "]\n";
        	}
*/
            if ( sQuery == "stat" )
                {
                SendStatistics(stream);
                SendStatistics(std::cout);
                }
            else if ( sQuery == "clean" )
                {
                stream << "Emptying DB, old statistics:\n";
                SendStatistics(stream);
                stream << "Cleaning, please wait\n";
                poOdb = std::make_unique<odb::COdb>();
                SendStatistics(stream);
                SendStatistics(std::cout);
                stream << "Done\n";
                }
            else if ( sQuery == "fillr" )
                {
                stream << "Fill DB with small set of data, old statistics:\n";
                SendStatistics(stream);
                stream << "Cleaning, please wait\n";
                poOdb = std::make_unique<odb::COdb>();
                stream << "Filling, please wait\n";
                FillInSomeData();
                SendStatistics(stream);
                SendStatistics(std::cout);
                stream << "Done\n";
                }
            else if ( sQuery == "load" )
                {
                stream << "Replacing DB, old statistics:\n";
                SendStatistics(stream);
                poOdb = std::make_unique<odb::COdb>();
                stream << "Loading, please wait\n";
                poOdb->LoadDB(sFilename);
                SendStatistics(stream);
                SendStatistics(std::cout);
                stream << "Done\n";
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
                stream << "help    - shows this help page\n";
                stream << "stat    - shows db statistics\n";
                stream << "clean   . deletes all entries from DB\n";
                stream << "fillr   . fills the DB with test data\n";
                stream << "load    . loads the DB from disk\n";
                stream << "save    . saves the DB to disk\n";
                stream << " \n";
                stream << "t:regex - search for a node\n";
                stream << "p:regex - search for a property\n";
                stream << "r:regex - search for a reason\n";
                stream << "a:regex - search for an atom\n";
                stream << "tpregex - search for a node having a specific property\n";
                stream << " \n";
                stream << "t.regex - search for a node (short result)\n";
                stream << "p.regex - search for a property (short result)\n";
                stream << "r.regex - search for a reason (short result)\n";
                stream << "a.regex - search for an atom (short result)\n";
                stream << "tcregex - search for nodes, returns only the result count\n";
                stream << "tjregex - search for nodes, returns result in JSON format\n";
                stream << "tPregex - search for a node having a specific property (short result)\n";
                stream << " \n";
                stream << "Example\n";
                stream << " \n";
                stream << "t:Star (Trek|Wars).*\n";
                stream << "t.Star (Trek|Wars).*\n";
                stream << "tcStar (Trek|Wars).*\n";
                stream << "tjStar (Trek|Wars).*\n";
                stream << " \n";
                stream << "Searches for all 'nodes' named \"Star Trek\" or \"Star Wars\"\n";
                stream << " \n";
                stream << "t+name  - insert a node\n";
                stream << "p+name  - insert a property\n";
                stream << "r+name  - insert a reason\n";
                stream << "a+name  - insert an atom\n";
                stream << " \n";
                stream << "ltidT:idR:idT        - links two Nodes by ID\n";
                stream << "lTnameT:nameR:nameT  - links two Nodes by Name\n";
                stream << "lpidP:idT            - links Property to Node\n";
                stream << "lPnameP:nameT        - links Property to Node\n";
                stream << "laidA:idT            - links Atom to Node\n";
                stream << "lAnameA:nameT        - links Atom to Node\n";
                stream << " \n";
                stream << "et  - lists unused Nodes\n";
                stream << "ep  - lists unused Properties\n";
                stream << "er  - lists unused Reasons\n";
                stream << "ea  - lists unused Atoms\n";
                stream << "ee  - lists all unused elements\n";
                }
            else
                {
                if ( sQuery.length() < 2 )
                        {
                        stream << "try: 'help' to get help\n";
                        }
                else if ( (sQuery[1] == ':') || (sQuery[1] == '.') || (sQuery[1] == 'c') || (sQuery[1] == 'i') || (sQuery[1] == 'j') || (sQuery.substr(0,2) == "tP") || (sQuery.substr(0,2) == "tp") )
                        {
                        if ( not Answer(sQuery, stream) ) stream << ": no result\n";
                        }
                else if ( sQuery[1] == '+' )
                        {
                        if ( not Insert(sQuery, stream) ) stream << ": not inserted\n";
                        }
                else if ( sQuery[0] == 'l' )
                        {
                        if ( not LinkNAppend(sQuery, stream) ) stream << ": not bound\n";
                        }
                else if ( sQuery[0] == 'e' )
                        {
                        if ( not FindUnuseds(sQuery, stream) ) stream << ": not bound\n";
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

