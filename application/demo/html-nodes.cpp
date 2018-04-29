/**
    @file html-nodes.cpp
 
    @author Manfred Morgner
    @date 15.04.2018

    Demonstration of how Things are made and how they can be connected to
    each other using specific unidirectional reasons.
 */

#include <iostream>
#include <sstream>

#include "generic.h"
#include "odb.h"
#include "atom.h"
#include "node.h"

/*
#include "../../inja/test/thirdparty/nlohmann/json.hpp"
#include "../../inja/src/inja.hpp"
*/

auto oOdb = odb::COdb();


// Demo main program
int main()
    {
    // 3 people
    oOdb.MakeThing("Udo");
    oOdb.MakeThing("Ina");
    oOdb.MakeThing("Rob");

    // 3 kind of relation
    oOdb.MakeReason("is father of");
    oOdb.MakeReason("knows");
    oOdb.MakeReason("loves");
    
    // 3 bindings
    oOdb.LinkThing2Thing("Udo", "is father of", "Ina");
    oOdb.LinkThing2Thing("Udo", "knows"       , "Rob");
    oOdb.LinkThing2Thing("Ina", "loves"       , "Rob");

    // show us
/*
    std::stringstream ss;
    std::cout << "---------------- all things" << '\n';
    std::cout << "{" << '\n';
    oOdb.print_json(ss);
    std::cout << ss.str() << '\n';
    std::cout << "}" << '\n';
*/
    
/*
    using json = nlohmann::json;


    inja::Environment env = inja::Environment();
    json data;
    data["data"] = "Jeff";
    data["deps"] = {"Ab","Zu"};

    std::string s = env.render_file("../templates/data.html", data);

    std::cout << s <<'\n';    
*/
    }

