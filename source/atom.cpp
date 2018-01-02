/*  atom.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include "atom.h"

void print(odb::CAtoms & container)
  {
  for (auto const & e:container)
    {
    std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << " odbrep:";
    e->print_atom_data_formated(std::cout);
    std::cout  << " (" << e.use_count() << ')' << '\n';
//    std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << '\n';
    }
  }

namespace odb {

} // namespace odb
