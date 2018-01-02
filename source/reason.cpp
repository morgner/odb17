/*  reason.cpp
 *
 *  Created on: 01.01.2018
 *      Author: manfred morgner
 */

#include "reason.h"
#include "thing.h"


void print(odb::CReasons & container)
  {
  for (auto const & e:container)
    {
    std::cout << e->type << " id: " << e->id << ' ' << '"' << *e << '"' << " (" << e.use_count() << ')' << '\n';
    e->Dump();
    }
  }

namespace odb {

std::ostream & operator << (std::ostream & ros, CReason const & croReason)
    {
    ros << croReason.m_sName; // << '\n';
    return ros;
    }

CReason::CReason()
    {
    }

CReason::CReason(std::string const & crsName)
: m_sName(crsName.length() ? crsName : g_csNameUnnamedReason )
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

void CReason::RelationAdd( PThing poThingFrom, PThing poThingTo )
    {
    m_mRelations.insert( std::pair<PThing, PThing>( poThingFrom, poThingTo ) );
    }

void CReason::Dump()
    {
    for ( auto const & e:m_mRelations )
        {
        std::cout << "  " << e.first->NameGet() << " => " << e.second->NameGet() << '\n';
        }
    }

std::string const & CReason::NameGet()
    {
    return m_sName;
    }

} // namespace odb
