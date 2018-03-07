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

#include "generic.h"

#include "atom.h"
#include "thing.h"
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
class COdb : public Identifiable<COdb>
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
        virtual ~COdb()
                    {
                    clear();
                    }

        /**
         * @brief Frees all objects
         *
         * Frees all known objects at last so far that freeing the
         * object collections releases all memory, bound to them.
         * So valgrind will find nothing left on the table.
         */
        void clear();

        /**
         * @brief Creates a PThing
         *
         * Creates a shared_ptr with a new CThing named as given in the
         * call. If no name is given, the name will be the class default
         *
         * @param crsName The name for the CThing
         */
#ifdef __DOXYGEN__
        PThing MakeThing(std::string const & crsName = "")
#else
        auto MakeThing(std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CThing>(crsName);
            m_oThings.insert( p );
            return std::move( p );
            }

        /**
         * @brief Creates a PThing with predefined ID
         *
         * Creates a shared_ptr with a new CThing named as given in the
         * call. If no name is given, the name will be the class default
         * 
         * @param nId The predefined ID if loading given sets into odb
         * @param crsName The name for the CThing
         */
#ifdef __DOXYGEN__
        PThing LoadThing(size_t nId, std::string const & crsName = "")
#else
        auto LoadThing(size_t nId, std::string const & crsName = ""s)
#endif
            {
            auto p = std::make_shared<CThing>(nId, crsName);
            m_oThings.insert( p );
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
            m_oAtoms.push_back( p );
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
            m_oAtoms.push_back( p );
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
        void print()
            {
            print(m_oThings);
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
        void print(CAtoms const & crContainer)
            {
            for (auto && e : crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\t'
                          << " data: " << *e << '\n';
                }
            } // void print(CAtoms const & crContainer)

        /**
         * @brief Print out container of identifiable objects
         *
         * @tparam T A shared_ptr of an 'Identifiable' type, enriched with
         *         a memberm_sName
         * @param  crContainer The forward iterable container, containing
         *         all T instances
         */
        template<typename T>
        void print(std::deque<T> const & crContainer)
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\n';
                }
            } // void print(std::deque<T> const & crContainer)

        /**
         * @brief Print out container of CThing's
         *
         * @param crContainer The forward iterable container, containing
         *        all CThing instances
         */
        void print(CThings const & crContainer)
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\n';
                }
            } // void print(CThings const & crContainer)

        /**
         * @brief Print out container of CProperty's
         *
         * @param crContainer The forward iterable container, containing
         *        all CProperty instances
         */
        void print(CProperties const & crContainer)
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\n';
                }
            } // void print(CProperties const & crContainer)

        /**
         * @brief Print out container of CReason's
         *
         * @param crContainer The forward iterable container, containing
         *        all CReason instances
         */
        void print(CReasons const & crContainer)
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\n';
                }
            } // void print(CReasons const & crContainer)

        /**
         * @brief Print out container of T's
         *
         * @param crContainer The forward iterable container, containing
         *        all T instances
         */
        template<typename T>
        void print(std::set<T, lessIdentifiableName<T>> const & crContainer)
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->m_nId << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\n';
                }
            } // void print(std::set<T> const & crContainer)

        /// @brief Replaces 2 with \" and \ with \\
	/// @param crsInput The string to be escaped
	auto Escape(std::string const & crsInput)
	    {
	    static std::regex m_oRegexESC{R"(\"|\\)"};
	    return std::regex_replace(crsInput, m_oRegexESC, "\\$&");
	    }

        /**
         * @brief Dump all CThings in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CThing instances
         * @param ros The output destination
         */
        void print_json(CThings const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Things\": " << '\n';
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
                    ros << "{\"thing-id\": " << b.first->m_nId << ", \"reason-id\": " << b.second->m_nId << "}";
                    }
                if ( ++cc < cm ) { ros << " ] },\n"; } else { ros << " ] }\n"; }
                }
            ros << spcr<3> << "]\n";
            } // void print_json(CThings const & crContainer, std::ostream & ros)

        /**
         * @brief Dump all CThings in Sub-JSON format
         *
         * @param crContainer The forward iterable container, containing
         *        all CThing instances
         * @param ros The output destination
         */
        void print_json_stream(CThings const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << "\"Things\":[";
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
                    ros << "{\"thing-id\":" << b.first->m_nId << ",\"reason-id\":" << b.second->m_nId << "}";
                    }
                if ( ++cc < cm ) { ros << "]},"; } else { ros << "]}"; }
                }
            ros << "]";
            } // void print_json_stream(CThings const & crContainer, std::ostream & ros)

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

 @par Sample: Link Atoms to Things and Thing to Thing and dump it as JSON
 @rst
 .. code-block:: cpp
 
	 #include <iostream>

	 #include "odb.h"
	 #include "atom.h"
	 #include "thing.h"

	 int main()
		{
		auto oOdb    = odb::COdb();
		auto pThing1 = oOdb.MakeThing("Ulrich");
		auto pThing2 = oOdb.MakeThing("Fred");
		auto pAtom1  = oOdb.MakeAtom("Leader", "Role");
		auto pAtom2  = oOdb.MakeAtom("Member", "Role");
		auto pReason = oOdb.MakeReason("pays");
		pThing1->Append(pAtom1);
		pThing2->Append(pAtom2);
		pThing1->Link(pThing2, pReason);
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
			"Things": 
				[
					{ "id": 0, "name": "Ulrich",
						"properties": [  ],
						"atoms": [ {"id": 0} ],
						"links": [ {"thing-id": 1, "reason-id": 0} ] },
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
            m_oThings.size() << "} ]," << '\n';

            print_json(m_oProperties, ros);
            print_json(m_oAtoms,      ros);
            print_json(m_oReasons,    ros);
            print_json(m_oThings,     ros);

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
        "Things": 
            [
                { "id": 0, "name": "Wundertüte",
                    "properties": [ {"id": 0},{"id": 1} ],
                    "atoms": [ {"id": 13},{"id": 14} ],
                    "links": [ {"thing-id": 6, "reason-id": 3} ] }
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
		<< "{\"T\":" << m_oThings.size()     << "},"
		<< "{\"P\":" << m_oProperties.size() << "},"
		<< "{\"R\":" << m_oReasons.size()    << "},"
		<< "{\"A\":" << m_oAtoms.size()      << "}"
		                                     << "],\n";

            print_json_stream(m_oProperties, ros); ros << ",\n";
            print_json_stream(m_oAtoms,      ros); ros << ",\n";
            print_json_stream(m_oReasons,    ros); ros << ",\n";
            print_json_stream(m_oThings,     ros); ros << "";

            ros << "}}\n";
            }
/*
{"Object Database Dump":{
"Sizes":[{"T":2},{"P":0},{"R":1},{"A":2}],
"Properties":[],
"Atoms":[{"id":0,"name":"Role","data":"Leader"},{"id":1,"name":"Role","data":"Member"}],
"Reasons":[{"id":0,"name":"pays"}],
"Things":[{"id":0,"name":"Ulrich","properties":[],"atoms":[{"id":0}],"links":[{"thing-id":1,"reason-id":0}]},{"id":1,"name":"Fred","properties":[],"atoms":[{"id":1}],"links":[]}]}}
*/

        /// @brief Has to return a thing with specified ID, if it does not exists, it is to make
        /// @param nId The id of the thing
        /// @param crsName The name of the thing if it has to be created
#ifdef __DOXYGEN__
        PThing FindOrLoadThingById( size_t nId, std::string const & crsName = "" )
#else
        PThing FindOrLoadThingById( size_t nId, std::string const & crsName = ""s )
#endif
            {
            PThing poThing;

            auto itThing =  m_oThings.get<id>().find( nId );
            if ( itThing == m_oThings.get<id>().end() )
                {
                if ( ""s == crsName ) poThing = LoadThing(nId); else poThing = LoadThing(nId, crsName);
                }
            else
                {
                poThing = *itThing;
                }
            return poThing;
            }

        /**
         * @brief Finds or creates a PThing with a named Property, which also may be created and assigned
         *
         * @param crsThing The name for the CThing
         * @param crsProperty The name for the CProperty
         */
        PThing FindOrMakeThingByProperty( std::string const & crsThing, std::string const & crsProperty )
            {
            PThing    poResult;
            PProperty poProperty;

            auto itProperty =  m_oProperties.get<name>().find( crsProperty );
            if ( itProperty == m_oProperties.get<name>().end() )
                {
                poProperty = MakeProperty(crsProperty);
                }
            else
                {
                poProperty = *itProperty;
                }

            if ( poProperty->m_oRelations.size() > 0 )
                {
                poResult = *poProperty->m_oRelations.begin();
                }
            else
                {
            poResult = MakeThing(crsThing);
            poResult->Append(poProperty);
            }

            return poResult;
            }

        /// @brief Has to return a property, if it does not exists, it is to make
        /// @param crsProperty The name of the Property
        PProperty & FindOrMakeProperty( std::string const & crsProperty )
            {
            static PProperty poProperty;
            auto itProperty =  m_oProperties.get<name>().find(crsProperty);
            if ( itProperty == m_oProperties.get<name>().end() )
                {
                poProperty = std::move(MakeProperty(crsProperty));
                }
            else
                {
                poProperty = *itProperty;
                }
            return poProperty;
        }

        /// todo: optimize / Appends a Property to a Thing by given index value
        bool AppendProperty2Thing( size_t nProperty, size_t nThing)
            {
            auto itProperty = m_oProperties.get<id>().find( nProperty );
            auto itThing    = m_oThings.get<id>().find( nThing );

            if ( (itThing == m_oThings.end()) || (itProperty == m_oProperties.end()) )
                {
//              std::cout << "FALSE AppendProperty2Thing( size_t "<< nProperty <<", size_t " << nThing << " )" << '\n';
                return false;
                }
//          std::cout << "true AppendProperty2Thing( size_t "<< nProperty <<", size_t " << nThing << " )" << '\n';
            PProperty poProperty = *itProperty;
            (*itThing)->Append( poProperty );
            return true;
            }

        /// todo: optimize / Appends a Property to a Thing by given names
        bool AppendProperty2Thing( std::string const & crsProperty, bool bForce, std::string const & crsThing )
            {
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

            for ( auto & a:m_oThings )
                {
                if ( a->m_sName == crsThing ) a->Append( poProperty );
                }
            return true;
            }

        /// todo: optimize / Appends an Atom to a Thing by given index value
        bool AppendAtom2Thing( size_t nThing, size_t nAtom )
            {
            if ( (nThing > m_oThings.size()) || (nAtom > m_oAtoms.size()) ) return false;
            auto itThing = std::find_if(m_oThings.begin(), m_oThings.end(), [&](PThing const & e){return e->m_nId == nThing;});
            if ( itThing == m_oThings.end() ) return false;
            auto itAtom  = std::find_if(m_oAtoms.begin(),  m_oAtoms.end(),  [&](PAtom  const & e){return e->m_nId == nAtom;});
            if ( itAtom  == m_oAtoms.end() ) return false;

            (*itThing)->Append( *itAtom );
            return true;
            }

        /// todo: optimize / Links a Thing to a Thing for a Reason by given index value
        bool LinkThing2Thing( size_t nThingFrom, size_t nThingTo, size_t nReason )
            {
            auto itThingFrom  =  m_oThings.get<id>().find( nThingFrom );
            auto itThingTo    =  m_oThings.get<id>().find( nThingTo );
            auto itReason     =  m_oReasons.get<id>().find( nReason );
            if ( (itThingFrom == m_oThings.end()) || (itThingTo == m_oThings.end()) || (itReason == m_oReasons.end()) )
                {
                std::cout << "0 " << (*itThingFrom)->m_sName << " " << (*itReason)->m_sName << " " << (*itThingTo)->m_sName << '\n';
                return false;
                }
//          std::cout << "true LinkThing2Thing( size_t " << nThingFrom << ", size_t " << nThingTo << ", size_t " << nReason << " )\n";
//          std::cout << "+ " << (*itThingFrom)->m_sName << " " << (*itReason)->m_sName << " " << (*itThingTo)->m_sName << '\n';
            (*itThingFrom)->Link( const_cast<PThing&>(*itThingTo), const_cast<PReason&>(*itReason) );
//          Link( *itThingFrom, *itReason, *itThingTo );
            return true;
            }

        //
        // =================== Search operations =======================
        //

        /// Result container of collecting operations, collecting IDs
        using CAggregate = std::set<size_t>;

        /// todo: optimize / Selects Thing-IDs by a Property 
        CAggregate SelectThingsByProperty( std::string const & crsProperty )
            {
            CAggregate result{};

            std::regex crsRegex(crsProperty);
            std::vector<PProperty> oSelection(m_oProperties.size());
            auto itSelection = std::copy_if(m_oProperties.begin(),
                                            m_oProperties.end(),
//                                             oSelection.begin(), [&](PProperty const & e) {return e->m_sName == crsProperty;});
                                               oSelection.begin(), [&](PProperty const & e) {return std::regex_match(e->m_sName, crsRegex);});
            oSelection.resize(std::distance(oSelection.begin(), itSelection));

            for ( auto const & s:oSelection )
                {
                for ( auto const & a:s->m_oRelations )
                    {
                    result.insert(a->m_nId);
                    }
                }
            return std::move(result);
            }

        /// Access function to call then container of CThing's
        CThings      const & Things () const { return m_oThings;  }
        /// Access function to call then container of CProperties
        CProperties  const & Properties () const { return m_oProperties; }
        /// Access function to call then container of CAtom's
        CAtoms       const & Atoms  () const { return m_oAtoms;   }
        /// Access function to call then container of CReason's
        CReasons     const & Reasons() const { return m_oReasons; }
        /// Access function to call then container of CStrand's
        CStrands     const & Strands() const { return m_oStrands; }
    protected:
        /// A container instance of CThing's
        CThings      m_oThings;
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
#endif
