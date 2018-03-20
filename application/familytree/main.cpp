/**
 * @file main.cpp
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include <iostream>     // cout
#include <forward_list> //
#include <list>         //
#include <set>          //

#include "thing.h"
#include "property.h"
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

void test1()
    {
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
    Link(d2, r1, d3); // double link

    std::cout << "d1 " << d1.use_count() << " d2 " << d2.use_count() << " d3 " << d3.use_count() << " d4 " << d4.use_count() << " d5 " << d5.use_count() << '\n';
    std::cout << "r1 " << r1.use_count() << " r2 " << r2.use_count() << '\n';
    std::cout << "d1 " << *d1 << '\n';

    /// double links
    d1->Link(d3, r1);
    d1->Link(d4, r1);
    d1->Link(d5, r1);
    d2->Link(d3, r1);

    /// double links
    Link(d1, r1, d3);
    Link(d1, r1, d4);
    Link(d1, r1, d5);
    Link(d2, r1, d3);

    std::cout << "d1 " << d1.use_count() << " d2 " << d2.use_count() << " d3 " << d3.use_count() << " d4 " << d4.use_count() << " d5 " << d5.use_count() << '\n';
    std::cout << "r1 " << r1.use_count() << " r2 " << r2.use_count() << '\n';
    std::cout << "d1 " << *d1 << '\n';

    auto a1 { g_oOdb.MakeAtom(100.2, "round", "", "%") };
    auto a2 = g_oOdb.MakeAtom(0.9, "size", "V", "dl");

    std::deque<uint64_t> a{33799, 53796, 33179, 34799};
    auto a3 = g_oOdb.MakeAtom(a, "Deque"s);
    std::forward_list<uint16_t> b{77, 55, 66, 44, 33};
    auto a4 = g_oOdb.MakeAtom(b, "Forward_list"s);
    std::list<std::string> c{"so", "so", "seltsamer", "sammler"};
    auto a5 = g_oOdb.MakeAtom(c, "List"s);
    std::set<char> d{'s','e','t','e','s','t'};
    auto a6 = g_oOdb.MakeAtom(d, "Set"s);
    std::multiset<char> e{'s','e','t','e','s','t'};
    auto a7 = g_oOdb.MakeAtom(e, "Multiset"s);

    std::cout << "d1 " << *d1 << '\n';
    d1->Append( a1 );
    std::cout << "d1 " << *d1 << '\n';
    d1->Append( a3 );
    std::cout << "d1 " << *d1 << '\n';
    d1->Append( a1 );
    std::cout << "d1 " << *d1 << '\n';
    }

void test2()
    {
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
    d5->Link(da, r5);

    da->Link(d3, r2);
    da->Link(d3, r3);

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

    auto a1  = g_oOdb.MakeAtom(100.2, "round", "", "%");
    auto a2  = g_oOdb.MakeAtom(0.9, "size", "V", "dl");
    auto a3  = g_oOdb.MakeAtom(1.2);
    auto a4  = g_oOdb.MakeAtom(std::vector<int>{3, 2, 1}, "vector");
    auto a5  = g_oOdb.MakeAtom(std::array <int, 3>{3, 2, 1}, "array");
    auto a6  = g_oOdb.MakeAtom(cs, "const string"s);
    auto a7  = g_oOdb.MakeAtom(ns, "non-const string");
    auto a8  = g_oOdb.MakeAtom(v, "vector of 3 int");
    auto a9  = g_oOdb.MakeAtom(w, "array of 3 char");
    auto a10 = g_oOdb.MakeAtom(3, "Three");
    auto a11 = g_oOdb.MakeAtom(2, "Two");
    auto a12 = g_oOdb.MakeAtom(1, "One");
    auto a13 = g_oOdb.MakeAtom(std::string(8, '-'), "Line"s);
    auto a14 = g_oOdb.MakeAtom('H', "Letter"s);
    auto a15 = g_oOdb.MakeAtom('e', "Letter"s);
    auto a18 = g_oOdb.MakeAtom("Quirks & Quarks", "Radio Cast"s);
    auto a19 = g_oOdb.MakeAtom("Thinking allowed"s,  "Podcast"s);
    auto a20 = g_oOdb.MakeAtom(std::vector<std::string>{"a3i"s, "b2j"s, "c1k"s}, "vos");

    d3->Append( a1 );
    d3->Append( a2 );
    d3->Append( a3 );
    d3->Append( a4 );
    d5->Append( a1 );
    d5->Append( a7 );
    d4->Append( a8 );
    d4->Append( a9 );
    d1->Append( a14 );
    d1->Append( a15 );
    d2->Append( a18 );
    d2->Append( a19 );

    auto p1  = g_oOdb.MakeProperty("Person"s);
    auto p2  = g_oOdb.MakeProperty("Writer"s);
    auto p3  = g_oOdb.MakeProperty("Book"s);
    d5->Append( p1 );
    d6->Append( p1 );
    d5->Append( p2 );
    d6->Append( p2 );
    d7->Append( p3 );
    d8->Append( p3 );
    d9->Append( p3 );
    da->Append( p3 );
    auto p4  = g_oOdb.MakeProperty("Podcast"s);
    auto p5  = g_oOdb.MakeProperty("Letter"s);
    auto p6  = g_oOdb.MakeProperty("Product"s);

    g_oOdb.MakeAtom("--------"s, "Line"s);
    }

/**
 * Demo main program
a */
int main()
    {
    test2();

//    auto a18 = d2->Append( g_oOdb.MakeAtom("Quirks & Quarks", "Radio Cast"s) );
//    auto a19 = d2->Append( g_oOdb.MakeAtom("Thinking allowed"s, "Podcast"s) );

    if ( !odb::CAtom::s_bDebug )
        {
        std::cout << "---------------- all atoms" << '\n';
        for ( auto const & a:g_oOdb.Atoms() )
            {
            std::cout << *a << '\n';
            }
        print(g_oOdb.Atoms());

        std::cout << "---------------- all properties" << '\n';
        for ( auto const & a:g_oOdb.Properties() )
            {
            std::cout << *a << '\n';
            for ( auto b:a->m_oRelations )
                std::cout << "  " << b->m_sName << '\n';
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
        print(g_oOdb.Reasons());

        std::cout << "---------------- odb plain" << '\n';
        g_oOdb.print();
        std::cout << "---------------- odb JSON" << '\n';
        g_oOdb.print_json(std::cout);
        std::cout << "---------------- odb End" << '\n';
        } // if ( !odb::CAtom::s_bDebug )

    } // int main()
