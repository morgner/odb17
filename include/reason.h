#ifndef CREASON_H
#define CREASON_H

/**
 * @file reason.h
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include <map>

#include "node.h"
#include "generic.h"


/// print CReasons in an informational fasion
void print(odb::CReasons const & crContainer);

namespace odb {

/// forward declarations of COdb to befriend with
class COdb;


/**
 *  @brief A Reason to link two Nodes (Unidirectional)
 */
class CReason : public IReason
    {
    friend COdb;

    public:
        /// The name of an unnamed reason
        static constexpr auto s_csNameUnnamedReason{"unnamedReason"};
    public:
                 /// DELETED: Default constructor
                 CReason() = delete;

                 /// DELETED: There is no reason to copy a CReason
                 CReason(CReason const &) = delete;

                 /// DEFAULT, NOEXCEPT: move constructor, 
                 /// make_shared<T> move-constructs returning objects
                 /// has to move too, we don't want copies!
                 CReason(CReason &&) noexcept = default;

                 /// Normal constructor, receiving the name of the reason
                 explicit CReason(std::string const & crsName);

                 /// Load constructor, receiving the ID and name of the reason
                 CReason(size_t nId, std::string const & crsName);

                 /// DEFAULT, NOEXCEPT: destructor
        virtual ~CReason() noexcept = default;

        /// Output operator to do an output of the instance
        friend std::ostream & operator << (std::ostream & ros, CReason const & croReason);

        /// Conversion operator will return the name of the instance
        operator std::string const & ();

        /// Add the information about linking from one CNode to another
        /// regarding 'this' CReason
        void RelationAdd( PNode & poNodeFrom, PNode & poNodeTo );

        /**
           @brief Removes a link between two CNode's

           Removes the information about a particular link from one CNode to
           another regarding 'this' reason

           @param poNodeFrom The node the link is claimed to be made from
           @param poNodeTo The node the link is claimed to be made to
         */
        void RelationSub( PNode & poNodeFrom, PNode & poNodeTo );

        /// Prints an informational output to std::cout
        void print();

        /// returns if the instance is 'free'
        auto IsUnUsed()
	    {
	    return (0 == m_mRelations.size());
	    }

    protected:
        /// A map containing all links from one node to another using
        /// 'this' CReason
        std::multimap<PNode, PNode> m_mRelations;

    }; // class CReason

} // namespace odb

// CREASON_H
#endif
