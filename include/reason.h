#ifndef CREASON_H
#define CREASON_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <map>

#include "thing.h"
#include "generic.h"


void print(odb::CReasons & container);

namespace odb {

// using namespace std::string_literals;

/// forward declarations to befriend with
class COdb;

/**
 * A Reason to link two Things (Unidirectional)
 */
class CReason : public Identifiable<CReason>
    {
    friend COdb;

    public:
        static constexpr auto s_csNameUnnamedReason{"unnamedReason"};
    public:

                 CReason() = delete;
                 CReason(std::string const & crsName);
        virtual ~CReason() = default;

        std::string const & operator = (std::string const & sName);

        friend std::ostream & operator << (std::ostream & ros, CReason const & croReason);

        operator std::string const & ();

        void RelationAdd( PThing poThingFrom, PThing poThingTo );

        void print();

    protected:
        std::string m_sName{ s_csNameUnnamedReason };
        std::multimap<PThing, PThing> m_mRelations;

    }; // class CReason

} // namespace odb

// CREASON_H
#endif
