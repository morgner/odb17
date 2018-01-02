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
#include <unordered_map>
#include <set>
#include <functional>

#include "generic.h"
#include "atom.h"

#include "reason.h"

namespace odb {

using namespace std::string_literals;



/**
 * A Thing as they are many of
 */
class CThing : public std::enable_shared_from_this<CThing>,
               public Identifiable<CThing> {
    public:
      static constexpr auto g_csNameUnnamedThing{"unnamedThing"};
    public:
	         CThing() {};
             CThing(std::string const & crsName)
	         : m_sName(crsName.length() ? crsName : g_csNameUnnamedThing)
             {
             }
	         CThing(CThing const &) = default;
	virtual ~CThing() {};

	friend auto & operator << (std::ostream & ros, CThing const & crThing)
	    {
	    ros << crThing.m_sName; // << '\n';
        bool bFirst = false; // true;
	    for (auto const & a:crThing.m_qpoAtoms)
	      {
          if (bFirst) { bFirst = false; } else { ros << '\n' << "  " << a->NameGet() << ": "; }
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

    auto const & NameGet() { return m_sName; } const

    auto Append (PAtom poAtom)
      {
      m_qpoAtoms.push_back(poAtom);
      poAtom->RelatingThingAdd( shared_from_this() );
      return poAtom;
      }

    PThing Link(PThing po2Thing, PReason po4Reason)
      {
      m_mLink.emplace(po2Thing, po4Reason);
      po2Thing->RelatingThingAdd( shared_from_this() );
      po4Reason->RelationAdd( shared_from_this(), po2Thing );
      return po2Thing;
      }

    PThing RelatingThingAdd(PThing poThing)
      {
      m_spoThingsRelating.insert(poThing);
      return poThing;
      }

    protected:
        std::string                              m_sName{g_csNameUnnamedThing};
        CAtoms                                   m_qpoAtoms;
        std::unordered_multimap<PThing, PReason> m_mLink;

        /// We need a set to get only one backreference from CThings
        /// even if there are multiple incoming links
        struct lessPThing
          {
          bool operator()(PThing const p1, PThing const p2) const
            { return p1->id < p2->id; }
          };
        std::set<PThing, lessPThing>             m_spoThingsRelating;

    }; // class CThing


} // namespace odb

// THING_H
#endif
