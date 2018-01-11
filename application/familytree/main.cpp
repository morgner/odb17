/*  main.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <iostream> // cout

#include "thing.h"
#include "reason.h"
#include "atom.h"
#include "odb.h"
#include "generic.h"


/**
 * Helper function to print a vector of objects
 */

/*
template<typename T>
auto print(std::deque<T> & deq, std::ostream & ros)
    {
    for (auto const & e : deq)
        {
        return ros << e->type << " id: " << e->id << " (u:" << e.use_count() << ')' << ' ' << *e << '\n';
        }
    }
*/

odb::COdb g_oOdb{};

using namespace std::string_literals;

/**
 * Demo main program
 */
int main()
    {
    odb::CAtom::s_bDebug = false;

    std::string const   cs("constness"s);
    std::string         ns("non-constness"s);
    int                 a[10] = {1, 2, 3, 4, 5, 6};
    std::vector<int>    v{1, 2, 3};
    std::array<char, 7> w{'a','b','c'};


    auto d1 = g_oOdb.MakeThing("Wundertüte"s);
    auto d2 = g_oOdb.MakeThing();
    auto d3 = g_oOdb.MakeThing("No Product"s);
    auto d4 = g_oOdb.MakeThing("Thoughtless bucket"s);
    auto d5 = g_oOdb.MakeThing("Terry Pratchett"s);
    auto d6 = g_oOdb.MakeThing("'Good Omens'"s);
    auto d7 = g_oOdb.MakeThing("Neil Gaiman"s);

    auto r1 = g_oOdb.MakeReason("made"s);
    auto r2 = g_oOdb.MakeReason("delivers"s);
    auto r3 = g_oOdb.MakeReason("owns"s);
    auto r4 = g_oOdb.MakeReason("wrote"s);

    auto made     = g_oOdb.MakeReason("made"s);
    auto delivers = g_oOdb.MakeReason("delivers"s);
    auto owns     = g_oOdb.MakeReason("owns"s);
    auto wrote    = g_oOdb.MakeReason("wrote"s);

//  Link(terry, wrote, good_omnes);

    d1->Link(d3, r2);
    d1->Link(d3, r3);
    d4->Link(d1, r1);
    d5->Link(d6, r4);
    d7->Link(d6, r4);

    auto a1 = d3->Append( g_oOdb.MakeAtom(100.2, "round", "", "%") );
    auto a2 = d3->Append( g_oOdb.MakeAtom(0.9, "size", "V", "dl") );
    auto a3 = d3->Append( g_oOdb.MakeAtom(1.2) );
    auto a4 = d3->Append( g_oOdb.MakeAtom(std::vector<int>{3, 2, 1}, "vector"));

    auto a5 = g_oOdb.MakeAtom(std::array <int, 3>{3, 2, 1}, "array");
    auto a6 = g_oOdb.MakeAtom(cs, "const string"s);
    auto a7 = g_oOdb.MakeAtom(ns, "non-const string");
    auto a8 = d4->Append( g_oOdb.MakeAtom(v, "vector of 3 int") );
    auto a9 = d4->Append( g_oOdb.MakeAtom(w, "array of 3 char") );

    auto a10 = g_oOdb.MakeAtom(3, "Three");
    auto a11 = g_oOdb.MakeAtom(2, "Two");
    auto a12 = g_oOdb.MakeAtom(1, "One");

    auto a13 = g_oOdb.MakeAtom(std::string(8, '-'), "Line"s);

    auto a14 = d1->Append( g_oOdb.MakeAtom('H', "Letter"s) );
    auto a15 = d1->Append( g_oOdb.MakeAtom('e', "Letter"s) );
    auto a16 = d1->Append( g_oOdb.MakeAtom('l', "Letter"s) );
    auto a17 = d1->Append( g_oOdb.MakeAtom('o', "Letter"s) );

    auto a18 = d2->Append( g_oOdb.MakeAtom("Quirks & Quarks", "Radio Cast"s) );
    auto a19 = d2->Append( g_oOdb.MakeAtom("Thinking allowed"s, "Podcast"s) );

    g_oOdb.MakeAtom("---------"s, "Line"s);

/*
    print(atoms);
    print(things);
    print(reasons);
*/

    if ( !odb::CAtom::s_bDebug )
        {
        std::cout << "---------------- atom" << '\n';
        std::cout << *a1 << '\n';
        std::cout << "---------------- thing" << '\n';
        std::cout << *d1 << '\n';
        std::cout << "----------------reason" << '\n';
        std::cout << *r1 << '\n';
        std::cout << "---------------- odb plain" << '\n';
        g_oOdb.print();
        std::cout << "---------------- odb JSON" << '\n';
        g_oOdb.print_json(std::cout);
        }
  }
