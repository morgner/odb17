/**
 * @file node.cpp
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include "node.h"
#include "reason.h"


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


std::ostream & operator << (std::ostream & ros, CNode const & crNode)
    {
    ros << crNode.m_sName; // << '\n';
    bool bFirst = false; // true;
    for (auto const & a:crNode.m_spoProperties)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  Property: " << a->m_sName << " "; }
        }
    for (auto const & a:crNode.m_spoAtoms)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  " << a->m_sName << ": "; }
        a->print_atom_data_formated(ros);
        }
    for (auto const & a:crNode.m_mLink)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  "; }
        ros << " => linked for reason: " << '"' << a.second->m_sName << '"' << " to: " << '"' << a.first->m_sName << '"';
//      ros << " => linked to: " << '"' << a.first->m_sName << '"' << " for reason: " << '"' << a.second->m_sName << '"';
//      ros << " = " << crNode.m_sName << ' ' << *a.second << ' ' << a.first->m_sName;
        }
    for (auto const & a:crNode.m_spoNodesRelating)
        {
        if (bFirst) { bFirst = false; } else { ros << '\n' << "  "; }
        ros << " <= linked from: " << a->m_sName;
        }
    return ros;
    }


CNode::CNode(std::string const & crsName)
    : INode(crsName.length() ? crsName : s_csNameUnnamedNode)
    {
    }

CNode::CNode(size_t nId, std::string const & crsName)
    : INode(nId, crsName.length() ? crsName : s_csNameUnnamedNode)
    {
    }


PProperty CNode::Append (PProperty poProperty)
    {
    if ( m_spoProperties.emplace(poProperty).second )
        {
        poProperty->RelationAdd( shared_from_this() );
        }
    return poProperty;
    }

PAtom CNode::Append (PAtom poAtom)
    {
    if ( m_spoAtoms.emplace(poAtom).second )
        {
        poAtom->RelatingNodeAdd( shared_from_this() );
        }
    return poAtom;
    }

PAtom Append (PNode poNode, PAtom poAtom)
    {
    return poNode->Append( poAtom );
    }

PNode CNode::Link(PNode po2Node, PReason po4Reason)
    {
    auto me = shared_from_this();
    if (s_bDebug) std::cout << ":--LINK- -- -intern ... ---------------------------------" << '\n';
    if (s_bDebug) std::cout << " 1 QUERY -- " << m_mLink.count(po2Node) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Node->m_sName << " (" << po2Node.use_count() << ")\n";

    bool bDoLink = ( 0 == m_mLink.count(po2Node) );
    if ( !bDoLink && me != po2Node )
        {
        bDoLink = true;
        for (auto it = m_mLink.find( po2Node ); it != m_mLink.end(); ++it )
            {
            if ( it->first != po2Node )
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
        m_mLink.emplace(po2Node, po4Reason);
        po2Node->RelatingNodeAdd( me );
        po4Reason->RelationAdd( me, po2Node );
        }

    if (s_bDebug) std::cout << " 2 RESLT -- " << m_mLink.count(po2Node) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Node->m_sName << " (" << po2Node.use_count() << ")\n";
    return po2Node;
    }

PNode Link(PNode poNode, PReason po4Reason, PNode po2Node)
    {
    return poNode->Link( po2Node, po4Reason );
    }

PNode CNode::Unlink(PNode po2Node, PReason po4Reason)
    {
    auto me = shared_from_this();
    if (s_bDebug) std::cout << ":-UNLINK -- ---------------------------------------------" << '\n';
    if (s_bDebug) std::cout << " 1 QUERY -- " << m_mLink.count(po2Node) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Node->m_sName << " (" << po2Node.use_count() << ")\n";
    for (auto it = m_mLink.find( po2Node ); it != m_mLink.end(); ++it )
        {
        if (s_bDebug) std::cout << " 2 FOUND -- " << this->m_sName << ", " << *it->second << ", " << it->first->m_sName << '\n';
        if ( it->first  != po2Node )
            {
            if (s_bDebug) std::cout << " X BREAK -- " << "end of search, we leave" << '\n';
            break;
            }
        if ( it->second == po4Reason )
            {
            if (s_bDebug) std::cout << " 3 MATCH -- " << this->m_sName << ", " << *it->second << ", " << it->first->m_sName << '\n';
            if ( 1 == m_mLink.count(po2Node) )
                {
                if (s_bDebug) std::cout << " 4 ERASE -- " << this->m_sName << ", " << *it->second << ", " << it->first->m_sName << '\n';
                po2Node->RelatingNodeSub( me );
                }
            po4Reason->RelationSub( me, po2Node );
            m_mLink.erase(it);
            if (s_bDebug) std::cout << " 5 BREAK -- " << "job done, we leave" << '\n';
            break;
            }
        }
    if (s_bDebug) std::cout << " 6 RESLT -- " << m_mLink.count(po2Node) << ' ' << m_sName << " (" << me.use_count()-1 << "), " << *po4Reason << " (" << po4Reason.use_count() << "), " << po2Node->m_sName << " (" << po2Node.use_count() << ")\n";
    return po2Node;
    }

PNode CNode::RelatingNodeAdd(PNode poNode)
    {
    if (s_bDebug) std::cout << " ===> RelatingNodeAdd : " << this->m_sName << " (" << poNode.use_count() << ") -> " << poNode->m_sName << " (" << poNode.use_count() << ")\n";
    m_spoNodesRelating.emplace(poNode);
    if (s_bDebug) std::cout << " <=== RelatingNodeAdd : " << this->m_sName << " (" << poNode.use_count() << ") -> " << poNode->m_sName << " (" << poNode.use_count() << ")\n";
    return poNode;
    }

PNode CNode::RelatingNodeSub(PNode poNode)
    {
    if (s_bDebug) std::cout << " ==== RelatingNodeSub : " << this->m_sName << " -> " << poNode->m_sName << " (" << poNode.use_count() << ")\n";
    m_spoNodesRelating.erase(poNode);
    if (s_bDebug) std::cout << " ==== RelatingNodeSub : " << this->m_sName << " -> " << poNode->m_sName << " (" << poNode.use_count() << ")\n";
    return poNode;
    }

} // namespace odb
