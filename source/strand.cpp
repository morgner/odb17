/*  reason.cpp
 *
 *  Created on: 01.01.2018
 *      Author: manfred morgner
 */

#include "strand.h"
#include "thing.h"
#include "generic.h"


void print(odb::CStrands & crContainer)
    {
    for (auto const & e:crContainer)
        {
        std::cout << e->type << " id: " << e->m_nId << ' ' << '"' << *e << '"' << " (" << e.use_count() << ')' << '\n';
        e->print();
        }
    }

namespace odb {

std::ostream & operator << (std::ostream & ros, CStrand const & croStrand)
    {
    ros << croStrand.m_sName; // << '\n';
    return ros;
    }

CStrand::CStrand(std::string const & crsName)
: m_sName(crsName.length() ? crsName : s_csNameUnnamedStrand )
    {
    }

std::string const & CStrand::operator = (std::string const & crsName)
    {
    return m_sName = crsName.length() ? crsName : s_csNameUnnamedStrand;
    }

CStrand::operator std::string const & ()
    {
    return m_sName;
    }

void CStrand::print()
    {
/*
    for ( auto const & e:m_mRelations )
        {
        std::cout << "  " << e.first->m_sName << " => " << e.second->m_sName << '\n';
        }
*/
    }

} // namespace odb
