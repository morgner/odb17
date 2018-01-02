#ifndef CREASON_H
#define CREASON_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

// #include <deque>
// #include <string>
// #include <memory>

// #include <iostream>
// #include <vector>
// #include <array>
#include <map>

#include "thing.h"
#include "generic.h"

namespace odb {

// using namespace std::string_literals;


/**
 * A Reason to link two Things (Unidirectional)
 */
class CReason : public Identifiable<CReason>
  {
  public:
    static constexpr auto g_csNameUnnamedReason{"unnamedReason"};
  public:
	  CReason();
	  CReason(std::string const & crsName)
	    : m_sName(crsName.length() ? crsName : g_csNameUnnamedReason )
	    {
	    }

	  auto const & operator = (std::string const & sName)
	    {
	    return m_sName = sName;
	    }

      friend auto & operator << (std::ostream & ros, CReason const & croReason)
	    {
	    ros << croReason.m_sName; // << '\n';
	    return ros;
	    }

	  operator std::string const & ()
	    {
	    return m_sName;
	    }

      void RelationAdd( PThing poThingFrom, PThing poThingTo )
        {
        m_mRelations.insert( std::pair<PThing, PThing>( poThingFrom, poThingTo ) );
        }

      void Dump()
        {
        for ( auto const & e:m_mRelations )
          {
          std::cout << "  " << e.first << " => " << e.second << '\n';
          }
        }

      auto const & NameGet() { return m_sName; }

  protected:
    std::string m_sName{ g_csNameUnnamedReason };
    std::multimap<PThing, PThing> m_mRelations;

  }; // class CReason

using PReason  = std::shared_ptr<CReason>;
using CReasons = std::deque<PReason>;

inline auto print(CReasons & container)
  {
  for (auto const & e : container)
    {
    std::cout << e->type << " id: " << e->id << ' ' << '"' << *e << '"' << " (" << e.use_count() << ')' << '\n';
    e->Dump();
    }
  }

} // namespace odb

// CREASON_H
#endif
