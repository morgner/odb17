/*  reason.cpp
 *
 *  Created on: 01.01.2018
 *      Author: manfred morgner
 */

#include "reason.h"
#include "thing.h"


void print(odb::CReasons const & crContainer)
    {
    for (auto const & e:crContainer)
        {
        std::cout << e->type << " id: " << e->id << ' ' << '"' << *e << '"' << " (" << e.use_count() << ')' << '\n';
        e->print();
        }
    }

namespace odb {

std::ostream & operator << (std::ostream & ros, CReason const & croReason)
    {
    ros << croReason.m_sName; // << '\n';
    return ros;
    }

CReason::CReason(std::string const & crsName)
: m_sName(crsName.length() ? crsName : s_csNameUnnamedReason )
    {
    }

std::string const & CReason::operator = (std::string const & sName)
    {
    return m_sName = sName;
    }

CReason::operator std::string const & ()
    {
    return m_sName;
    }

void CReason::RelationAdd( PThing & poThingFrom, PThing & poThingTo )
    {
    m_mRelations.insert( std::pair<PThing, PThing>( poThingFrom, poThingTo ) );
    }

void CReason::RelationDel( PThing & poThingFrom, PThing & poThingTo )
    {
    for ( auto it = m_mRelations.find(poThingFrom); it != m_mRelations.end(); ++it )
        {
        if ( it->first  != poThingFrom ) break;
        if ( it->second == poThingTo )
            {
            m_mRelations.erase( it );
            break;
            }
        }
    }

void CReason::print()
    {
    for ( auto const & e:m_mRelations )
        {
        std::cout << "  " << e.first->m_sName << " => " << e.second->m_sName << '\n';
        }
    }

} // namespace odb
