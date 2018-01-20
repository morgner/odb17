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

// using namespace std::string_literals;

/// forward declarations to befriend with
class COdb;

/**
  @brief A Reason to link two Things (Unidirectional)

  This class is necessary to link two CThing instances to give the link an
  explaination. This enables us to link the same CThing's multiple time.

  - thing1 - reason - thing2

  For example:

  -# Heinz - wrote - 'Trees of Estonia'
  -# Heinz - signed - 'Trees of Estonia'

  Links are unidirectional. Meaning if it's true that

  - Heinz - wrote - 'Trees of Estonia'

  it may not be true that

  - 'Trees of Estonia' - wrote - Heinz

  But to ensure thing2 feels the link, it will be informed that a link to it
  became established. The linked thing registers which thing is linking
  to it only ones. In our example 'Trees of Estonia' registers, that Heinz links
  to it.

  If some process/entity needs to know how often and for which reasons, it
  has to go to Heinz and ask. The linking thing is resonsible for correct
  management of links, reasons and backlinks.

  CReason registers each link it it used for

 */
class CReason : public Identifiable<CReason>
    {
    friend COdb;

    public:
        /// The name of an unnamed reason
        static constexpr auto s_csNameUnnamedReason{"unnamedReason"};
    public:
                 /// Forbidden
                 CReason() = delete;

                 /// There is no reason to copy a CReason
                 CReason(CReason const & src) = delete;

                 /// Normal constructor, receiving the name of the reason
                 CReason(std::string const & crsName);

        virtual ~CReason() = default;

        /// Assignment operator to assign a name to the instance
        std::string const & operator = (std::string const & crsName);

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
        /// The name of the reason
        std::string m_sName{ s_csNameUnnamedReason };
        /// A map containing links from  one thing to another
        std::multimap<PThing, PThing> m_mRelations;

    }; // class CReason

} // namespace odb

// CREASON_H
#endif
