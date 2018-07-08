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


using namespace std::string_literals;

odb::COdb oOdb; // = std::make_unique<odb::COdb>();
odb::COdb oOdbR;// = std::make_unique<odb::COdb>();

template<typename ...Args> void mknodes     (odb::COdb & oOdb, Args&&... args) { (oOdb.MakeNode    (args), ...); }
template<typename... Args> void mkproperties(odb::COdb & oOdb, Args&&... args) { (oOdb.MakeProperty(args), ...); }
template<typename... Args> void mkreasons   (odb::COdb & oOdb, Args&&... args) { (oOdb.MakeReason  (args), ...); }
template<typename... Args> void mkatoms     (odb::COdb & oOdb, Args&&... args) { (oOdb.MakeAtom    (args, "fold" ), ...); }

// append a property to a group of nodes, if property does not exists and
// 'cbForce' is 'true', the property will be added to the DB
template<typename... Args>
void ap2ts(std::string const & crsProperty, // name of the property
                  bool const   cbForce,     // create it if not existent?
                     Args&&... args)        // pack of names of 'nodes'
    {
    (oOdb.AppendProperty2Node(crsProperty, cbForce, args), ...);
    }
        
template<typename... Args>
void lt2t(std::string const & crsNameTo, // name of the property
          std::string const & crsReason, // create it if not existent?
                    Args&&... args)      // pack of names of 'nodes'
    {
    (oOdb.LinkNode2Node(args, crsReason, crsNameTo), ...);
    }

void FillInSomeData(odb::COdb & oOdb)
    {
    // filling in some data
    // ================================================================================================
    mknodes     (oOdb, "Ulli", "Nora", "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha", "Elise", "Jack");
    mknodes     (oOdb, "Emerald woods", "Madix", "Skoda", "Trombone", "Lecho", "SilentOS", "Insurance");
    mkproperties(oOdb, "person", "male", "female", "driver", "consumer", "contractor");
    mkreasons   (oOdb, "wrote", "read", "bought", "left", "foundet", "loves", "sells", "works at", "uses", "plays");
    mkatoms     (oOdb,  2.5, "done", 7, std::array{2,1,3}, "go", 89, "sold", "percent");
    // ================================================================================================

    // link nodes to nodes
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

    // assign 'properties' to groups of 'nodes' (supported by fold expressions)
    // ========================================================================================================
    ap2ts( "person",       false, "Ulli", "Nora", "Peter", "Paula", "Rudi", "Marta", "Arnold", "Bertha", "Elise");
    ap2ts( "male",         false, "Ulli",         "Peter",          "Rudi",          "Arnold"                   );
    ap2ts( "female",       false,         "Nora",          "Paula",         "Marta",           "Bertha", "Elise");
    ap2ts( "driver",       false, "Ulli",                  "Paula", "Rudi", "Marta",           "Bertha"         );
    ap2ts( "consumer",     false, "Ulli", "Nora", "Peter",                                               "Elise");
    ap2ts( "contractor",   false,                 "Peter", "Paula", "Rudi",                              "Elise");
    ap2ts( "artist",       true,  "Ulli",                                            "Arnold", "Bertha", "Elise");
    ap2ts( "builder",      true,                  "Peter", "Paula",                  "Arnold"                   );
    ap2ts( "book",         true,  "Emerald woods"                                                               );
    ap2ts( "movie",        true,  "Emerald woods"                                                               );
    ap2ts( "music",        true,  "Trombone"                                                               );
    ap2ts( "company",      true,  "Insurance"                                                               );
    ap2ts( "manufacturer", true,  "Skoda"                                                               );
    ap2ts( "OS",           true,  "SilentOS"                                                               );
    // ========================================================================================================

    auto px = oOdb.MakeNode("Willi");
    oOdb.AppendProperty2Node( 0, px->m_nId );
    oOdb.AppendProperty2Node( 6, px->m_nId );
    oOdb.AppendProperty2Node( 7, px->m_nId );

    } // void FillInSomeData()



bool FindUnuseds(std::ostream & ros)
    {
    oOdb.print_json(oOdb.FindUnUsedNodes(), ros);
    oOdb.print_json(oOdb.FindUnUsedReasons(), ros);
    oOdb.print_json(oOdb.FindUnUsedProperties(), ros);
    oOdb.print_json(oOdb.FindUnUsedAtoms(), ros);

    return true;
    } //

void PrintStatistics(odb::COdb const & croOdb, std::ostream & ros = std::cout)
    {
    ros << "---------------- " <<  croOdb.Nodes().size()      << " nodes" << " \n";
    ros << "---------------- " <<  croOdb.Properties().size() << " properties" << " \n";
    ros << "---------------- " <<  croOdb.Reasons().size()    << " reasons" << " \n";
    ros << "---------------- " <<  croOdb.Atoms().size()      << " atoms" << " \n";
    ros << " \n";
    }

int main()
    {
    std::cout << "(1) odb-1 initial";
	{
	PrintStatistics(oOdb);
	FillInSomeData(oOdb);
	std::cout << "(2) odb-1 filled";
	PrintStatistics(oOdb);
	oOdb.SaveDB("test_out1.json");
	}

    std::cout << "(3) odb-2 initial";
	{
	PrintStatistics(oOdb);
	oOdbR.LoadDB("test_out1.json");
	std::cout << "(4) odb-2 loadet";
	PrintStatistics(oOdb);
	oOdbR.SaveDB("test_out2.json");
	}

    return 0;
    }

