/**
 * @file thing.cpp
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include "node.h"
#include "reason.h"

#include "generic.h"


namespace odb {

void CNode::clear()
    {
    for ( auto & a:m_spoAtoms )
        {
        a->clear();
        }
    m_spoAtoms.clear();
    m_spoProperties.clear();
    m_mLink.clear();
    }


std::ostream & operator << (std::ostream & ros, CNode const & crThing)
    {
    ros << crThing.m_sName; // << '\n';
    bool bFirst = false; // true;
    for (auto const & a:crThing.m_spoProperties)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  Property: " << a->m_sName << " "; }
        }
    for (auto const & a:crThing.m_spoAtoms)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  " << a->m_sName << ": "; }
        a->print_atom_data_formated(ros);
        }
    for (auto const & a:crThing.m_mLink)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  "; }
        ros << " => linked for reason: " << '"' << a.second->m_sName << '"' << " to: " << '"' << a.first->m_sName << '"';
//      ros << " => linked to: " << '"' << a.first->m_sName << '"' << " for reason: " << '"' << a.second->m_sName << '"';
//      ros << " = " << crThing.m_sName << ' ' << *a.second << ' ' << a.first->m_sName;
        }
    for (auto const & a:crThing.m_spoThingsRelating)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  "; }
        ros << " <= linked from: " << a->m_sName;
        }
    return ros;
    }


CNode::CNode(std::string const & crsName)
    : INode(crsName.length() ? crsName : s_csNameUnnamedThing)
    {
    }

CNode::CNode(size_t nId, std::string const & crsName)
    : INode(nId, crsName.length() ? crsName : s_csNameUnnamedThing)
    {
    }


PProperty CNode::Append (PProperty poProperty)
    {
    if ( m_spoProperties.emplace(poProperty).second )
        {
        poProperty->RelationAdd( shared_from_this() );
        }
    return std::move(poProperty);
    }

PAtom CNode::Append (PAtom poAtom)
    {
    if ( m_spoAtoms.emplace(poAtom).second )
        {
        poAtom->RelatingThingAdd( shared_from_this() );
        }
    return std::move(poAtom);
    }

PAtom Append (PNode poThing, PAtom poAtom)
    {
    return poThing->Append( poAtom );
    }

PNode CNode::Link(PNode po2Thing, PReason po4Reason)
    {
    auto me = shared_from_this();
    if (s_bDebug) std::cout << ":--LINK- -- -intern ... ---------------------------------" << '\n';
    if (s_bDebug) std::cout << " 1 QUERY -- " << m_mLink.count(po2Thing) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Thing->m_sName << " (" << po2Thing.use_count() << ")\n";

    bool bDoLink = ( 0 == m_mLink.count(po2Thing) );
    if ( !bDoLink && me != po2Thing )
        {
        bDoLink = true;
        for (auto it = m_mLink.find( po2Thing ); it != m_mLink.end(); ++it )
            {
            if ( it->first != po2Thing )
                {
                break;
                }
            if ( it->second == po4Reason )
                {
                bDoLink = false;
                if (s_bDebug) std::cerr << "ERROR; double link, ignored: " << *this << '\n';
                break;
                }
            }
        }

    if ( bDoLink )
        {
        m_mLink.emplace(po2Thing, po4Reason);
        po2Thing->RelatingThingAdd( me );
        po4Reason->RelationAdd( me, po2Thing );
        }

    if (s_bDebug) std::cout << " 2 RESLT -- " << m_mLink.count(po2Thing) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Thing->m_sName << " (" << po2Thing.use_count() << ")\n";
    return std::move(po2Thing);
    }

PNode Link(PNode poThing, PReason po4Reason, PNode po2Thing)
    {
    return poThing->Link( po2Thing, po4Reason );
    }

PNode CNode::Unlink(PNode po2Thing, PReason po4Reason)
    {
    auto me = shared_from_this();
    if (s_bDebug) std::cout << ":-UNLINK -- ---------------------------------------------" << '\n';
    if (s_bDebug) std::cout << " 1 QUERY -- " << m_mLink.count(po2Thing) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Thing->m_sName << " (" << po2Thing.use_count() << ")\n";
    for (auto it = m_mLink.find( po2Thing ); it != m_mLink.end(); ++it )
        {
        if (s_bDebug) std::cout << " 2 FOUND -- " << this->m_sName << ", " << *it->second << ", " << it->first->m_sName << '\n';
        if ( it->first  != po2Thing )
            {
            if (s_bDebug) std::cout << " X BREAK -- " << "end of search, we leave" << '\n';
            break;
            }
        if ( it->second == po4Reason )
            {
            if (s_bDebug) std::cout << " 3 MATCH -- " << this->m_sName << ", " << *it->second << ", " << it->first->m_sName << '\n';
            if ( 1 == m_mLink.count(po2Thing) )
                {
                if (s_bDebug) std::cout << " 4 ERASE -- " << this->m_sName << ", " << *it->second << ", " << it->first->m_sName << '\n';
                po2Thing->RelatingThingSub( me );
                }
            po4Reason->RelationSub( me, po2Thing );
            m_mLink.erase(it);
            if (s_bDebug) std::cout << " 5 BREAK -- " << "job done, we leave" << '\n';
            break;
            }
        }
    if (s_bDebug) std::cout << " 6 RESLT -- " << m_mLink.count(po2Thing) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Thing->m_sName << " (" << po2Thing.use_count() << ")\n";
    return std::move(po2Thing);
    }

PNode CNode::RelatingThingAdd(PNode poThing)
    {
    if (s_bDebug) std::cout << " ===> RelatingThingAdd : " << this->m_sName << " (" << poThing.use_count() << ") -> " << poThing->m_sName << " (" << poThing.use_count() << ")\n";
    m_spoThingsRelating.emplace(poThing);
    if (s_bDebug) std::cout << " <=== RelatingThingAdd : " << this->m_sName << " (" << poThing.use_count() << ") -> " << poThing->m_sName << " (" << poThing.use_count() << ")\n";
    return std::move(poThing);
    }

PNode CNode::RelatingThingSub(PNode poThing)
    {
    if (s_bDebug) std::cout << " ==== RelatingThingSub : " << this->m_sName << " -> " << poThing->m_sName << " (" << poThing.use_count() << ")\n";
    m_spoThingsRelating.erase(poThing);
    if (s_bDebug) std::cout << " ==== RelatingThingSub : " << this->m_sName << " -> " << poThing->m_sName << " (" << poThing.use_count() << ")\n";
    return std::move(poThing);
    }

} // namespace odb
