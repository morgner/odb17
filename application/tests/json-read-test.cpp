/**
    @file json-read-test.cpp
 
    @author Manfred Morgner
    @date 21.02.2018
 */

#include <iostream>
#include <fstream>

#include <json/json.h>

#include "odb.h"


odb::COdb oOdb;


int main()
    {
    oOdb.LoadDB("db.json");
    oOdb.print_json(std::cout);

    odb::CNodes p;
    {auto t1 = std::clock(); p = oOdb.FindNodesByProperty (std::regex("actor"));
                             p = oOdb.FindNodesByProperty (std::regex("actor"));
                             p = oOdb.FindNodesByProperty (std::regex("actor")); auto t2 = std::clock(); std::cout << "A: " << t2-t1 << " - " << p.size() << '\n';}

    {auto t1 = std::clock(); p = oOdb.FindNodesByProperty (std::regex("actor"));
                             p = oOdb.FindNodesByProperty (std::regex("actor"));
                             p = oOdb.FindNodesByProperty (std::regex("actor")); auto t2 = std::clock(); std::cout << "A: " << t2-t1 << " - " << p.size() << '\n';}
/*
    {auto t1 = std::clock(); p = oOdb.FindNodesByPropertyX(std::regex("actor"));
                             p = oOdb.FindNodesByPropertyX(std::regex("actor"));
                             p = oOdb.FindNodesByPropertyX(std::regex("actor")); auto t2 = std::clock(); std::cout << "B: " << t2-t1 <<  " - " << p.size() <<'\n';}
*/
    }
/*
{
    "Object Database Dump": 
        {
        "Sizes": [ {"P": 1089},{"A": 3000},{"R": 10},{"T": 1000} ]
        "Properties": [ { "id": 0, "name": "Person" } ]
        "Atoms": [ { "id": 0, "name": "round", "suffix": "%", "data": "100.2" } ]
        "Reasons": [ { "id": 0, "name": "made" } ]
        "Nodes": 
            [
                { "id": 0, "name": "Wundertüte",
                    "properties": [ {"id": 0},{"id": 1} ],
                    "atoms": [ {"id": 13},{"id": 14} ],
                    "links": [ {"node-id": 6, "reason-id": 3} ] }
            ]
        }
}
*/
