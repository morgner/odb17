/*  atom.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include "atom.h"
#include "thing.h"


namespace odb {

bool CAtom::lessPThing::operator()(PThing const & poThing1,
                                   PThing const & poThing2) const
    {
    return poThing1->id < poThing2->id;
    }

} // namespace odb
