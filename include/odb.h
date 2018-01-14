#ifndef CODB_H
#define CODB_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <string>
#include <memory>    // shared_ptr
#include <iostream>  // cout

#include "generic.h"

#include "atom.h"
#include "thing.h"
#include "reason.h"
#include "strand.h"


namespace odb {


/**
 * The database
 */
class COdb : public Identifiable<COdb>
    {
    public:
                 COdb() = default;
                 COdb(COdb const & src) = delete;
                 COdb & operator = (COdb const & src) = delete;
        virtual ~COdb()
                    {
                    clear();
                    }

        void clear();

	    auto MakeThing(std::string const & crsName = ""s)
            {
            auto p = std::make_shared<CThing>(crsName);
            m_oThings.push_back( p );
            return std::move( p );
            }

        template<typename T>
        auto const MakeAtom(
            T data,
            std::string const & crsName   = ""s,
            std::string const & crsPrefix = ""s,
            std::string const & crsSuffix = ""s,
            std::string const & crsFormat = ""s)
            {
            auto p = std::make_shared<CAtom>(data, crsName, crsPrefix, crsSuffix, crsFormat);
            m_oAtoms.push_back( p );
            return std::move( p );
            }

        auto MakeReason(std::string const & crsName = ""s)
            {
            auto p = std::make_shared<CReason>(crsName);
            m_oReasons.push_back( p );
            return std::move( p );
            }

        auto MakeStrand(std::string const & crsName = ""s)
            {
            auto p = std::make_shared<CStrand>(crsName);
            m_oStrands.push_back( p );
            return std::move( p );
            }

	    void print()
            {
            print(m_oThings);
            print(m_oAtoms);
            print(m_oReasons);
            } // void print()

        void print(CAtoms const & crContainer)
            {
            for (auto && e : crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->id << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\t'
                          << " data: " << *e << '\n';
                }
            } // void print(CAtoms const & crContainer)

        template<typename T>
        void print(std::deque<T> const & crContainer)
            {
            for (auto const & e:crContainer)
                {
                std::cout << e->type << '\t' << " id: " << e->id << '\t'
                          << " name: " << e->m_sName << '\t'
                          << '(' << e.use_count() << ')' << '\n';
                }
            } // void print(std::deque<T> const & crContainer)



        /// JSON validator+converter: https://jsonformatter.org/

        void print_json(CThings const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Objects\": " << '\n';
            ros << spcr<3> << '[' << '\n';
            for (auto const & e:crContainer)
                {
                ros << spcr<4> << "{ ";
//              ros            << "\"type\": \"" << e->type    << "\", ";
                ros            << "\"id\": \""   << e->id      << "\", ";
                ros            << "\"name\": \"" << e->m_sName << "\",\n";
                ros << spcr<5> << "\"atoms\": [ ";
                long lc{0};     // Block counter
                bool lb{false}; // Start signal
                for (auto const & a:e->m_spoAtoms)
                    {
                    if ( !lb ) { lb=true; ros << ""; } else { ros << ","; }
                    ++lc;
                    if ( lc % 5 == 0 & lc > 1 ) { ros << "\n" << spcr<6> ; }
                    ros << "{\"id\": \"" << a->id << "\"}";
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
                    ros << "{\"thing-id\": \"" << b.first->id << "\", \"reason-id\": \"" << b.second->id << "\"}";
                    }
                if ( ++cc < cm ) { ros << " ] },\n"; } else { ros << " ] }\n"; }
                }
            ros << spcr<3> << "],\n";
            } // void print_json(CThings const & crContainer, long const nRI, std::ostream & ros)

// Atoms

        void print_json(CAtoms const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Atoms\": " << '\n';
            ros << spcr<3> << '[' << '\n';
            for ( auto const & a:crContainer )
                {
                ros << spcr<4> << "{ ";
//              ros            << "\"type\": \""   << a->type      << "\", ";
                ros            << "\"id\": \""     <<  a->id        << "\", ";
                ros            << "\"name\": \""   <<  a->m_sName   << "\", ";
                ros            << "\"prefix\": \"" <<  a->m_sPrefix << "\", ";
                ros            << "\"suffix\": \"" <<  a->m_sSuffix << "\", ";
                ros            << "\"format\": \"" <<  a->m_sFormat << "\", ";
                ros            << "\"data\": \""   << *a            << "\" ";
                if ( ++cc < cm ) { ros << "},\n"; } else { ros << "}\n"; }
                }
            ros << spcr<3> << "],\n";
            } // void print_json(CAtoms const & crContainer, long const nRI, std::ostream & ros)

// Reasons
        void print_json(CReasons const & crContainer, std::ostream & ros)
            {
            std::size_t cm{crContainer.size()};
            std::size_t cc{0};
            ros << spcr<2> << "\"Reasons\": " << '\n';
            ros << spcr<3> << '[' << '\n';
            for ( auto const & a:crContainer )
                {
                ros << spcr<4> << "{ ";
//              ros            << "\"type\": \"" << a->type      << "\", ";
                ros            << "\"id\": \""     << a->id        << "\", ";
                ros            << "\"name\": \""   << a->m_sName   << "\" ";
                if ( ++cc < cm ) { ros << "},\n"; } else { ros << "}\n"; }
                }
            ros << spcr<3> << ']' << '\n';
            }

        void print_json(std::ostream & ros)
            {
            ros << spcr<0> << '{' << '\n';
            ros << spcr<1> << "\"Object Database Dump\": " << '\n';
            ros << spcr<2> << '{' << '\n';

            print_json(m_oThings,  ros);
            print_json(m_oAtoms,   ros);
            print_json(m_oReasons, ros);

            ros << spcr<2> << '}' << '\n';
            ros << spcr<1> << '}' << '\n';
            ros << spcr<0> << '}' << '\n';
          }

        CThings  const & Things () const { return m_oThings;  }
        CAtoms   const & Atoms  () const { return m_oAtoms;   }
        CReasons const & Reasons() const { return m_oReasons; }
        CStrands const & Strands() const { return m_oStrands; }
    protected:
        CThings  m_oThings;
        CAtoms   m_oAtoms;
        CReasons m_oReasons;
        CStrands m_oStrands;

    }; // class COdb

} // namespace odb

// CODB_H
#endif
