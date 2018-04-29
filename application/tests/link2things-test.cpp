/**
 *  @file link2nodes-test.cpp
 *
 *  @author Manfred Morgner
 *  @date 21.01.2018
 */

#include <iostream>

#include "odb.h"


/// @brief Demo main program "linking nodes together"
int main()
    {
    // creating nodes
    auto oOdb    = odb::COdb();
    auto pNode1 = oOdb.MakeNode("Ulrich");
    auto pNode2 = oOdb.MakeNode("Fred");
    auto pReason = oOdb.MakeReason("pays");

    // linking them together
    pNode1->Link(pNode2, pReason);

    // 1) print the node's view
    std::cout << "node: " << *pNode1 << '\n';
    std::cout << "node: " << *pNode2 << '\n';

    // 2) print complete database in simple form
    oOdb.print();

    // 3) print complete database in json format
    oOdb.print_json(std::cout);
    }
