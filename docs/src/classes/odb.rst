COdb
====

The OjectDatabase is a database which aims to build a natural image of the
relevant links between things amongst each other as well as things and atoms.

CThings are objects which will dominate the model to build, like cars in a 
car database or trees in a database about forrests

CAtoms are attributes of CThings and
CThings may link amongst each other for specified CReasons
CProperty is a primitive property of a thing

 - A thing may be a tree
 - Attributes of a tree may be, position, health status, animals living in it
 - A tree may relate to other trees by taking the light off, building a forrest
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
