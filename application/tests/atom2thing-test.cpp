/**
    @file atom2node-test.cpp
 
    @author Manfred Morgner
    @date 21.01.2018
 */

#include <iostream>

#include "odb.h"
#include "atom.h"
#include "node.h"

/**
    @brief Demo main program
 */
int main()
    {
    auto oOdb    = odb::COdb();
    auto pNode1 = oOdb.MakeNode("Ulrich");
    auto pNode2 = oOdb.MakeNode("Fred");
    auto pAtom1  = oOdb.MakeAtom("Leader", "Role");
    auto pAtom2  = oOdb.MakeAtom("Member", "Role");
    auto pReason = oOdb.MakeReason("pays");
    pNode1->Append(pAtom1);
    pNode2->Append(pAtom2);
    pNode1->Link(pNode2, pReason);
    std::cout << "node: " << *pNode1 << '\n';
    std::cout << "node: " << *pNode2 << '\n';
    oOdb.print();
    oOdb.print_json(std::cout);
    oOdb.print_json_stream(std::cout);
    }
