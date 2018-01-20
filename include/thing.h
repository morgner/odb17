#ifndef THING_H
#define THING_H

/**
 * @file thing.h
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include <deque>
#include <ostream>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <functional>

#include "generic.h"
#include "atom.h"


namespace odb {

using namespace std::string_literals;

/// forward declarations to befriend with
class COdb;
class CReason;

/**
@brief A Thing as they are many of

The Thing, formerly known as Object in the Object Database, renamed for
the practicle reason of having a unique starting letter amongst the
other code units.

It may contain an arbitrary amount of arbitrary Atoms (equivalents to
Data Fields), Links to other CThing's for specified CReason's as well
as Backlinks to Reason'ed Link sources.

The linkiing CThing is responsible for ressource management. It
manages connections

 - from thing to thing
 - the backlink for links from itself to another thing
 - from thing to atoms
 - the backlink for links from itself to atoms

@author Manfred Morgner
@since  0.0.1 c++17
 */
class CThing : public std::enable_shared_from_this<CThing>,
               public Identifiable<CThing>
    {
    friend class COdb;
    friend class CReason;

    public:
        /// The name of the thing
        static constexpr auto s_csNameUnnamedThing{"unnamedThing"};
        /// Do we generate debug output?
        static constexpr bool s_bDebug{false};
    public:
                /// We construct always with a name for the Thing
                 CThing() = delete;
                /// and we don't make copies
                 CThing(CThing const &) = delete;
                /**
                 * @brief The constructor taking a name for the CThing
                 * @param crsName A name for the Thing
                 * @author Manfred Morgner
                 * @since  0.0.1 c++17
                 */
                 CThing(std::string const & crsName);
                /// Nothings special here
        virtual ~CThing() = default;

        /**
         * @brief We need to unbind all relations in the odb before destructing
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        void clear();

        /**
         * @brief The free output operator for CThing
         * @param ros The output stream to send the Thing to
         * @param crThing The Thing to output
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        friend std::ostream & operator << (std::ostream & ros, CThing const & crThing);

        /**
         * @brief Appends an CAtom to its atom list
         *
         * Appending an CAtom to this CThing requires the thing to
         * inform the appended Atom about this CThing is linking to it
         *
         * @param poAtom An Atom to bind into the Thing
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        PAtom & Append (PAtom & poAtom);

        /**
         * @brief The free Append function for CAtoms to CThing
         * @param poThing A Thing to receive an Atom
         * @param poAtom An Atom to bind into the Thing
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        friend PAtom & Append (PThing & poThing, PAtom & poAtom);

        /**
         * @brief Links this CThing to another CThing for a CReason
         * @param po2Thing A Thing to Link to
         * @param po4Reason The Reason we link for
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        PThing & Link(PThing & po2Thing, PReason & po4Reason);

        /**
         * @brief The free Link function for CThing to CThing
         * @param poThing The Thing to create a link from
         * @param po2Thing A Thing to Link to
         * @param po4Reason The Reason we link for
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        friend PThing & Link(PThing & poThing, PReason & po4Reason, PThing & po2Thing);

        /**
         * @brief Removes a link to a specific CThing with a specific CReason
         * @param po2Thing A Thing to Linked to
         * @param po4Reason The Reason we linked for
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        PThing & Unlink(PThing & po2Thing, PReason & po4Reason);

        /**
         * @brief adds a CThing as referencing to this
         * @param poThing A CThing that links to us notifies us, we register it
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        PThing & RelatingThingAdd(PThing & poThing);

        /**
         * @brief subtract a CThing as referencing to this
         * @param poThing A CThing that linked to us notifies us, we deregister it
         * @author Manfred Morgner
         * @since  0.0.1 c++17
         */
        PThing & RelatingThingSub(PThing & poThing);

        public:
            /// The name of the CThing
            std::string m_sName{s_csNameUnnamedThing};
        protected:
            /**
             * @brief compare operator for two Identifiable's
             *
             * Universal compare operator comparing class instances derived
             * from class Identifiable by instance member 'id'
             *
             * @tparam T The type of the elements to compare
             *
             * @author Manfred Morgner
             * @since  0.0.1 c++17
             */
            template<typename T>
            struct lessIdentifiable
                {
                /**
                 * @brief Compares two objects derived from Identifiable
                 * @param p1 The element to compare
                 * @param p2 The element to compare with
                 * @author Manfred Morgner
                 * @since  0.0.1 c++17
                 */
                bool operator()(T const & p1, T const & p2) const
                    { return p1->id < p2->id; }
                };
            /**
             * @brief Holds the links to another CThing for CReason
             * @param PThing The PThing we link to
             * @param PReason The PReason we link for
             * @param Compare Function to compare two CThings
             * @author Manfred Morgner
             * @since  0.0.1 c++17
             */
            std::multimap<PThing, PReason, lessIdentifiable<PThing>> m_mLink;

            /**
             * @brief Registers PThings relating to itself
             * @param PThing The PThing we are linked from
             * @param Compare Function to compare two CThing's
             * @author Manfred Morgner
             * @since  0.0.1 c++17
             */
            std::set<PThing, lessIdentifiable<PThing>>               m_spoThingsRelating;

            /**
             * @brief Registers PThings relating to itself
             * @param PAtom PAtom's we own
             * @param Compare Function to compare two PAtom's
             * @author Manfred Morgner
             * @since  0.0.1 c++17
             */
            std::set<PAtom, lessIdentifiable<PAtom>>                 m_spoAtoms;

    }; // class CThing

} // namespace odb

// THING_H
#endif
