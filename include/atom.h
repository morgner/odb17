#ifndef CATOM_H
#define CATOM_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <deque>
#include <ostream>
#include <string>
#include <memory>

#include "generic.h"


namespace modb {

/**
 * An Atom, from whitch Things are built from
 */
class CAtom : public Identifiable<CAtom>
    {
    public:
                 CAtom();
        virtual ~CAtom();
                 CAtom(CAtom const & other);
                 CAtom& operator=(CAtom const & other);

    protected:

    private:
    }; // class CAtom

using CAtoms = std::deque<std::shared_ptr<CAtom>>;

} // namespace modb

// CATOM_H
#endif
