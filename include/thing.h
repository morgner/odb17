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
#include "property.h"


namespace odb {

/// forward declarations to befriend with
class COdb;
class CProperty;
class CReason;

using IThing = Identifiable<CThing>;

/**
 * @brief A Thing as there are many
 * 
 * A CThing, or Node, is a main structure element in a GrapDB like
 * odb. The counterpart is a CReason. CThing's will be linked
 * unidirectional to another CThing. There may be an arbitrary amount
 * of Links between CThings e.g.
 * 
 *  - Node, (Link-)Reason, Node
 *  - Mary, wrote, Sandberg
 *  - Mary, read,  Sandberg
 *  - Mary, loves, Jack
 * 
 * @author Manfred Morgner
 * @since  0.1.17
 */
class CThing : public std::enable_shared_from_this<CThing>,
               public IThing
    {
    friend class COdb;
    friend class CProperty;
    friend class CReason;

    public:
        /// The name of the thing
        static constexpr auto s_csNameUnnamedThing{"unnamedThing"};
        /// Do we generate debug output?
        static constexpr bool s_bDebug{false};
    public:
                 /// DELETED: We never construct without a name for the thing
	         CThing() = delete;

                 /// DELETED: and we don't make copies
                 CThing(CThing const &) = delete;

                 /// Move-Contructor, noexcept and default.
                 /// make_shared<T> move-constructs.
                 /// Function return of CThing's moves too.
                 CThing(CThing&&) noexcept = default;

                 /// Normal constructor, receiving the name of the Thing
                 explicit CThing(std::string const & crsName);
                 
                 /// Load constructor, receiving the ID and name of the Thing
                 CThing(size_t nId, std::string const & crsName);

        /// Destructor (default). Nothings special here
        virtual ~CThing() noexcept = default;

        /// We need to unbind all relations in the COdb before letting us
        /// destruct
        void clear();

        /// Compares the name of a thing with an input string for 'equal'
        friend bool operator == (PThing const & croThing, std::string const & crsInput)
            {
            return croThing->m_sName == crsInput;
            }

        /// Compares the name of a thing with an input string for 'lesser'
        friend bool operator <  (PThing const & croThing, std::string const & crsInput)
            {
            return croThing->m_sName < crsInput;
            }

        /**
         * @brief The free output operator for CThing
         * 
         * The ouput operator prints all information about the CThing
         * instance to the given output stream. This is:
         * 
         *  - Name
         *  - CProperty's
         *  - CAtom's
         *  - Link destinations + CReason
         *  - Backlinks
         * 
         * @param ros The output stream to send the Thing to
         * @param crThing The Thing to output
         */
        friend std::ostream & operator << (std::ostream & ros, CThing const & crThing);

        /**
         * @brief Appends an CProperty to its property list
         *
         * Appending an CProperty to this CThing includes the Thing to
         * inform the appended Property about this Thing is linking to
         * it
         *
         * @param poProperty A Property to bind with the Thing
         */
        PProperty Append (PProperty poProperty);

        /**
         * @brief Appends an CAtom to its atom list
         *
         * Appending an CAtom to this CThing includes the Thing to
         * inform the appended Atom about this CThing is linking to it
         *
         * @param poAtom An Atom to bind into the Thing
         */
        PAtom Append (PAtom poAtom);

        /**
         * @brief Links this CThing to another CThing for a CReason
         * @param po2Thing A Thing to Link to
         * @param po4Reason The Reason we link for
         */
        PThing Link(PThing po2Thing, PReason po4Reason);

        /**
         * @brief Removes a link to a specific CThing with a specific CReason
         * @param po2Thing A Thing to Linked to
         * @param po4Reason The Reason we linked for
         */
        PThing Unlink(PThing po2Thing, PReason po4Reason);

        /**
         * @brief adds a CThing as referencing to this
         * @param poThing A CThing that links to us notifies us, we register it
         */
        PThing RelatingThingAdd(PThing poThing);

        /**
         * @brief subtract a CThing as referencing to this
         * @param poThing A CThing that linked to us notifies us, we deregister it
         */
        PThing RelatingThingSub(PThing poThing);

        protected:
            /**
             * @brief Holds the links to another CThing for CReason
             * @param PThing The PThing we link to
             * @param PReason The PReason we link for
             * @param Compare Function to compare two CThings
             */
            std::multimap<PThing, PReason, lessIdentifiableId<PThing>> m_mLink;

            /**
             * @brief Registers PThings relating to itself
             * @param PThing The PThing we are linked from
             * @param Compare Function to compare two CThing's
             */
            std::set<PThing, lessIdentifiableId<PThing>>               m_spoThingsRelating;

            /**
             * @brief Registers PProperties of this CThing
             * @param PProperty PProperties we have
             * @param Compare Function to compare two PAtom's
             */
            std::set<PProperty, lessIdentifiableName<PProperty>>       m_spoProperties;

            /**
             * @brief Registers PAtoms of this CThing
             * @param PAtom PAtom's we own
             * @param Compare Function to compare two PAtom's
             */
            std::set<PAtom, lessIdentifiableId<PAtom>>                 m_spoAtoms;

    }; // class CThing

} // namespace odb

// THING_H
#endif
