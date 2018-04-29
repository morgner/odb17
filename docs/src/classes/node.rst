CNode
======

The Node, formerly known as Object in the Object Database, is an entity
representing some 'thing' or some 'person'. Or, if you see animals neigther as
thing nor as person, some 'animal'. We are living in a database world so this
distinguation makes no difference, at least for the database.

A node may contain an arbitrary amount of arbitrary

 - CAtom's (equivalent to Data Fields)
 - CProperty's (minimal data units, multiusable)
 - Links to other CNode's for specified CReason's
 - as well as Backlinks to Reason'ed Link sources.

Basically the linking CNode manages the following resources:

 - from node to node
 - the backlink for links from itself to another nodes
 - from node to atoms
 - the backlink for links from itself to atoms

Sample code

.. literalinclude:: ../../../application/demo/nodes.cpp
   :language: cpp

Output

.. code-block:: none

    ---------------- all nodes
    Udo
       => linked to: "Ina" for reason: "is father of"
       => linked to: "Rob" for reason: "knows"
    Ina
       => linked to: "Rob" for reason: "loves"
       <= linked from: Udo
    Rob
       <= linked from: Ina
       <= linked from: Udo


.. doxygenclass:: odb::CNode
