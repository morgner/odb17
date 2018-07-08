#ifndef CSTRAND_H
#define CSTRAND_H

/**
 * @file strand.h
 *
 * @author Manfred Morgner
 * @date 07.01.2018
 */

#include "node.h"
#include "generic.h"


namespace odb {

// using namespace std::string_literals;

/// forward declarations to befriend with
class COdb;

using IStrand = Identifiable<CStrand>;

/**
 * A Strand to link two Nodes (Unidirectional)
 */
class CStrand : public IStrand
    {
    friend COdb;

    public:
        /// The name of an unnamed strand
        static constexpr auto s_csNameUnnamedStrand{"unnamedStrand"};
    public:

                 /// forbidden
                 CStrand() = delete;
                 /// Normal constructor requesting a name for the strand
	         explicit CStrand(std::string const & crsName);

	         /// Nothings special
                 /// and we don't make copies
	         CStrand(CStrand const &) = delete;

                 /// make_shared<T> moveconstructs
	         CStrand(CStrand &&) = default;

        virtual ~CStrand() noexcept = default;

        /// Appending another atom to the strand
        PAtom Append(PAtom poAtom)
            {
            /// todo: implementation
            m_poAtoms.insert(poAtom);
            return poAtom;
            }

        /// friend function to send the atoms of the strand to the given stream
        friend std::ostream & operator << (std::ostream & ros, CStrand const & croStrand);

        /// Function to receive the name of the strand
        operator std::string const & ();

        /// Print the strand in an informational manner
        void print();

    protected:
        /// The name of an unnamed strand
        std::string m_sName{ s_csNameUnnamedStrand };
        /// The atoms of the strand
        CAtoms      m_poAtoms;

    }; // class CStrand

} // namespace odb

// CSTRAND_H
#endif
