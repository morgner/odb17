#ifndef CPROPERTY_H
#define CPROPERTY_H

/**
 * @file property.h
 *
 * @author Manfred Morgner
 * @date 01.02.2018
 */

#include <map>

#include "thing.h"
#include "generic.h"


/// print CProperties in an informational fasion
void print(odb::CProperties const & crContainer);

namespace odb {

using namespace std::string_literals;

/// forward declarations to befriend with
class COdb;

/**
    @brief A Property for a CThing
 */
class CProperty : public Identifiable<CProperty>
    {
    friend COdb;

    public:
        /// The name of an unnamed property
        static constexpr auto s_csNameUnnamedProperty{"unnamedProperty"};
    public:
                 /// @brief deleted: default constructor
                 CProperty() = delete;

                 /// @brief deleted: copy constructor
		 /// There is no reason to copy a CProperty
                 CProperty(CProperty const & src) = delete;

                 /// Normal constructor, receiving the name of the property
                 CProperty(std::string const & crsName);

        virtual ~CProperty() = default;

        /// Assignment operator to assign a name to the instance
        std::string const & operator = (std::string const & crsName);

        /// Output operator to do an output of the instance
        friend std::ostream & operator << (std::ostream & ros, CProperty const & croProperty);

        /// Conversion operator will return the name of the instance
        operator std::string const & ();
        
	/// Compares the name with an input string
	friend bool operator == (PProperty const & croProperty, std::string const & crsInput)
	    {
            return croProperty->m_sName == crsInput;
	    }

        /// Add the information about a link from a CThing
        void RelationAdd( PThing poThing );

        /// @brief Removes a link to a CThing
        /// @param poThing The thing the link is pointing to
        void RelationSub( PThing poThing );

        /// Prints an informational output to std::cout
        void print();

    public:
        /// The name of the property
        std::string m_sName{ s_csNameUnnamedProperty };
//  ? protected:
        /// A set containing backlinks from things
        std::set<PThing> m_oRelations;

    }; // class CProperty

} // namespace odb

// CPROPERTY_H
#endif
