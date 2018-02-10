CAtom
=====

An CAtom is a container for a single data element, let's say a number or
a text. It stores additional information to use in a GUI as there are

 - Name
 - Prefix 
 - Suffix
 - Format template

One can compare an atom with single data field like in a conventional
database. Unlike conventional databases fields/atoms do not have a fixed
structure, they even do not have to exist.

CAtom may act as a template for other atoms. In such case the atom,
using the other as template, does not need to fill elements which are
given by the template. It will appear as if the elements of the template
are elements of the using atom, as long as they are not overwritten.

Sample code

.. code-block:: cpp
 
    #include <iostream>

    #include "odb.h"
    #include "atom.h"

    int main()
       {
       auto oOdb = odb::COdb();
       auto atom = oOdb.MakeAtom(2.5, "gain", "is", "%");
       std::cout << "atom data: " << *atom << '\n';
       std::cout << "atom frmt: " << atom->m_sName << ' ';
       atom->print_atom_data_formated(std::cout);
       std::cout << '\n';
       }

Output

.. code-block:: none

   atom data: 2.5
   atom frmt: gain is 2.5 %

.. doxygenclass:: odb::CAtom
