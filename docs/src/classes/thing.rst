CThing
======

The Thing, formerly known as Object in the Object Database, renamed for the
practicle reason of having a unique starting letter amongst the other code
units.

It may contain an arbitrary amount of arbitrary Atoms (equivalents to Data
Fields), Links to other CThing's for specified CReason's as well as Backlinks to
Reason'ed Link sources.

The linkiing CThing is responsible for ressource management. It
manages connections

 - from thing to thing
 - the backlink for links from itself to another thing
 - from thing to atoms
 - the backlink for links from itself to atoms

Demonstration

.. code-block:: cpp

    #include <iostream>

    #include "odb.h"
    #include "atom.h"
    #include "thing.h"

    int main()
        {
        // creatinig the objects
        auto oOdb    = odb::COdb();
        auto pThing1 = oOdb.MakeThing("Ulrich");
        auto pThing2 = oOdb.MakeThing("Fred");
        auto pAtom1  = oOdb.MakeAtom("Leader", "Role");
        auto pAtom2  = oOdb.MakeAtom("Member", "Role");
        auto pReason = oOdb.MakeReason("pays");

        // linking them together
        pThing1->Append(pAtom1);
        pThing2->Append(pAtom2);
        pThing1->Link(pThing2, pReason);

        // 1. print the thing's view
        std::cout << "thing: " << *pThing1 << '\n';
        std::cout << "thing: " << *pThing2 << '\n';
        // 2. print whole database as list
        oOdb.print();
        // 3. print whole database in JSON format
        oOdb.print_json(std::cout);
        }

Output 1: The CThing's explaining there content

.. code-block:: plain

    thing: Ulrich
      Role: Leader
       => linked to: "Fred" for reason: "pays" = Ulrich pays Fred
    thing: Fred
      Role: Member
       <= linked from: Ulrich

Output 2: The whole database in list format

.. code-block:: plain

    odb::CThing	 id: 0	 name: Ulrich	(5)
    odb::CThing	 id: 1	 name: Fred	(5)
    odb::CAtom	 id: 0	 name: Role	(3)	 data: Leader
    odb::CAtom	 id: 1	 name: Role	(3)	 data: Member
    odb::CReason	 id: 0	 name: pays	(3)

Output 3: The whole database in JSON format

.. code-block:: plain

    {
    "Object Database Dump": 
        {
        "Things": 
            [
                { "id": "0", "name": "Ulrich",
                    "atoms": [ {"id": "0"} ],
                    "links": [ {"thing-id": "1", "reason-id": "0"} ] },
                { "id": "1", "name": "Fred",
                    "atoms": [ {"id": "1"} ],
                    "links": [  ] }
            ],
        "Atoms": 
            [
                { "id": "0", "name": "Role", "data": "Leader" },
                { "id": "1", "name": "Role", "data": "Member" }
            ],
        "Reasons": 
            [
                { "id": "0", "name": "pays" }
            ]
        }
    }


.. doxygenclass:: odb::CThing
