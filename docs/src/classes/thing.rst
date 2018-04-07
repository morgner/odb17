CThing
======

The Thing, formerly known as Object in the Object Database, renamed for the
practicle reason of having a unique starting letter amongst the other code
units. Should be renamed again, to "CNode" and so on.

It may contain an arbitrary amount of arbitrary

 - CAtom's (equivalent to Data Fields)
 - CProperty's (minimal data units, multiusable)
 - Links to other CThing's for specified CReason's
 - as well as Backlinks to Reason'ed Link sources.

The linking CThing is responsible for resource management. It manages
connections

 - from thing to thing
 - the backlink for links from itself to another thing
 - from thing to atoms
 - the backlink for links from itself to atoms

Sample code

.. literalinclude:: ../../../application/tests/link2things-test.cpp
   :language: cpp

Output 1: The CThing's explaining there content

.. code-block:: none

   thing: Ulrich
     Role: Leader
      => linked to: "Fred" for reason: "pays"
   thing: Fred
     Role: Member
      <= linked from: Ulrich

Output 2: The whole database in list format

.. code-block:: none

   #    TYPE     ID      NAME          RefCnt    DATA
   #=========================================================
   odb::CThing	 id: 0	 name: Ulrich	(5)
   odb::CThing	 id: 1	 name: Fred	(5)
   odb::CAtom	 id: 0	 name: Role	(3)	 data: Leader
   odb::CAtom	 id: 1	 name: Role	(3)	 data: Member
   odb::CReason	 id: 0	 name: pays	(3)

Output 3: The whole database in JSON format

.. code-block:: none

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
