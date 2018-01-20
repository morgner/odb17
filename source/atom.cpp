/**
 * @file atom.cpp
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include "atom.h"
#include "thing.h"


namespace odb {

bool CAtom::lessPThing::operator()(PThing const & poThing1,
                                   PThing const & poThing2) const
    {
    return poThing1->id < poThing2->id;
    }

void print(CAtoms const & crContainer)
    {
    for (auto const & e:crContainer)
        {
        std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->m_sName << '\t' << " data: " << *e << " odbrep:";
        e->print_atom_data_formated(std::cout);
        std::cout  << " (" << e.use_count() << ")\n";

        for (auto const & a:e->m_spoThingsRelating)
            {
            std::cout << " CThing: " << a->m_sName << '\n';
            }
//          std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << '\n';
        }
    }

} // namespace odb
