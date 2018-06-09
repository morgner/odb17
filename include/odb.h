#ifndef CODB_H
#define CODB_H

/**
 * @file odb.h
 *
 * @author Manfred Morgner
 * @date 26.12.2017
 */

#include <string>
#include <memory>    // shared_ptr
#include <iostream>  // cout
#include <regex>
#include <set>
#include <vector>
#include <optional>

#include "generic.h"

#include <fstream>     // Load'n'Save
#include <json/json.h>
//#include <json.hpp>  // nlohmann
#include <stdlib.h>    // atol

#include "atom.h"
#include "node.h"
#include "property.h"
#include "reason.h"
#include "strand.h"


namespace odb {


/**
 *  @brief The database
 *
 *  A Object Database itself is an Identifiable object. Enabeling multiple
 *  database instances in one application.
 *
 *  @author Manfred Morgner
 *  @since  0.1.17
 */
class COdb : public IOdb
    {
    public:
                /**
                 * @brief Creates a COdb, a database
                 *
                 * We only default construct the database, no copy construction
                 * no copy of the database at all. It is not known, what copying
                 * the database means.
                 */
	         COdb() = default;
                 /**
                   * @brief deleted: copy constructor
                   *
                   * Copying a databse is not supported yet. We will support it
                   * as soon as we find out what it means.
                   */
                 COdb(COdb const & src) = delete;
                 /**
                   * @brief deleted: move constructor
                   *
                   * Moving a databse is not supported yet.
                   */
                 COdb(COdb const && src) = delete;
                 /**
                   * @brief deleted: Assignment operator
                   *
                   * Here we are in the same situation as with copy construction
                   */
                 COdb & operator = (COdb const & src) = delete;
                 /**
                  * @brief Some cleanup
                  *
                  * If we wish to convience valgrind we are doing it right,
                  * we have to free all links to other objects before leaving
                  * the show.
                  */
	virtual ~COdb() noexcept
                    {
                    clear();
                    }

        /**
         * @brief Frees all objects
         *
         * Frees all known objects at last so far that freeing the
         * object collections releases all memory, bound to them.
         * So valgrind will find nonode left on the table.
         */
        void clear();


        /**
         * @brief Creates a PNode
         *
         * Creates a shared_ptr with a new CNode named as given in the
         * call. If no name is given, the name will be the class default
         *
         * @param crsName The name for the CNode
         */
#ifdef __DOXYGEN__
        PNode MakeNode(std::string const & crsName = "")
#else
        auto MakeNode(std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CNode>(crsName);
            m_oNodes.insert( p );
            return std::move( p );
            }

        /**
         * @brief Creates a PNode with predefined ID
         *
         * Creates a shared_ptr with a new CNode named as given in the
         * call. If no name is given, the name will be the class default
         *
         * @param nId The predefined ID if loading given sets into odb
         * @param crsName The name for the CNode
         */
#ifdef __DOXYGEN__
        PNode LoadNode(size_t nId, std::string const & crsName = "")
#else
        auto LoadNode(size_t nId, std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CNode>(nId, crsName);
            m_oNodes.insert( p );
            return std::move( p );
            }

        /**
         * @brief Creates a PProperty
         *
         * Creates a shared_ptr with a new CProperty named as given in
         * the call. If no name is given, the name will be the class
         * default
         *
         * @param crsName The name for the CProperty
         */
#ifdef __DOXYGEN__
        PProperty MakeProperty(std::string const & crsName = "")
#else
        auto MakeProperty(std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CProperty>(crsName);
            m_oProperties.insert( p );
            return std::move( p );
            }

        /**
         * @brief Creates a PProperty with predefined ID
         *
         * Creates a shared_ptr with a new CProperty named as given in
         * the call. If no name is given, the name will be the class
         * default
         *
         * @param nId The predefined ID if loading given sets into odb
         * @param crsName The name for the CProperty
         */
#ifdef __DOXYGEN__
        PProperty LoadProperty(size_t nId, std::string const & crsName = "")
#else
        auto LoadProperty(size_t nId, std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CProperty>(nId, crsName);
            m_oProperties.insert( p );
            return std::move( p );
            }

        /**
            @brief Creates a PAtom

            Creates a shared_ptr with a new CAtom named as given in the
            call. If no name is given, the name will be the class default

            The data type of the data element in CAtom follows the input
            data type. It can be if any primitive type or most of the
            simple containers, like string or vector.

            CAtom further on manages the life time of the data element.
            It's a unique_ptr

            @param data The data for the CAtom
            @param crsName The name for the CAtom
            @param crsPrefix The prefix for the CAtom in a GUI
            @param crsSuffix The suffix for the CAtom in a GUI
            @param crsFormat The format for the CAtom in a GUI
         */
#ifdef __DOXYGEN__
        PAtom const MakeAtom(
            int data,
            std::string const & crsName   = "",
            std::string const & crsPrefix = "",
            std::string const & crsSuffix = "",
            std::string const & crsFormat = "")
#else
        template<typename T>
        auto const MakeAtom(
            T data,
            std::string const & crsName   = ""s,
            std::string const & crsPrefix = ""s,
            std::string const & crsSuffix = ""s,
            std::string const & crsFormat = ""s)
#endif
            {
            auto p = std::make_shared<CAtom>(data, crsName, crsPrefix, crsSuffix, crsFormat);
            m_oAtoms.insert( p );
            return std::move( p );
            }

        /**
            @brief Creates a PAtom with predefined ID

            Creates a shared_ptr with a new CAtom named as given in the
            call. If no name is given, the name will be the class default

            The data type of the data element in CAtom follows the input
            data type. It can be if any primitive type or most of the
            simple containers, like string or vector.

            CAtom further on manages the life time of the data element.
            It's a unique_ptr

            @param nId The predefined ID if loading given sets into odb
            @param data The data for the CAtom
            @param crsName The name for the CAtom
            @param crsPrefix The prefix for the CAtom in a GUI
            @param crsSuffix The suffix for the CAtom in a GUI
            @param crsFormat The format for the CAtom in a GUI
         */
#ifdef __DOXYGEN__
        PAtom const LoadAtom(
            size_t nId,
            int data,
            std::string const & crsName   = "",
            std::string const & crsPrefix = "",
            std::string const & crsSuffix = "",
            std::string const & crsFormat = "")
#else
        template<typename T>
        auto const LoadAtom(
            size_t nId,
            T data,
            std::string const & crsName   = ""s,
            std::string const & crsPrefix = ""s,
            std::string const & crsSuffix = ""s,
            std::string const & crsFormat = ""s)
#endif
            {
            auto p = std::make_shared<CAtom>(nId, data, crsName, crsPrefix, crsSuffix, crsFormat);
            m_oAtoms.insert( p );
            return std::move( p );
            }

        /**
         * @brief Creates a PReason
         *
         * Creates a shared_ptr with a new CReason named as given in the
         * call. If no name is given, the name will be the class default
         *
         * @param crsName The name for the CReason
         */
#ifdef __DOXYGEN__
         PReason MakeReason(std::string const & crsName = "")
#else
         auto MakeReason(std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CReason>(crsName);
            m_oReasons.insert( p );
            return std::move( p );
            }

        /**
         * @brief Creates a PReason with predefined ID
         *
         * Creates a shared_ptr with a new CReason named as given in the
         * call. If no name is given, the name will be the class default
         *
         * @param nId The predefined ID if loading given sets into odb
         * @param crsName The name for the CReason
         */
#ifdef __DOXYGEN__
        PReason LoadReason(size_t nId, std::string const & crsName = "")
#else
        auto LoadReason(size_t nId, std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CReason>(nId, crsName);
            m_oReasons.insert( p );
            return std::move( p );
            }

        /**
         * @brief Creates a PStrand
         *
         * Creates a shared_ptr with a new CStrand named as given in the
         * call. If no name is given, the name will be the class default
         *
         * @param crsName The name for the CStrand
         */
#ifdef __DOXYGEN__
         PStrand MakeStrand(std::string const & crsName = "")
#else
         auto MakeStrand(std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CStrand>(crsName);
            m_oStrands.push_back( p );
            return std::move( p );
            }

        /**
         * @brief Print out the database (Informative format)
         */
        void print() const
            {
            print(m_oNodes);
            print(m_oAtoms);
            print(m_oProperties);
            print(m_oReasons);
            } // void print()

        /**
         * @brief Print out container of CAtom objects
         *
         * @param crContainer The forward iterable container, containing
         *        PAtom's
         */
        void print(CAtoms const & crContainer) const
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->m_sType << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\t'
                          << " data: " << *e << '\n';
                }
            } // void print(CAtoms const & crContainer)

        /**
         * @brief Print out container of CNode's
         *
         * @param crContainer The forward iterable container, containing
         *        all CNode instances
         */
        template<typename T>
        void print(CT<T> const & crContainer) const
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->m_sType << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\n';
                }
            } // void print(CNodes const & crContainer)


        /// @brief Replaces 2 with \" and \ with \\
	/// @param crsInput The string to be escaped
	auto Escape(std::string const & crsInput)
	    {
	    static std::regex m_oRegexESC{R"(\"|\\)"};
	    return std::regex_replace(crsInput, m_oRegexESC, "\\$&");
	    }

        /**
         * @brief Dump all CNodes in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CNode instances
         * @param ros The output destination
         */
        void print_json(CNodes const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Nodes\": " << '\n';
            ros << spcr<3> << '[' << '\n';
            for (auto const & e:crContainer)
                {
                ros << spcr<4> << "{ ";
//              ros            << "\"type\": \"" << e->type    << "\", ";
                ros            << "\"id\": "     << e->m_nId      << ", ";
                ros            << "\"name\": \"" << Escape(e->m_sName) << "\",\n";

                ros << spcr<5> << "\"properties\": [ ";
                long lc{0};     // Block counter
                bool lb{false}; // Start signal
                for (auto const & a:e->m_spoProperties)
                    {
                    if ( !lb ) { lb=true; ros << ""; } else { ros << ","; }
                    ++lc;
                    if ( lc % 5 == 0 & lc > 1 ) { ros << "\n" << spcr<6> ; }
                    ros << "{\"id\": " << a->m_nId << "}";
                    }
                ros << " ],\n";

                ros << spcr<5> << "\"atoms\": [ ";
                lc = 0;     // Block counter
                lb = false; // Start signal
                for (auto const & a:e->m_spoAtoms)
                    {
                    if ( !lb ) { lb=true; ros << ""; } else { ros << ","; }
                    ++lc;
                    if ( lc % 5 == 0 & lc > 1 ) { ros << "\n" << spcr<6> ; }
                    ros << "{\"id\": " << a->m_nId << "}";
                    }
                ros << " ],\n";

                ros << spcr<5> << "\"links\": [ ";
                lc = 0;     // Block counter
                lb = false; // Start signal
                for (auto const & b:e->m_mLink)
                    {
                    if ( !lb ) { lb=true; ros << ""; } else { ros << ","; }
                    ++lc;
                    if ( lc % 3 == 0 & lc > 1 ) { ros << "\n" << spcr<6>; }
                    ros << "{\"node-id\": " << b.first->m_nId << ", \"reason-id\": " << b.second->m_nId << "}";
                    }
                if ( ++cc < cm ) { ros << " ] },\n"; } else { ros << " ] }\n"; }
                }
            ros << spcr<3> << "]\n";
            } // void print_json(CNodes const & crContainer, std::ostream & ros)

        /**
         * @brief Dump all CNodes in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CNode instances
         * @param ros The output destination
         */
        void print_json_stream(CNodes const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << "\"Nodes\":[";
            for (auto const & e:crContainer)
                {
//              ros << "{\"type\":\"" << e->type  << "\",";
                ros << "{\"id\":"     << e->m_nId << ",";
                ros <<  "\"name\":\"" << Escape(e->m_sName) << "\",";

                ros << "\"properties\":[";
                bool lb{false}; // Start signal
                for (auto const & a:e->m_spoProperties)
                    {
                    if ( !lb ) { lb=true; } else { ros << ","; }
                    ros << "{\"id\":" << a->m_nId << "}";
                    }
                ros << "],";

                ros << "\"atoms\":[";
                lb = false; // Start signal
                for (auto const & a:e->m_spoAtoms)
                    {
                    if ( !lb ) { lb=true; } else { ros << ","; }
                    ros << "{\"id\":" << a->m_nId << "}";
                    }
                ros << "],";

                ros << "\"links\":[";
                lb = false; // Start signal
                for (auto const & b:e->m_mLink)
                    {
                    if ( !lb ) { lb=true; } else { ros << ","; }
                    ros << "{\"node-id\":" << b.first->m_nId << ",\"reason-id\":" << b.second->m_nId << "}";
                    }
                if ( ++cc < cm ) { ros << "]},"; } else { ros << "]}"; }
                }
            ros << "]";
            } // void print_json_stream(CNodes const & crContainer, std::ostream & ros)

        /**
         * @brief Dump all CProperty's in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CProperty instances
         * @param ros The output destination
         */
        void print_json(CProperties const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Properties\": " << '\n';
            ros << spcr<3> << '[' << '\n';
            for ( auto const & a:crContainer )
                {
                ros << spcr<4> << "{ ";
                ros            << "\"id\": "       << a->m_nId        << ", ";
                ros            << "\"name\": \""   << Escape(a->m_sName)  << "\" ";
                if ( ++cc < cm ) { ros << "},\n"; } else { ros << "}\n"; }
                }
            ros << spcr<3> << "],\n";
            } // void print_json(CProperties const & crContainer, std::ostream & ros)

        /**
         * @brief Dump all CProperty's in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CProperty instances
         * @param ros The output destination
         */
        void print_json_stream(CProperties const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << "\"Properties\":[";
            for ( auto const & a:crContainer )
                {
                ros << "{";
                ros << "\"id\":"     << a->m_nId             << ",";
                ros << "\"name\":\"" << Escape(a->m_sName)   << "\"";
                if ( ++cc < cm ) { ros << "},"; } else { ros << "}"; }
                }
            ros << "]";
            } // void print_json_stream(CProperties const & crContainer, std::ostream & ros)

        /**
         * @brief Dump all CAtoms in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CAtom instances
         * @param ros The output destination
         */
        void print_json(CAtoms const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Atoms\": " << '\n';
            ros << spcr<3> << '[' << '\n';
            for ( auto const & a:crContainer )
                {
                                           ros << spcr<4> << "{ ";
//                                         ros << "\"type\": \""   <<  a->type      << "\", ";
                                           ros << "\"id\": "       <<  a->m_nId        << ", ";

                if ( a->m_sName.size() )   ros << "\"name\": \""   <<  Escape(a->m_sName  ) << "\", ";
                if ( a->m_sPrefix.size() ) ros << "\"prefix\": \"" <<  Escape(a->m_sPrefix) << "\", ";
                if ( a->m_sSuffix.size() ) ros << "\"suffix\": \"" <<  Escape(a->m_sSuffix) << "\", ";
                if ( a->m_sFormat.size() ) ros << "\"format\": \"" <<  Escape(a->m_sFormat) << "\", ";

                                           ros << "\"data\": \""   << *a            << "\" ";
                if ( ++cc < cm ) { ros << "},\n"; } else { ros << "}\n"; }
                }
            ros << spcr<3> << "],\n";
            } // void print_json(CAtoms const & crContainer, long const nRI, std::ostream & ros)

        /**
         * @brief Dump all CAtoms in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CAtom instances
         * @param ros The output destination
         */
        void print_json_stream(CAtoms const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << "\"Atoms\":[";
            for ( auto const & a:crContainer )
                {
                                           ros << "{";
//                                         ros << "\"type\":\"" <<  a->type  << "\",";
                                           ros << "\"id\":"     <<  a->m_nId << ",";

                if ( a->m_sName.size() )   ros << "\"name\":\""   <<  Escape(a->m_sName  ) << "\",";
                if ( a->m_sPrefix.size() ) ros << "\"prefix\":\"" <<  Escape(a->m_sPrefix) << "\",";
                if ( a->m_sSuffix.size() ) ros << "\"suffix\":\"" <<  Escape(a->m_sSuffix) << "\",";
                if ( a->m_sFormat.size() ) ros << "\"format\":\"" <<  Escape(a->m_sFormat) << "\",";

                                           ros << "\"data\":\""   << *a << "\"";
                if ( ++cc < cm ) { ros << "},"; } else { ros << "}"; }
                }
            ros << "]";
            } // void print_json_stream(CAtoms const & crContainer, long const nRI, std::ostream & ros)

        /**
         * @brief Dump all CReasons in Sub-JSON format
         * @param crContainer The forward iterable container, containing
         *        all CReason instances
         * @param ros The output destination
         */
        void print_json(CReasons const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Reasons\": " << '\n';
            ros << spcr<3> << '[' << '\n';
            for ( auto const & a:crContainer )
                {
                ros << spcr<4> << "{ ";
                ros            << "\"id\": "       << a->m_nId        << ", ";
                ros            << "\"name\": \""   << Escape(a->m_sName) << "\" ";
                if ( ++cc < cm ) { ros << "},\n"; } else { ros << "}\n"; }
                }
            ros << spcr<3> << "],\n";
            }

        /**
         * @brief Dump all CReasons in Sub-JSON format
         * @param crContainer The forward iterable container, containing
         *        all CReason instances
         * @param ros The output destination
         */
        void print_json_stream(CReasons const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << "\"Reasons\":[";
            for ( auto const & a:crContainer )
                {
                ros << "{\"id\":"      << a->m_nId           << ",";
                ros <<  "\"name\":\""  << Escape(a->m_sName) << "\"";
                if ( ++cc < cm ) { ros << "},"; } else { ros << "}"; }
                }
            ros << "]";
            }


        /**
         * @brief Dump the hole database in JSON format
         *
         * see also: JSON validator+converter: https://jsonformatter.org/
         *
         * @param ros The output destination

 @par Sample: Link Atoms to Nodes and Node to Node and dump it as JSON
 @rst
 .. code-block:: cpp

	 #include <iostream>

	 #include "odb.h"
	 #include "atom.h"
	 #include "node.h"

	 int main()
	     {
	     auto oOdb    = odb::COdb();
	     auto pNode1 = oOdb.MakeNode("Ulrich");
	     auto pNode2 = oOdb.MakeNode("Fred");
	     auto pAtom1  = oOdb.MakeAtom("Leader", "Role");
	     auto pAtom2  = oOdb.MakeAtom("Member", "Role");
	     auto pReason = oOdb.MakeReason("pays");
	     pNode1->Append(pAtom1);
	     pNode2->Append(pAtom2);
	     pNode1->Link(pNode2, pReason);
	     oOdb.print_json(std::cout);
	     }

 Output

 .. code-block:: none

    {
    "Object Database Dump":
        {
        "Sizes": [ {"P": 0},{"A": 2},{"R": 1},{"T": 2} ],
        "Properties":
            [
            ],
        "Atoms":
            [
                { "id": 0, "data": "Leader" },
                { "id": 1, "data": "Member" }
            ],
        "Reasons":
            [
                { "id": 0, "name": "pays" }
            ],
        "Nodes":
            [
                { "id": 0, "name": "Ulrich",
                    "properties": [  ],
                    "atoms": [ {"id": 0} ],
                    "links": [ {"node-id": 1, "reason-id": 0} ] },
                { "id": 1, "name": "Fred",
                    "properties": [  ],
                    "atoms": [ {"id": 1} ],
                    "links": [  ] }
            ]
        }
     }

 @endrst

*/
        void print_json(std::ostream & ros)
            {
            ros << spcr<0> << '{' << '\n';
            ros << spcr<1> << "\"Object Database Dump\": " << '\n';
            ros << spcr<2> << '{' << '\n';

            ros << spcr<2> << "\"Sizes\": [ {\"P\": " <<
            m_oProperties.size() << "},{\"A\": " <<
            m_oAtoms.size() << "},{\"R\": " <<
            m_oReasons.size() << "},{\"T\": " <<
            m_oNodes.size() << "} ]," << '\n';

            print_json(m_oProperties, ros);
            print_json(m_oAtoms,      ros);
            print_json(m_oReasons,    ros);
            print_json(m_oNodes,     ros);

            ros << spcr<2> << '}' << '\n';
            ros << spcr<0> << '}' << '\n';
            }
/*
{
    "Object Database Dump":
        {
        "Sizes": [ {"P": 1089},{"A": 3000},{"R": 10},{"T": 1000} ],
        "Properties": [ { "id": 0, "name": "Person" } ],
        "Atoms": [ { "id": 0, "name": "round", "suffix": "%", "data": "100.2" } ],
        "Reasons": [ { "id": 0, "name": "made" } ],
        "Nodes":
            [
                { "id": 0, "name": "Wundertüte",
                    "properties": [ {"id": 0},{"id": 1} ],
                    "atoms": [ {"id": 13},{"id": 14} ],
                    "links": [ {"node-id": 6, "reason-id": 3} ] }
            ]
        }
}
*/
        /**
         * @brief Dump the hole database in JSON format
         * @param ros The output destination
         */
        void print_json_stream(std::ostream & ros)
            {
            ros << "{\"Object Database Dump\":{" << '\n';

            ros <<            "\"Sizes\":["
		<< "{\"T\":" << m_oNodes.size()     << "},"
		<< "{\"P\":" << m_oProperties.size() << "},"
		<< "{\"R\":" << m_oReasons.size()    << "},"
		<< "{\"A\":" << m_oAtoms.size()      << "}"
		                                     << "],\n";

            print_json_stream(m_oProperties, ros); ros << ",\n";
            print_json_stream(m_oAtoms,      ros); ros << ",\n";
            print_json_stream(m_oReasons,    ros); ros << ",\n";
            print_json_stream(m_oNodes,     ros); ros << "";

            ros << "}}\n";
            }
/*
{"Object Database Dump":{
"Sizes":[{"T":2},{"P":0},{"R":1},{"A":2}],
"Properties":[],
"Atoms":[{"id":0,"name":"Role","data":"Leader"},{"id":1,"name":"Role","data":"Member"}],
"Reasons":[{"id":0,"name":"pays"}],
"Nodes":[{"id":0,"name":"Ulrich","properties":[],"atoms":[{"id":0}],"links":[{"node-id":1,"reason-id":0}]},{"id":1,"name":"Fred","properties":[],"atoms":[{"id":1}],"links":[]}]}}
*/

        /// @brief Saves an odb json file
        /// @param crsFilename
        void SaveDB(std::string const & crsFilename)
            {
            std::fstream db(crsFilename, std::ifstream::out);
            print_json(db);
        //  print_json_stream(db);
            db.close();
            }

        /// @brief Loads an odb json file
        /// @param crsFilename
        bool LoadDB(std::string const & crsFilename)
            {
            auto poJson = std::make_unique<Json::Value>();
            try
                {
                std::ifstream db_dump(crsFilename, std::ifstream::binary);
                db_dump >> *poJson;
                }
            catch (...)
                {
                std::cerr << "db file not found or not readable";
                return false;
                }

            auto const & properties = (*poJson)["Object Database Dump"]["Properties"];
            for ( size_t index = 0; index < properties.size(); ++index )
                {
                auto nId   = properties[(int)index].get("id",      0).asUInt();
                auto sName = properties[(int)index].get("name", "").asString();
                LoadProperty(nId, sName);
                }

            auto const & atoms = (*poJson)["Object Database Dump"]["Atoms"];
            for ( size_t index = 0; index < atoms.size(); ++index )
                {
                auto nId     = atoms[(int)index].get("id",      0).asUInt();
                auto sName   = atoms[(int)index].get("name",   "").asString();
                auto sPrefix = atoms[(int)index].get("prefix", "").asString();
                auto sSuffix = atoms[(int)index].get("suffix", "").asString();
                auto sFormat = atoms[(int)index].get("format", "").asString();
                auto sData   = atoms[(int)index].get("data",   "").asString();
                LoadAtom(nId, sData, sName, sPrefix, sSuffix, sFormat);
                }

            auto const & reasons = (*poJson)["Object Database Dump"]["Reasons"];
            for ( size_t index = 0; index < reasons.size(); ++index )
                {
                auto nId   = reasons[(int)index].get("id",    0).asUInt();
                auto sName = reasons[(int)index].get("name", "").asString();
                LoadReason(nId, sName);
                }

            auto const & nodes = (*poJson)["Object Database Dump"]["Nodes"];
            for ( size_t index = 0; index < nodes.size(); ++index )
                {
                auto nId   = nodes[(int)index].get("id",    0).asUInt();
                auto sName = nodes[(int)index].get("name", "").asString();
                LoadNode(nId, sName);
                }
            for ( size_t index = 0; index < nodes.size(); ++index )
                {
                auto nId   = nodes[(int)index].get("id",    0).asUInt();

                auto const & p = nodes[(int)index]["properties"];
                for ( size_t i = 0; i < p.size(); ++i )
                    {
                    auto nPId = p[(int)i].get("id", 0).asUInt();
                    AppendProperty2Node( nPId, nId );
                    }

                auto const & a = nodes[(int)index]["atoms"];
                for ( size_t i = 0; i < a.size(); ++i )
                    {
                    auto nAId = a[(int)i].get("id", 0).asUInt();
                    AppendAtom2Node( nAId, nId );
                    }

                auto const & l = nodes[(int)index]["links"];
                for ( size_t i = 0; i < l.size(); ++i )
                    {
                    auto nTId = l[(int)i].get("node-id",  0).asUInt();
                    auto nRId = l[(int)i].get("reason-id", 0).asUInt();
                    LinkNode2Node( nId, nRId, nTId );
                    }
                }
            poJson = std::make_unique<Json::Value>();
            return true;
            } // LoadDB(...)    


        /// @brief Has to return a node with specified ID, if it does not exists, it is to make
        /// @param nId The id of the node
        /// @param crsName The name of the node if it has to be created
#ifdef __DOXYGEN__
        PNode FindOrLoadNodeById( size_t const nId, std::string const & crsName = "" )
#else
        auto FindOrLoadNodeById( size_t const nId, std::string const & crsName = ""s )
#endif
            {
            PNode poNode;

            auto const itNode =  m_oNodes.get<id>().find( nId );
            if ( itNode == m_oNodes.get<id>().end() )
                {
                poNode = ( ""s == crsName ) ? LoadNode(nId) : LoadNode(nId, crsName);
                }
            else
                {
                poNode = *itNode;
                }
            return std::move(poNode);
            }


        /// @brief Finds PNode with a named Property only if it's unique
        /// 
        /// @param crsProperty The name for the CProperty
        /// 
        ONode FindNodeByProperty( std::string const & crsProperty )
            {
            auto const itProperty =  m_oProperties.get<name>().find( crsProperty );
            if ( itProperty == m_oProperties.get<name>().end() )
                {
                return std::nullopt;
                }
            PProperty poProperty = *itProperty;
            if ( (0 == poProperty->m_oRelations.size()) && 
                      (poProperty->m_oRelations.size() > 1) )
                {
                return std::nullopt;
                }
            return std::move(*poProperty->m_oRelations.begin());
            }


        /// @brief Finds PNode with a named Property only if it's one
        /// 
        /// @param crsProperty The name for the CProperty
        /// 
        CNodes FindNodesByProperty( std::string const & crsProperty )
            {
            auto oRange = m_oProperties.get<name>().equal_range(crsProperty);
            // todo: preallocation
            CNodes oResult;
            for ( auto it = oRange.first; it != oRange.second; ++it )
                {
                for (auto const & a:(*it)->m_oRelations)
                    {
                    oResult.insert(a);
                    }
                }
            return std::move(oResult);
            } // CNodes FindNodesByProperty( std::string ...


        /// @brief Finds PNode with a named Property only if it's one
        /// 
        /// @param crsRegex The name for the CProperty
        /// 
        CNodes FindNodesByProperty( std::regex const & crsRegex )
            {
        //    std::vector<PProperty> oProperties;
        //    std::copy_if(m_oProperties.begin(),
        //                 m_oProperties.end(),
        //                 std::back_inserter(oProperties), [&](PProperty const & e) {return std::regex_match(e->m_sName, crsRegex);});
 
            CProperties oProperties;
            for ( auto a:m_oProperties )
                {
                if ( std::regex_match(a->m_sName, crsRegex) ) oProperties.insert(a);
                }

            // todo: preallocation
            CNodes oResult;
            for (auto const & b:oProperties)
                {
                for (auto const & a:b->m_oRelations)
                    {
                    oResult.insert(a);
                    }
                }
            return std::move(oResult);
            } // CNodes FindNodesByProperty( std::regex ...


        /// @brief Finds or creates a PNode with a named Property, which also may be created and assigned
        ///
        /// @param crsNode The name for the CNode
        /// @param crsProperty The name for the CProperty
        ///
        PNode FindOrMakeNodeByProperty( std::string const & crsNode, std::string const & crsProperty )
            {
            PProperty poProperty;

            auto const itProperty =  m_oProperties.get<name>().find( crsProperty );
            if ( itProperty == m_oProperties.get<name>().end() )
                {
                poProperty = MakeProperty(crsProperty);
                }
            else
                {
                poProperty = *itProperty;
                }

            PNode poResult;
            if ( poProperty->m_oRelations.size() > 0 )
                {
                poResult = *poProperty->m_oRelations.begin();
                }
            else
                {
                poResult = MakeNode(crsNode);
                poResult->Append(poProperty);
                }
 
            return std::move(poResult);
            } // PNode FindOrMakeNodeByProperty( std::string ...

        MLinkets FindNodeLinkingSameNode( size_t const cnIdNodeA, size_t const cnIdNodeB )
            {
            MLinkets oResult; 

            auto const itNodeA = m_oNodes.get<id>().find( cnIdNodeA );
            if ( itNodeA == m_oNodes.get<id>().end() ) return oResult;
            auto const itNodeB = m_oNodes.get<id>().find( cnIdNodeB );
            if ( itNodeB == m_oNodes.get<id>().end() ) return oResult;


            auto oLinksA = (*itNodeA)->Linkets();
            auto oLinksB = (*itNodeB)->Linkets();

            auto it = std::set_intersection (oLinksA.begin(), oLinksA.end(), oLinksB.begin(), oLinksB.end(),
                                             std::inserter(oResult, oResult.begin()));

            return std::move(oResult);
            }

        /// @brief Has to return a node, if it does not exists, it is to make
        /// @param crsNode The name of the Node
        PNode FindOrMakeNode( std::string const & crsNode )
            {
            PNode poNode;

            auto const itNode =  m_oNodes.get<name>().find(crsNode);
            if ( itNode == m_oNodes.get<name>().end() )
                {
                poNode = std::move(MakeNode(crsNode));
                }
            else
                {
                poNode = *itNode;
                }
            return std::move(poNode);
            }

        /// @brief Has to return a property, if it does not exists, it is to make
        /// @param crsProperty The name of the Property
        PProperty FindOrMakeProperty( std::string const & crsProperty )
            {
            PProperty poProperty;

            auto const itProperty =  m_oProperties.get<name>().find(crsProperty);
            if ( itProperty == m_oProperties.get<name>().end() )
                {
                poProperty = std::move(MakeProperty(crsProperty));
                }
            else
                {
                poProperty = *itProperty;
                }
            return std::move(poProperty);
            }

        /// @brief Has to return a Reason, if it does not exists, it is to make
        /// @param crsReason The name of the Reason
        PReason FindOrMakeReason( std::string const & crsReason )
            {
            PReason poReason;

            auto const itReason =  m_oReasons.get<name>().find(crsReason);
            if ( itReason == m_oReasons.get<name>().end() )
                {
                poReason = std::move(MakeReason(crsReason));
                }
            else
                {
                poReason = *itReason;
                }
            return std::move(poReason);
            }

        /// @brief Has to return an Atom, if it does not exists, it is to make
        /// @param crsAtom The name of the Atom
        PAtom FindOrMakeAtom( std::string const & crsAtom )
            {
            PAtom poAtom;

            auto const itAtom =  m_oAtoms.get<name>().find(crsAtom);
            if ( itAtom == m_oAtoms.get<name>().end() )
                {
                poAtom = std::move(MakeAtom(crsAtom));
                }
            else
                {
                poAtom = *itAtom;
                }
            return std::move(poAtom);
            }

        /// todo: optimize / Appends a Property to a Node by given index value
        bool AppendProperty2Node( size_t nProperty, size_t nNode)
            {
            auto const itProperty = m_oProperties.get<id>().find( nProperty );
            auto itNode    = m_oNodes.get<id>().find( nNode );

            if ( (itNode == m_oNodes.end()) || (itProperty == m_oProperties.end()) )
                {
                return false;
                }
            PProperty poProperty = *itProperty;
            (*itNode)->Append( poProperty );
            return true;
            }

        /// todo: optimize / Appends a Property to a Node by given names
        bool AppendProperty2Node( std::string const & crsProperty, std::string const & crsNode )
            {
            return AppendProperty2Node( crsProperty, false, crsNode );
            }

        /// todo: optimize / Appends a Property to a Node by given names
        bool AppendProperty2Node( std::string const & crsProperty, bool const bForce, std::string const & crsNode )
            {
            auto itNodes =  m_oNodes.get<name>().find( crsNode );
            if ( itNodes == m_oNodes.get<name>().end() )
                {
                return false;
                }

            PProperty poProperty;

            auto itProperty =  m_oProperties.get<name>().find( crsProperty );
            if ( itProperty == m_oProperties.get<name>().end() )
                {
                if ( !bForce ) return false;
                poProperty = MakeProperty(crsProperty);
                }
            else
                {
                poProperty = *itProperty;
                }

            (*itNodes)->Append( poProperty );

            return true;
            }

        /// todo: optimize / Appends an Atom to a Node by given index value
        bool AppendAtom2Node( size_t nNode, size_t nAtom )
            {
            if ( (nNode >  m_oNodes.size()) || (nAtom > m_oAtoms.size()) ) return false;
            auto itNode =  m_oNodes.get<id>().find( nNode );
            if ( itNode == m_oNodes.end() ) return false;
            auto itAtom  =  m_oAtoms.get<id>().find( nAtom );
            if ( itAtom  == m_oAtoms.end() ) return false;

            (*itNode)->Append( const_cast<PAtom&>(*itAtom) );
            return true;
            }

        /// todo: optimize / Links a Node to a Node for a Reason by given index value
        bool LinkNode2Node( size_t nNodeFrom, size_t nReason, size_t nNodeTo )
            {
            auto itNodeFrom  =  m_oNodes  .get<id>().find( nNodeFrom );
            auto itNodeTo    =  m_oNodes  .get<id>().find( nNodeTo );
            auto itReason    =  m_oReasons.get<id>().find( nReason );
            if ( (itNodeFrom == m_oNodes  .end()) || (itNodeTo == m_oNodes.end()) || (itReason == m_oReasons.end()) )
                {
//              std::cout << "0 " << (*itNodeFrom)->m_sName << " " << (*itReason)->m_sName << " " << (*itNodeTo)->m_sName << '\n';
/*
                std::cout << "0 ";
                if (itNodeFrom == m_oNodes. end()) std::cout << "E:" << nNodeFrom << " "; else std::cout << (*itNodeFrom)->m_sName << " ";
                if (itReason    == m_oReasons.end()) std::cout << "E:" << nReason    << " "; else std::cout << (*itReason)->m_sName    << " ";
                if (itNodeTo   == m_oNodes. end()) std::cout << "E:" << nNodeTo   << " "; else std::cout << (*itNodeTo)->m_sName   << " ";
                std::cout << '\n';
*/
                return false;
                }
//          std::cout << "true LinkNode2Node( size_t " << nNodeFrom << ", size_t " << nReason << ", size_t " << nNodeTo << " )\n";
//          std::cout << "+ " << (*itNodeFrom)->m_sName << " " << (*itReason)->m_sName << " " << (*itNodeTo)->m_sName << '\n';
            (*itNodeFrom)->Link( const_cast<PNode&>(*itNodeTo), const_cast<PReason&>(*itReason) );
//          Link( *itNodeFrom, *itReason, *itNodeTo );
            return true;
            }

        /// todo: optimize / Links a Node to a Node for a Reason by given names
        bool LinkNode2Node( std::string const & crsNodeFrom, std::string const & crsReason, std::string const & crsNodeTo )
            {
            auto itNodeFrom  =  m_oNodes .get<name>().find( crsNodeFrom );
            auto itNodeTo    =  m_oNodes .get<name>().find( crsNodeTo );
            auto itReason    =  m_oReasons.get<name>().find( crsReason );
            if ( (itNodeFrom == m_oNodes.get<name>().end()) || (itNodeTo == m_oNodes.get<name>().end()) || (itReason == m_oReasons.get<name>().end()) )
                {
/*
                std::cout << "0 ";
                if (itNodeFrom == m_oNodes.  get<name>().end()) std::cout << "E:" << crsNodeFrom << " "; else std::cout << (*itNodeFrom)->m_sName << " ";
                if (itReason   == m_oReasons.get<name>().end()) std::cout << "E:" << crsReason    << " "; else std::cout << (*itReason)->m_sName    << " ";
                if (itNodeTo   == m_oNodes.  get<name>().end()) std::cout << "E:" << crsNodeTo   << " "; else std::cout << (*itNodeTo)->m_sName   << " ";
                std::cout << '\n';
*/
                return false;
                }
//          std::cout << "true LinkNode2Node( size_t " << nNodeFrom << ", size_t " << nReason << ", size_t " << nNodeTo << " )\n";
//          std::cout << "+ " << (*itNodeFrom)->m_sName << " " << (*itReason)->m_sName << " " << (*itNodeTo)->m_sName << '\n';
            (*itNodeFrom)->Link( const_cast<PNode&>(*itNodeTo), const_cast<PReason&>(*itReason) );
//          Link( *itNodeFrom, *itReason, *itNodeTo );
            return true;
            }

        /**
         * @brief Finds the T with ID nId
         *
         * @tparam T the type of the filtered objects
         * @param croContainer The container to be filtered
         * @param nId The ID of the T
         */
        template<typename T>
        std::optional<PT<T>> Find( CT<T> const & croContainer, size_t nId )
            {
            auto oResult =  croContainer.find(nId);
            if ( oResult == croContainer.end() ) return std::nullopt;
            return std::move( *oResult );
            }

    protected:
        /**
         * @brief Finds all Ts with the given NAME
         *
         * @tparam T the type of the filtered objects
         * @param croContainer The container to be filtered
         * @param crsName The NAME of the Reasons
         */
        template<typename T>
        CT<T> Find( CT<T> const & croContainer, std::string const & crsName )
            {
            auto oRange = croContainer.template get<name>().equal_range(crsName);

            // todo: preallocation
            CT<T> oResult;
            for ( auto it = oRange.first; it != oRange.second; ++it )
                {
                oResult.insert(*it);
                }
            return std::move(oResult);
            }

        /**
         * @brief Finds all Ts with the given NAME as RegEx
         *
         * @tparam T the type of the filtered objects
         * @param croContainer The container to be filtered
         * @param crsRegex The NAME of the Ts in Regex
         */
        template<typename T>
        auto Find( CT<T> const & croContainer, std::regex const & crsRegex )
            {
            // todo: preallocation
            CT<T> oResult;
            for ( auto a:croContainer )
                {
                if ( std::regex_match(a->m_sName, crsRegex) ) oResult.insert(a);
                }
            return std::move(oResult);
            }
    public:
        /// API Adapter
        auto FindNode     ( size_t                  nId ) { return Find(m_oNodes, nId );}
        /// API Adapter
        auto FindNodes    ( std::string const & crsName ) { return Find(m_oNodes, crsName ); }
        /// API Adapter
        auto FindNodes    ( std::regex const & crsRegex ) { return Find(m_oNodes, crsRegex); }

        /// API Adapter
        auto FindProperty  ( size_t                  nId ) { return Find(m_oProperties, nId     ); }
        /// API Adapter
        auto FindProperties( std::string const & crsName ) { return Find(m_oProperties, crsName ); }
        /// API Adapter
        auto FindProperties( std::regex const & crsRegex ) { return Find(m_oProperties, crsRegex); }

        /// API Adapter
        auto FindReason    ( size_t                  nId ) { return Find(m_oReasons, nId     ); }
        /// API Adapter
        auto FindReasons   ( std::string const & crsName ) { return Find(m_oReasons, crsName ); }
        /// API Adapter
        auto FindReasons   ( std::regex const & crsRegex ) { return Find(m_oReasons, crsRegex); }

        /// API Adapter
        auto FindAtom      ( size_t                  nId ) { return Find(m_oAtoms, nId     ); }
        /// API Adapter
        auto FindAtoms     ( std::string const & crsName ) { return Find(m_oAtoms, crsName ); }
        /// API Adapter
        auto FindAtoms     ( std::regex const & crsRegex ) { return Find(m_oAtoms, crsRegex); }


/*
        /// Result container of collecting operations, collecting IDs
        using CAggregate = std::set<size_t>;

        /// todo: optimize / Selects Node-IDs by a Property
        /// Returns the IDs of all nodes having a property as RegExNamed
        CAggregate SelectNodesByProperty( std::regex const & croProperty )
            {
            CAggregate result{};

            std::vector<PProperty> oSelection;
            auto itSelection = std::copy_if(m_oProperties.begin(),
                                            m_oProperties.end(),

//          shrink_to_fit();

            for ( auto const & s:oSelection )
                {
                for ( auto const & a:s->m_oRelations )
                    {
                    result.insert(a->m_nId);
                    }
                }
            return std::move(result);
            }

*/
    protected:
        /**
         * @brief finds unused elements in the given collection
         *
         * @tparam T The type the container hels elements of T The type the container hels elements of
         *
         */
        template<typename T>
        CT<T> FindUnUsed( CT<T> const & croContainer )
            {
            // todo: preallocation
            CT<T> oResult;
            for ( auto const & a:croContainer )
                {
                if ( a->IsUnUsed() ) oResult.insert(a);
                }
            return std::move(oResult);
            }
    public:
	/// find all nodes not linked with anynode
        auto FindUnUsedNodes()
	    {
	    return FindUnUsed( m_oNodes );
	    }
	/// find all reasons not used with anynode
	auto FindUnUsedReasons()
	    {
	    return FindUnUsed( m_oReasons );
	    }
	/// find all properties not linked to anynode
	auto FindUnUsedProperties()
	    {
	    return FindUnUsed( m_oProperties );
	    }
	/// find all atoms not linked to anynode
	auto FindUnUsedAtoms()
	    {
	    return FindUnUsed( m_oAtoms );
	    }



        /// Access function to call then container of CNode's
        auto const & Nodes() const { return m_oNodes;  }
        /// Access function to call then container of CProperties
        auto const & Properties() const { return m_oProperties; }
        /// Access function to call then container of CAtom's
        auto const & Atoms() const { return m_oAtoms;   }
        /// Access function to call then container of CReason's
        auto const & Reasons() const { return m_oReasons; }
        /// Access function to call then container of CStrand's
        auto const & Strands() const { return m_oStrands; }
    protected:
        /// A container instance of CNode's
        CNodes      m_oNodes;
        /// A container instance of CProperties
        CProperties  m_oProperties;
        /// A container instance of CAtom's
        CAtoms       m_oAtoms;
        /// A container instance of CReason's
        CReasons     m_oReasons;
        /// A container instance of CStrand's
        CStrands     m_oStrands;

    }; // class COdb

} // namespace odb

// CODB_H


/*
template<typename CompatibleKey> std::pair<iterator,iterator> equal_range(const CompatibleKey& x)const;

Requires: CompatibleKey is a compatible key of key_compare.
Effects: Equivalent to make_pair(lower_bound(x),upper_bound(x)).
Complexity: O(log(n)).



template<typename CompatibleKey,typename CompatibleCompare> std::pair<iterator,iterator> equal_range(const CompatibleKey& x,const CompatibleCompare& comp)const;

Requires: (CompatibleKey, CompatibleCompare) is a compatible extension of key_compare.
Effects: Equivalent to make_pair(lower_bound(x,comp),upper_bound(x,comp)).
Complexity: O(log(n)).



template<typename Modifier> bool modify(iterator position,Modifier mod);

Requires: mod is a unary function object accepting arguments of type value_type&. position is a valid dereferenceable iterator of the index. The execution of mod(e), where e is the element pointed to by position, does not invoke any operation of the multi_index_container after e is directly modified or, before modification, if the operation would invalidate position.
Effects: Calls mod(e) where e is the element pointed to by position and rearranges *position into all the indices of the multi_index_container. Rearrangement is successful if
the index is non-unique OR no other element exists with equivalent key,
AND rearrangement is allowed by all other indices of the multi_index_container.
If the rearrangement fails, the element is erased.
Postconditions: Validity of position is preserved if the operation succeeds. If the key of the modified value is equivalent to that of the original value, the position of the element does not change.
Returns: true if the operation succeeded, false otherwise.
Complexity: O(M(n)).
Exception safety: Basic. If an exception is thrown by some user-provided operation (except possibly mod), then the element pointed to by position is erased.
*/

#endif

