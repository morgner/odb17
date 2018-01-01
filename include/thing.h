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
class CThing : public Identifiable<CThing> {
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

	friend std::ostream & operator << (std::ostream & ros, CThing const & crThing)
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

    auto emplace_back(PAtom & poAtom) { m_qpoAtoms.emplace_back(poAtom); return poAtom; }

    protected:
        std::string m_sName{g_csNameUnnamedThing};
        CAtoms      m_qpoAtoms;

    }; // class CThing

using PThing = std::shared_ptr<CThing>;
using CThings = std::deque<PThing>;

} // namespace odb

// THING_H
#endif
