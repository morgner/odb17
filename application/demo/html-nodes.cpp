/**
    @file html-nodes.cpp
 
    @author Manfred Morgner
    @date 15.04.2018

    Demonstration of how Nodes are made and how they can be connected to
    each other using specific unidirectional reasons.
 */

#include <iostream>

#include "odb.h"
#include "../../../te/include/te.h"


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
    oOdb.print_json(ss);
    std::cout << ss.str() << '\n';
*/
    TRenderData o{ 
               {"title",               { {"", "odb Interactor"}    } },
               {"message",             { {"", "WELCOME"}           } },
               {"former-query",        { {"", "t:Star Trek"}       } },
               {"static+favicon-icon", { {"", "/static/fav.icon"}  } },
               {"static+style-css",    { {"", "/static/style.css"} } },
             };

    TRenderItem u;

    odb::CNodes mn = oOdb.FindNodes(std::regex(".*"));
    std::string sTemplate = "nodes.html";
    for ( auto const & a:mn )
	{
        u.emplace("", a->m_sName);
	u.emplace("id", std::to_string(a->m_nId));
        u.emplace("type", a->m_sType);
	o.emplace("node", u );
	u.clear();
	}

/*
	odb::ONode a = oOdb.FindNode(0);
	std::string sTemplate = "node.html";
	u.emplace("", (*a)->m_sName);
	u.emplace("id", std::to_string((*a)->m_nId));
	u.emplace("type", (*a)->m_sType);
	o.emplace("node", u );
	u.clear();
*/
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

    Cte const ote(o, sTemplate, "../templates/");

    std::cout << ote;
    }

