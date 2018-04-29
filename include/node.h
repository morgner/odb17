#ifndef THING_H
#define THING_H

/**
 * @file node.h
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

using INode = Identifiable<CNode>;

/**
 * @brief A Node as you imagine it
 * 
 * A CNode, or Node, is a main structure element in a GrapDB like odb. The
 * counterpart is a CReason. CNode's will be linked unidirectional to another
 * CNode's. There may be an arbitrary amount of Links between CNodes e.g.
 * 
 *  - Node, (Link-)Reason, Node
 *  - Mary, wrote, a book
 *  - Mary, read,  a book
 *  - Mary, loves, Jack
 * 
 * @author Manfred Morgner
 * @since  0.1.17
 */
class CNode : public std::enable_shared_from_this<CNode>,
              public INode
    {
    friend class COdb;
    friend class CProperty;
    friend class CReason;

    public:
        /// The name of the node
        static constexpr auto s_csNameUnnamedNode{"unnamedNode"};
        /// Do we generate debug output?
        static constexpr bool s_bDebug{false};
    public:
                 /// DELETED: We never construct without a name for the node
	         CNode() = delete;

                 /// DELETED: and we don't make copies
                 CNode(CNode const &) = delete;

                 /// Move-Contructor, noexcept and default.
                 /// make_shared<T> move-constructs.
                 /// Function return of CNode's moves too.
                 CNode(CNode&&) noexcept = default;

                 /// Normal constructor, receiving the name of the Node
                 explicit CNode(std::string const & crsName);
                 
                 /// Load constructor, receiving the ID and name of the Node
                 CNode(size_t nId, std::string const & crsName);

        /// Destructor (default). Nothings special here
        virtual ~CNode() noexcept = default;

        /// We need to unbind all relations in the COdb before letting us
        /// destruct
        void clear();

        /**
         * @brief The free output operator for CNode
         * 
         * The ouput operator prints all information about the CNode
         * instance to the given output stream. This is:
         * 
         *  - Name
         *  - CProperty's
         *  - CAtom's
         *  - Link destinations + CReason
         *  - Backlinks
         * 
         * @param ros The output stream to send the Node to
         * @param crNode The Node to output
         */
        friend std::ostream & operator << (std::ostream & ros, CNode const & crNode);

        /**
         * @brief Appends an CProperty to its property list
         *
         * Appending an CProperty to this CNode includes the Node to
         * inform the appended Property about this Node is linking to
         * it
         *
         * @param poProperty A Property to bind with the Node
         */
        PProperty Append (PProperty poProperty);

        /**
         * @brief Appends an CAtom to its atom list
         *
         * Appending an CAtom to this CNode includes the Node to
         * inform the appended Atom about this CNode is linking to it
         *
         * @param poAtom An Atom to bind into the Node
         */
        PAtom Append (PAtom poAtom);

        /**
         * @brief Links this CNode to another CNode for a CReason
         * @param po2Node A Node to Link to
         * @param po4Reason The Reason we link for
a         */
        PNode Link(PNode po2Node, PReason po4Reason);

        /**
         * @brief Removes a link to a specific CNode with a specific CReason
         * @param po2Node A Node to Linked to
         * @param po4Reason The Reason we linked for
         */
        PNode Unlink(PNode po2Node, PReason po4Reason);

        /**
         * @brief adds a CNode as referencing to this
         * @param poNode A CNode that links to us notifies us, we register it
         */
        PNode RelatingNodeAdd(PNode poNode);

        /**
         * @brief subtract a CNode as referencing to this
         * @param poNode A CNode that linked to us notifies us, we deregister it
         */
        PNode RelatingNodeSub(PNode poNode);

        /// returns if the instance is 'free'
        auto IsUnUsed()
	    {
	    return ( (0 == m_mLink.size()) &&
		     (0 == m_spoNodesRelating.size()) &&
		     (0 == m_spoProperties.size()) &&
		     (0 == m_spoAtoms.size()) );
	    }

    protected:
        /**
         * @brief Holds the links to another CNode for CReason
         * @param PNode The PNode we link to
         * @param PReason The PReason we link for
         * @param Compare Function to compare two CNodes
         */
        std::multimap<PNode, PReason, lessIdentifiableId<PNode>> m_mLink;

        /**
         * @brief Registers PNodes relating to 'this' instance
         * @param PNode The PNode we are linked from
         * @param Compare Function to compare two CNode's
         */
        SNodes               m_spoNodesRelating;

        /**
         * @brief Registers PProperties of this CNode
         * @param PProperty PProperties we have
         * @param Compare Function to compare two PAtom's
         */
        SProperties           m_spoProperties;

        /**
         * @brief Registers PAtoms of this CNode
         * @param PAtom PAtom's we own
         * @param Compare Function to compare two PAtom's
         */
        SAtoms                m_spoAtoms;

    }; // class CNode

} // namespace odb

// THING_H
#endif
