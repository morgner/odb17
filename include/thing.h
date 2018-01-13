#ifndef THING_H
#define THING_H

/*  thing.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <deque>
#include <ostream>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <functional>

#include "generic.h"
#include "atom.h"


namespace odb {

using namespace std::string_literals;

/// forward declarations to befriend with
class COdb;
class CReason;

/**
 * A Thing as they are many of
 */
class CThing : public std::enable_shared_from_this<CThing>,
               public Identifiable<CThing>
    {
    friend class COdb;
    friend class CReason;

    public:
        static constexpr auto g_csNameUnnamedThing{"unnamedThing"};
    public:
                 CThing() = delete;
                 CThing(std::string const & crsName);
                 CThing(CThing const &) = default;
        virtual ~CThing() = default;

        void clear();

        friend std::ostream & operator << (std::ostream & ros, CThing const & crThing);


        PAtom & Append (PAtom & poAtom);

        friend PAtom & Append (PThing & poThing, PAtom & poAtom);

        PThing & Link(PThing & po2Thing, PReason & po4Reason);

        friend PThing & Link(PThing & poThing, PReason & po4Reason, PThing & po2Thing);

        PThing & Unlink(PThing & po2Thing, PReason & po4Reason);

        PThing & RelatingThingAdd(PThing & poThing);

        PThing & RelatingThingDel(PThing & poThing);

        protected:
            std::string m_sName{g_csNameUnnamedThing};
            CAtoms      m_qpoAtoms;

            /// compare operator for two PThings
            struct lessPThing
                {
                bool operator()(PThing const p1, PThing const p2) const
                    { return p1->id < p2->id; }
                };
            std::multimap<PThing, PReason, lessPThing> m_mLink; // link thing with thing for reason
            std::set<PThing, lessPThing>               m_spoThingsRelating;

    }; // class CThing

} // namespace odb

// THING_H
#endif
