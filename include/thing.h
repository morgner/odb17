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

	auto & operator << (std::ostream& ros)
	    {
	    return ros << m_sName << ":" << id;
	    }

        auto const & NameGet() { return m_sName; }

    protected:
        std::string m_sName{g_csNameUnnamedThing};

    }; // class CThing

using PThing = std::shared_ptr<CThing>;
using CThings = std::deque<PThing>;

} // namespace odb

// THING_H
#endif
