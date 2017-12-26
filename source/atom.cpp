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

CAtom::CAtom(const CAtom& other)
    {
    //copy ctor
    }

CAtom& CAtom::operator=(const CAtom& rhs)
    {
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
    }

} // namespace modb
