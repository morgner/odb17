#ifndef CREASON_H
#define CREASON_H

/**
 * @file reason.h
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include <map>

#include "thing.h"
#include "generic.h"


/// print CReasons in an informational fasion
void print(odb::CReasons const & crContainer);

namespace odb {

/// forward declarations to befriend with
class COdb;


/**
    @brief A Reason to link two Things (Unidirectional)
 */
class CReason : public IReason
    {
    friend COdb;

    public:
        /// The name of an unnamed reason
        static constexpr auto s_csNameUnnamedReason{"unnamedReason"};
    public:
                 /// Forbidden
                 CReason() = delete;

                 /// There is no reason to copy a CReason
                 CReason(CReason const &) = delete;

                 /// make_shared<T> moveconstruct
                 CReason(CReason &&) noexcept = default;

                 /// Normal constructor, receiving the name of the reason
	         explicit CReason(std::string const & crsName);

                 /// Load constructor, receiving the ID and name of the reason
                 CReason(size_t nId, std::string const & crsName);

        virtual ~CReason() noexcept = default;

        /// Output operator to do an output of the instance
        friend std::ostream & operator << (std::ostream & ros, CReason const & croReason);

        /// Conversion operator will return the name of the instance
        operator std::string const & ();

        /// Add the information about a link from one CThing to another regarding 'this' reason
        void RelationAdd( PThing & poThingFrom, PThing & poThingTo );

        /**
           @brief Removes a link between two CThing's

           Removes the information about a particular link from one CThing to
           another regarding 'this' reason

           @param poThingFrom The thing the link is claimed to be made from
           @param poThingTo The thing the link is claimed to be made to
         */
        void RelationSub( PThing & poThingFrom, PThing & poThingTo );

        /// Prints an informational output to std::cout
        void print();

    protected:
        /// A map containing links from  one thing to another
        std::multimap<PThing, PThing> m_mRelations;

    }; // class CReason

} // namespace odb

// CREASON_H
#endif
