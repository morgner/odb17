/**
    @file main.cpp
 
    @author Manfred Morgner
    @date 21.01.2018
 */

#include <iostream>

#include "odb.h"
#include "atom.h"

/**
    @brief Demo main program
 */
int main()
    {
    auto oOdb = odb::COdb();
    auto atom = oOdb.MakeAtom(2.5, "gain", "is", "%");
    std::cout << "atom data: " << *atom << '\n';
    std::cout << "atom frmt: " << atom->m_sName << ' ';
    atom->print_atom_data_formated(std::cout);
    std::cout << '\n';
    oOdb.print_json(std::cout);
    } // int main()
