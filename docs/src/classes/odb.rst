COdb
====

The OjectDatabase is a Graph-Database.

It aims to build a natural image of the relevant links between CThings (=nodes)
amongst each other as well as things and atoms (=data fields).

CThings are objects or nodes, like cars in cars or trees or people.

CThing's may be linked amongst each other for specified CReasons
CReason's are unidirectional linking CThing's
CProperty is a primitive property of a CThing
CAtom's are attributes of CThings containing descriptive data

 - A thing may be a tree
 - Attributes of a tree may be, position, health status, animals living in it
 - A tree may relate to other trees by taking the light, building a forrest
 - ...

Sample code

.. code-block:: cpp
 
    #include <iostream>

    #include "odb.h"

    int main()
       {
       auto oOdb = odb::COdb();
       oOdb.print_json( std::cout );
       std::cout << '\n';
       }

.. doxygenclass:: odb::COdb
