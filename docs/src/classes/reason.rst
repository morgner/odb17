CReason
=======

CReason is necessary to link two CThing instances to give the link an
explaination. This enables to link the same CThing's multiple times.

.. code-block:: text

    thing1 - reason - thing2

For example:

.. code-block:: text

    Heinz - wrote - 'Trees of Estonia'
    Heinz - signed - 'Trees of Estonia'

Links are unidirectional. Meaning if it's true that

.. code-block:: text

    Heinz - wrote - 'Trees of Estonia'

it may not be true that

.. code-block:: text

    'Trees of Estonia' - wrote - Heinz

To ensure thing2 feels the link, it will be informed that a link to it
became established. The linked thing registers which thing is linking to
it only ones. In our example 'Trees of Estonia' registers, that Heinz
links to it.

If some process/entity needs to know how often and for which reasons, it
has to go to Heinz and ask. The linking thing cares about correct
management of links, reasons and backlinks.

CReason registers each link it is used for

Demostration

.. code-block:: cpp

    #include <iostream>

    #include "odb.h"
    #include "thing.h"

    int main()
        {
        // generating the objects
        auto oOdb    = odb::COdb();
        auto pThing1 = oOdb.MakeThing("Ulrich");
        auto pThing2 = oOdb.MakeThing("Fred");
        auto pReason = oOdb.MakeReason("pays");
        // create a connection
        pThing1->Link(pThing2, pReason);
        // let them explain the situation
        std::cout << "thing: " << *pThing1 << '\n';
        std::cout << "thing: " << *pThing2 << '\n';
        }

Output:

.. code-block:: none

   thing: Ulrich
      => linked to: "Fred" for reason: "pays" = Ulrich pays Fred
   thing: Fred
      <= linked from: Ulrich

.. doxygenclass:: odb::CReason
