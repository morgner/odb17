CAtom
=====

An CAtom is a container for a single data element, let's say a number or
a text. It stores additional information to use in a GUI as there are

 - (Field-)Name
 - Prefix 
 - Suffix
 - (output) Format template

One can compare an Atom with single data field as in a conventional
table oiented database. Unlike conventional databases fields/atoms do
not have a fixed structure, they even do not have to exist.

CAtom may act as a template for other atoms. In such case the atom,
using the other as template, does not need to fill elements which are
given by the template. It will appear as if the elements of the template
are elements of the using Atom, as long as they are not overwritten.

Sample code

.. literalinclude:: ../../../application/tests/atom-test.cpp
   :language: cpp

Output

.. code-block:: none

   atom data: 2.5
   atom frmt: gain is 2.5 %

.. doxygenclass:: odb::CAtom
