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
a */
int main()
    {
/*
    auto d1 = g_oOdb.MakeThing("Terry Pratchett"s);
    auto d2 = g_oOdb.MakeThing("Neil Gaiman"s);

    auto d3 = g_oOdb.MakeThing("'Good Omens'"s);
    auto d4 = g_oOdb.MakeThing("'The Ligth Fantastic'"s);
    auto d5 = g_oOdb.MakeThing("'The Thief of Time'"s);

    auto r1 = g_oOdb.MakeReason("wrote"s);
    auto r2 = g_oOdb.MakeReason("read"s);

    Link(d1, r1, d3);
    Link(d1, r1, d4);
    Link(d1, r1, d5);
    Link(d2, r1, d3);

    std::cout << "d1 " << d1.use_count() << " d2 " << d2.use_count() << " d3 " << d3.use_count() << " d4 " << d4.use_count() << " d5 " << d5.use_count() << '\n';
    std::cout << "r1 " << r1.use_count() << " r2 " << r2.use_count() << '\n';
    std::cout << "d1 " << *d1 << '\n';

    d1->Link(d3, r1);
    d1->Link(d4, r1);
    d1->Link(d5, r1);
    d2->Link(d3, r1);

    Link(d1, r1, d3);
    Link(d1, r1, d4);
    Link(d1, r1, d5);
    Link(d2, r1, d3);

    std::cout << "d1 " << d1.use_count() << " d2 " << d2.use_count() << " d3 " << d3.use_count() << " d4 " << d4.use_count() << " d5 " << d5.use_count() << '\n';
    std::cout << "r1 " << r1.use_count() << " r2 " << r2.use_count() << '\n';
    std::cout << "d1 " << *d1 << '\n';

    auto a1 { g_oOdb.MakeAtom(100.2, "round", "", "%") };
    auto a2 = g_oOdb.MakeAtom(0.9, "size", "V", "dl");

    std::cout << "d1 " << *d1 << '\n';
    d1->Append( a1 );
    std::cout << "d1 " << *d1 << '\n';
    d1->Append( a2 );
    std::cout << "d1 " << *d1 << '\n';
    d1->Append( a1 );
    std::cout << "d1 " << *d1 << '\n';
*/

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
    auto d6 = g_oOdb.MakeThing("Neil Gaiman"s);
    auto d7 = g_oOdb.MakeThing("'Good Omens'"s);
    auto d8 = g_oOdb.MakeThing("'The Colour of Magic'"s);
    auto d9 = g_oOdb.MakeThing("'The Ligth Fantastic'"s);
    auto da = g_oOdb.MakeThing("'The Thief of Time'"s);

    auto r1 = g_oOdb.MakeReason("made"s);
    auto r2 = g_oOdb.MakeReason("delivers"s);
    auto r3 = g_oOdb.MakeReason("owns"s);
    auto r4 = g_oOdb.MakeReason("wrote"s);
    auto r5 = g_oOdb.MakeReason("read"s);

    d5->Link(d7, r4);
    d5->Link(d8, r4);
    d5->Link(d9, r4);
    d5->Link(da, r4);
    d5->Link(da, r4);

    d6->Link(d7, r4);
    d6->Link(d7, r4);

/*
    Link(d1, r2, d3);
    Link(d1, r3, d3);
    d4->Link(d1, r1);
    d5->Link(d6, r1);
    d5->Link(d6, r2);
    d5->Link(d6, r4);
    d5->Link(d6, r5);
    d5->Link(d8, r4);
    d5->Link(d9, r4);
    d5->Link(da, r4);
    d7->Link(d6, r4);

    d5->Unlink( d6, r4);
    d4->Unlink( d1, r1);
    d4->Unlink( d1, r1);
    d5->Unlink( d9, r1);
*/
    auto a1 = g_oOdb.MakeAtom(100.2, "round", "", "%"); d3->Append( a1 );
    auto a2 = g_oOdb.MakeAtom(0.9, "size", "V", "dl");  Append( d3, a2 );
    auto a3 = g_oOdb.MakeAtom(1.2); d3->Append( a3 );
    auto a4 = g_oOdb.MakeAtom(std::vector<int>{3, 2, 1}, "vector"); d3->Append( a4 );

    auto a5 = g_oOdb.MakeAtom(std::array <int, 3>{3, 2, 1}, "array");
    auto a6 = g_oOdb.MakeAtom(cs, "const string"s);
    auto a7 = g_oOdb.MakeAtom(ns, "non-const string"); d5->Append( a7 ); d5->Append( a7 );

    auto a8 = g_oOdb.MakeAtom(v, "vector of 3 int"); d4->Append( a8 );
    auto a9 = g_oOdb.MakeAtom(w, "array of 3 char"); d4->Append( a9 );

    auto a10 = g_oOdb.MakeAtom(3, "Three");
    auto a11 = g_oOdb.MakeAtom(2, "Two");
    auto a12 = g_oOdb.MakeAtom(1, "One");

    auto a13 = g_oOdb.MakeAtom(std::string(8, '-'), "Line"s);

    auto a14 = g_oOdb.MakeAtom('H', "Letter"s); d1->Append( a14 );
    auto a15 = g_oOdb.MakeAtom('e', "Letter"s); d1->Append( a15 );

    auto a18 = g_oOdb.MakeAtom("Quirks & Quarks", "Radio Cast"s); d2->Append( a18 );
    auto a19 = g_oOdb.MakeAtom("Thinking allowed"s,  "Podcast"s); d2->Append( a19 );

//    auto a18 = d2->Append( g_oOdb.MakeAtom("Quirks & Quarks", "Radio Cast"s) );
//    auto a19 = d2->Append( g_oOdb.MakeAtom("Thinking allowed"s, "Podcast"s) );

    g_oOdb.MakeAtom("--------"s, "Line"s);


    if ( !odb::CAtom::s_bDebug )
        {
        std::cout << "---------------- all atoms" << '\n';
        for ( auto const & a:g_oOdb.Atoms() )
            {
            std::cout << *a << '\n';
            }
        std::cout << "---------------- all things" << '\n';
        for ( auto const & a:g_oOdb.Things() )
            {
            std::cout << *a << '\n';
            }
        std::cout << "---------------- all reasons" << '\n';
        for ( auto const & a:g_oOdb.Reasons() )
            {
            std::cout << *a << '\n';
            }
        std::cout << "---------------- odb plain" << '\n';
        g_oOdb.print();
        std::cout << "---------------- odb JSON" << '\n';
        g_oOdb.print_json(std::cout);
        std::cout << "---------------- odb End" << '\n';
        } // if ( !odb::CAtom::s_bDebug )

    } // int main()
