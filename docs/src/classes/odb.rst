COdb
====

The OjectDatabase is a Graph-Database.

see also: https://en.wikipedia.org/wiki/Graph_database

It aids you in your endeavor to warp the world around you. A GraphDB has
no actual data structure, it has a data management structure but without
any force to drive you in a specific direction.

So you may build your own world in your DB as you like it. Learning in
depth about its nature.

odb was testet with inhaling the whole IMDB with about 30Mio orignal
data records using only 27GB of RAM. If you wish to test it for yourself
(having 30GB free memory), build the odb an do th following:

.. code-block:: bash

    cd ../raw-data
    cat imdb.calls
    # do all curl calls
    gunzip *.gz
    cd ../build
    ./7-read-imdb.test 20000000

It yields a statistic like this:

.. code-block:: text

    ---------------- 12mio things
    ---------------- 15mio properties
    ---------------- 5mio reasons
    ---------------- 0 atoms

    ---------------- Search in (t)hings or ... or (s)ave&quit:
    
Now you can search the IMDB using a complete new perspective:


.. code-block:: text

    ---------------- Search in (t)hings or (r)easons or (p)roperties ...: p
    
    ---------------- Search in Properties: .*:HU:.*

    1:Carmencita - spanyol tánc:HU::imdbDisplay::0:
      Property of: Carmencita    
    
    2:A bohóc és kutyái:HU::imdbDisplay::0:
      Property of: Le clown et ses chiens
    
    ---------------- Search in (t)hings or (r)easons or (p)roperties ...: t
    
    ---------------- Search in Things: .*Star.*
    
    Demolishing and Building Up the Star Theatre
      Property: 1901 
      Property: 1:Building Up and Demolishing the Star Theatre:US::dvd::0: 
      Property: 2:Снос и возведение Звездного Театра:RU::::0: 
      Property: 3:Demolishing and Building Up the Star Theatre:::original::1: 
      Property: 4:Star Theatre:US:::copyright title:0: 
      Property: 5:A Star Theatre lebontása és felépítése:HU::imdbDisplay::0: 
      Property: 6:Demolishing and Building Up the Star Theatre:US::::0: 
      Property: Documentary 
      Property: Short 
      Property: class:movie 
      Property: short 
      Property: tt0000410 

odb may support you to build a natural image of relevant links
between CThing's amongst each other using named, unidirectional
CReason's as well as appending things like CProperty's (descriptive
mini data elements), CAtom's (true data fields) to CThing's.

You may use the odb linked in your development environment.

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
