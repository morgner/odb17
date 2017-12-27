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

/**
 * A Thing as they are many of
 */
class CThing : public Identifiable<CThing> {
    protected:
	int m_nInt{0};

    public:
	CThing();
	CThing(CThing const &) = default;
	virtual ~CThing();

	std::ostream& operator << (std::ostream& ros)
	    {
	    return ros << 'T' << m_nInt;
	    }

	operator std::string() const
	    {
	    return std::to_string(m_nInt);
	    }
    }; // class CThing

using CThings = std::deque<std::shared_ptr<CThing>>;

} // namespace odb

// THING_H
#endif
