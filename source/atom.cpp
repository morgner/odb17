/*  atom.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include "atom.h"

namespace modb {

CAtom::CAtom()
    {
    //ctor
    }

CAtom::~CAtom()
    {
    //dtor
    }

CAtom::CAtom(CAtom const & other)
    {
    //copy ctor
    }

CAtom& CAtom::operator=(CAtom const & rhs)
    {
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
    }

} // namespace modb
