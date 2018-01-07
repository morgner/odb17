
/*  thing.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include "thing.h"
#include "reason.h"

#include "generic.h"


namespace odb {

std::ostream & operator << (std::ostream & ros, CThing const & crThing)
    {
    ros << crThing.m_sName; // << '\n';
    bool bFirst = false; // true;
    for (auto const & a:crThing.m_qpoAtoms)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  " << a->m_sName << ": "; }
        a->print_atom_data_formated(ros);
        }
    for (auto const & a:crThing.m_mLink)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  "; }
        ros << " => linked to: " << '"' << a.first->m_sName << '"' << " for reason: " << '"' << *a.second << '"';
        ros << " = " << crThing.m_sName << ' ' << *a.second << ' ' << a.first->m_sName;
        }
    for (auto const & a:crThing.m_spoThingsRelating)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  "; }
        ros << " <= linked from: " << a->m_sName;
        }
    return ros;
    }


CThing::CThing()
    {
    }

CThing::CThing(std::string const & crsName)
: m_sName(crsName.length() ? crsName : g_csNameUnnamedThing)
    {
    }

CThing::~CThing()
    {
    };

PAtom CThing::Append (PAtom poAtom)
    {
    m_qpoAtoms.push_back(poAtom);
    poAtom->RelatingThingAdd( shared_from_this() );
    return poAtom;
    }

PThing CThing::Link(PThing po2Thing, PReason po4Reason)
    {
    m_mLink.emplace(po2Thing, po4Reason);
    po2Thing->RelatingThingAdd( shared_from_this() );
    po4Reason->RelationAdd( shared_from_this(), po2Thing );
    return po2Thing;
    }

PThing CThing::RelatingThingAdd(PThing poThing)
    {
    m_spoThingsRelating.insert(poThing);
    return poThing;
    }

} // namespace odb
