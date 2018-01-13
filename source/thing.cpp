
/*  thing.cpp
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include "thing.h"
#include "reason.h"

#include "generic.h"


namespace odb {


void CThing::clear()
    {
    for ( auto & a:m_qpoAtoms )
        {
        a->clear();
        }
    m_qpoAtoms.clear();
    m_mLink.clear();
    }


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


CThing::CThing(std::string const & crsName)
    : m_sName(crsName.length() ? crsName : g_csNameUnnamedThing)
    {
    }

PAtom & CThing::Append (PAtom & poAtom)
    {
//    std::cout << "Append: '" << *poAtom << "' to '" << *this << "', (" << poAtom.use_count();
    m_qpoAtoms.push_back(poAtom);
    poAtom->RelatingThingAdd( shared_from_this() );
//    std::cout << ") => (" << poAtom.use_count() << ")\n";
    return poAtom;
    }

/// friend
PAtom & Append (PThing & poThing, PAtom & poAtom)
    {
//    std::cout << "Append: '" << *poAtom << "' to '" << *poThing << "', (" << poAtom.use_count();
    poThing->m_qpoAtoms.push_back(poAtom);
    poAtom->RelatingThingAdd( poThing );
//    std::cout << ") => (" << poAtom.use_count() << ")\n";
    return poAtom;
    }

PThing & CThing::Link(PThing & po2Thing, PReason & po4Reason)
    {
    auto me = shared_from_this();
    if ( me != po2Thing )
        {
        m_mLink.emplace(po2Thing, po4Reason);
        po2Thing->RelatingThingAdd( me );
        po4Reason->RelationAdd( me, po2Thing );
        }
    else
        {
        std::cerr << "Thing-LOOP, ignored: " << *this << '\n';
        }
    return po2Thing;
    }

/// friend
PThing & Link(PThing & poThing, PReason & po4Reason, PThing & po2Thing)
    {
    if ( poThing != po2Thing )
        {
        poThing->m_mLink.emplace(po2Thing, po4Reason);
        po2Thing->RelatingThingAdd( poThing );
        po4Reason->RelationAdd( poThing, po2Thing );
        }
    else
        {
        std::cerr << "Thing-LOOP, ignored: " << *poThing << '\n';
        }
    return po2Thing;
    }

PThing & CThing::Unlink(PThing & po2Thing, PReason & po4Reason)
    {
    /// todo: implementation
//    m_mLink.clear();
//    po2Thing->RelatingThingAdd( shared_from_this() );
//    po4Reason->RelationAdd( shared_from_this(), po2Thing );
    return po2Thing;
    }

PThing & CThing::RelatingThingAdd(PThing & poThing)
    {
    m_spoThingsRelating.insert(poThing);
    return poThing;
    }

} // namespace odb
