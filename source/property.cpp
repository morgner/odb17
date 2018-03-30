/**
 * @file property.cpp
 *
 * @author Manfred Morgner
 * @date 01.02.2018
 */

#include "property.h"
#include "thing.h"


void print(odb::CProperties const & crContainer)
    {
    for (auto const & e:crContainer)
        {
        std::cout << e->m_sType << " id: " << e->m_nId << ' ' << '"' << *e << '"' << " (" << e.use_count() << ')' << '\n';
        e->print();
        }
    }

namespace odb {

std::ostream & operator << (std::ostream & ros, CProperty const & croProperty)
    {
    ros << croProperty.m_sName << '\n';
    for ( auto const & a:croProperty.m_oRelations )
        {
	ros << "  Property of: " << a->m_sName << '\n';
	}
    ros << "  total: " << croProperty.m_oRelations.size() << '\n';
    return ros;
    }

CProperty::CProperty(std::string const & crsName)
    : IProperty(crsName.length() ? crsName : s_csNameUnnamedProperty )
    {
    }

CProperty::CProperty(size_t nId, std::string const & crsName)
    : IProperty(nId, crsName.length() ? crsName : s_csNameUnnamedProperty )
    {
    }

CProperty::operator std::string const & ()
    {
    return m_sName;
    }

void CProperty::RelationAdd( PThing poThing )
    {
    m_oRelations.insert( poThing );
    }

void CProperty::RelationSub( PThing poThing )
    {
    m_oRelations.erase( poThing );
    }

void CProperty::print()
    {
    for ( auto const & e:m_oRelations )
        {
        std::cout << "  " << e->m_sName << '\n';
        }
    }

} // namespace odb
