/**
 *  @file link2things-test.cpp
 *
 *  @author Manfred Morgner
 *  @date 21.01.2018
 */

#include <iostream>

#include "odb.h"
#include "thing.h"

/// @brief Demo main program "linking things together"
int main()
    {
    // creating things
    auto oOdb    = odb::COdb();
    auto pThing1 = oOdb.MakeThing("Ulrich");
    auto pThing2 = oOdb.MakeThing("Fred");
    auto pReason = oOdb.MakeReason("pays");

    // linking them together
    pThing1->Link(pThing2, pReason);

    // 1) print the thing's view
    std::cout << "thing: " << *pThing1 << '\n';
    std::cout << "thing: " << *pThing2 << '\n';

    // 2) print complete database in simple form
    oOdb.print();

    // 3) print complete database in json format
    oOdb.print_json(std::cout);
    }
