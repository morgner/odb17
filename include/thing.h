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

#include "generic.h"
#include "atom.h"

namespace odb {

using namespace std::string_literals;



/**
 * A Thing as they are many of
 */
class CThing : public std::enable_shared_from_this<CThing>, public Identifiable<CThing> {
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
          if (bFirst) { bFirst = false; } else { ros << '\n' << "  "; }
	      a->print_atom_data_formated(ros);
	      }
	    return ros;
	    }

    auto const & NameGet() { return m_sName; }

    auto Append (PAtom poAtom)
      {
      m_qpoAtoms.push_back(poAtom);
      poAtom->RelatingThingAdd( shared_from_this() );
      return poAtom;
      }

    protected:
        std::string m_sName{g_csNameUnnamedThing};
        CAtoms      m_qpoAtoms;

    }; // class CThing


} // namespace odb

// THING_H
#endif
