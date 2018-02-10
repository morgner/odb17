COdb
====

The OjectDatabase is a database which aims to build a natural image of the
relevant links between things amongst each other as well as things and atoms.

CTings are objects which will dominate the model to build, like cars, people,
trees, animals or such.

CAtoms are attributes of CThings and
CThing may link amongst each other for specified CReasons
CProperty is a primitive property of a thing

 - A thing may be a tree
 - Attributes of a tree may be, position, health status, animals living in it
 - A tree may relate to other trees by taking the light off, building a forrest
 - ...

or

 - A thing may be a person or a book or a car
 - Attributes of a person may be, home town, hobbies, sex, profession, skills
 - A person may relate to another person by being a partner, a child, a ...
 - A person may relate to a book by owning a copy, wrote it, wrote about it
 - ...

and so on.

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
