CThing
======

The Thing, formerly known as Object in the Object Database, renamed for
the practicle reason of having a unique starting letter amongst the other
code units. Should be renamed again, to "CNode" and so on.

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

.. literalinclude:: ../../../application/demo/things.cpp
   :language: cpp

Output

.. code-block:: none

    ---------------- all things
    Udo
       => linked to: "Ina" for reason: "is father of"
       => linked to: "Rob" for reason: "knows"
    Ina
       => linked to: "Rob" for reason: "loves"
       <= linked from: Udo
    Rob
       <= linked from: Ina
       <= linked from: Udo


.. doxygenclass:: odb::CThing
