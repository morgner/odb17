#ifndef CSTRAND_H
#define CSTRAND_H

/*  atom.h
 *
 *  Created on: 07.01.2018
 *      Author: manfred morgner
 */


#include "thing.h"
#include "generic.h"


namespace odb {

// using namespace std::string_literals;

/// forward declarations to befriend with
class COdb;

/**
 * A Strand to link two Things (Unidirectional)
 */
class CStrand : public Identifiable<CStrand>
    {
    friend COdb;

    public:
        static constexpr auto s_csNameUnnamedStrand{"unnamedStrand"};
    public:

        CStrand();
        CStrand(std::string const & crsName);

        PAtom Append(PAtom poAtom)
            {
            /// todo: implementation
            m_poAtoms.push_back(poAtom);
            return poAtom;
            }

        std::string const & operator = (std::string const & sName);

        friend std::ostream & operator << (std::ostream & ros, CStrand const & croStrand);

        operator std::string const & ();

        void print();

    protected:
        std::string m_sName{ s_csNameUnnamedStrand };
        CAtoms      m_poAtoms;

    }; // class CStrand

} // namespace odb

// CSTRAND_H
#endif
