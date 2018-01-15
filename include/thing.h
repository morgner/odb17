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
        static constexpr bool s_bDebug{false};
    public:
                 CThing() = delete;
                 CThing(std::string const & crsName);
                 CThing(CThing const &) = delete;
        virtual ~CThing() = default;

        void clear();

        friend std::ostream & operator << (std::ostream & ros, CThing const & crThing);


        PAtom & Append (PAtom & poAtom);

        friend PAtom & Append (PThing & poThing, PAtom & poAtom);

        PThing & Link(PThing & po2Thing, PReason & po4Reason);

        friend PThing & Link(PThing & poThing, PReason & po4Reason, PThing & po2Thing);

        PThing & Unlink(PThing & po2Thing, PReason & po4Reason);

        PThing & RelatingThingAdd(PThing & poThing);

        PThing & RelatingThingSub(PThing & poThing);

        public:
            std::string m_sName{g_csNameUnnamedThing};
        protected:

            /// compare operator for two PThings

            template<typename T>
            struct lessIdentifiable
                {
                bool operator()(T const p1, T const p2) const
                    { return p1->id < p2->id; }
                };
            std::multimap<PThing, PReason, lessIdentifiable<PThing>> m_mLink; // link thing with thing for reason
            std::set<PThing, lessIdentifiable<PThing>>               m_spoThingsRelating;
            std::set<PAtom,  lessIdentifiable<PAtom>>                m_spoAtoms;

    }; // class CThing

} // namespace odb

// THING_H
#endif
