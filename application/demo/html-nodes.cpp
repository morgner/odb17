/**
    @file html-nodes.cpp
 
    @author Manfred Morgner
    @date 15.04.2018

    Demonstration of how Nodes are made and how they can be connected to
    each other using specific unidirectional reasons.
 */

#include <iostream>
#include <sstream>

#include "odb.h"


/*
#include "../../inja/test/thirdparty/nlohmann/json.hpp"
#include "../../inja/src/inja.hpp"
*/

auto oOdb = odb::COdb();


// Demo main program
int main()
    {
    // 3 people
    oOdb.MakeNode("Udo");
    oOdb.MakeNode("Ina");
    oOdb.MakeNode("Rob");

    // 3 kind of relation
    oOdb.MakeReason("is father of");
    oOdb.MakeReason("knows");
    oOdb.MakeReason("loves");
    
    // 3 bindings
    oOdb.LinkNode2Node("Udo", "is father of", "Ina");
    oOdb.LinkNode2Node("Udo", "knows"       , "Rob");
    oOdb.LinkNode2Node("Ina", "loves"       , "Rob");

    // show us
/*
    std::stringstream ss;
    std::cout << "---------------- all nodes" << '\n';
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

