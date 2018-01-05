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


namespace odb {

/**
 * The database
 */
class COdb : public Identifiable<COdb>
    {
    public:
                 COdb(){};
        virtual ~COdb(){};
                 COdb(COdb const & src)
                   {
                   *this = src;
                   }

                 COdb & operator = (COdb const & src)
                   {
                   // todo: Implement DB copying or prevent it
                   }

	    auto MakeThing(std::string const & crsName = ""s)
          {
          auto p = std::make_shared<odb::CThing>(crsName);
          m_oThings.push_back( p );
          return p;
          }

        template<typename T>
        auto const MakeAtom(
          T data,
          std::string const & crsName   = ""s,
          std::string const & crsPrefix = ""s,
          std::string const & crsSuffix = ""s,
          std::string const & crsFormat = ""s)
          {
          auto p = std::make_shared<odb::CAtom>(data, crsName, crsPrefix, crsSuffix, crsFormat);
	      m_oAtoms.push_back( p );
          return p;
          }

        auto MakeReason(std::string const & crsName = ""s)
          {
          auto p = std::make_shared<odb::CReason>(crsName);
          m_oReasons.push_back( p );
          return p;
          }

	    void print()
          {
          print(m_oThings);
          print(m_oAtoms);
          print(m_oReasons);
          }

        void print(CAtoms const & container)
          {
          for (auto && e : container)
            {
            std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << '\n';
            }
          }

        template<typename T>
        void print(std::deque<T> const & container)
          {
          for (auto const & e:container)
            {
            std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\n';
            }
          }


        static long g_nJsonIndent;

        void print_json(CThings const & container, long const nRI, std::ostream & ros)
          {
          std::size_t cm{container.size()};
          std::size_t cc{0};
          for (auto const & e:container)
            {
            ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "{ ";
//            ros                                            << "\"type\": \"" << e->type      << "\", ";
            ros                                            << "\"id\": \""   << e->id        << "\", ";
            ros                                            << "\"name\": \"" << e->NameGet() << "\",\n";

            ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "\"atoms\": [ ";
            long lc{0};     // Block counter
            bool lb{false}; // Start signal
            for (auto const & a:e->m_qpoAtoms)
              {
              if ( !lb ) { lb=true; ros << ""; } else { ros << ","; }
              if ( lc++ % 5 == 0 & lc > 1 ) { ros << "\n" << std::string((nRI+3)*g_nJsonIndent, ' ') ; }
              ros << "{\"id\": \"" << a->id << "\"}";
              }
            ros << " ],\n";

            ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "\"links\": [ ";
            lc = 0;     // Block counter
            lb = false; // Start signal
            for (auto const & b:e->m_mLink)
              {
              if ( !lb ) { lb=true; ros << ""; } else { ros << ","; }
              if ( lc++ % 3 == 0 & lc > 1 ) { ros << "\n" << std::string((nRI+3)*g_nJsonIndent, ' ') ; }
              ros << "{\"to-thing-id\": \"" << b.first->id << "\", \"for-reason-id\": \"" << b.second->id << "\"}";
              }
            if ( ++cc < cm ) { ros << " ] },\n"; } else { ros << " ] }\n"; }
            }
          ros << std::string((nRI+0)*g_nJsonIndent, ' ') << "],\n";

// Atoms
          cm = m_oAtoms.size();
          cc = 0;
          ros << std::string((nRI+-1)*g_nJsonIndent, ' ') << "\"Atoms\": " << '\n';
          ros << std::string((nRI+ 0)*g_nJsonIndent, ' ') << '[' << '\n';
          for ( auto const & a:m_oAtoms )
            {
            ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "{ ";
//            ros                                            << "\"type\": \""   << a->type      << "\", ";
            ros                                            << "\"id\": \""     << a->id        << "\", ";
            ros                                            << "\"name\": \""   << a->m_sName   << "\", ";
            ros                                            << "\"prefix\": \"" << a->m_sPrefix << "\", ";
            ros                                            << "\"suffix\": \"" << a->m_sSuffix << "\", ";
            ros                                            << "\"format\": \"" << a->m_sFormat << "\" ";
//            ros                                            << "\"data\": \""   << a->m_tData   << "\"\n";
            if ( ++cc < cm ) { ros << "},\n"; } else { ros << "}\n"; }
            }
          ros << std::string((nRI+ 0)*g_nJsonIndent, ' ') << "],\n";


          cm = m_oReasons.size();
          cc = 0;
          ros << std::string((nRI+-1)*g_nJsonIndent, ' ') << "\"Reasons\": " << '\n';
          ros << std::string((nRI+ 0)*g_nJsonIndent, ' ') << '[' << '\n';
          for ( auto const & a:m_oReasons )
            {
            ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "{ ";
//            ros                                            << "\"type\": \"" << a->type      << "\", ";
            ros                                            << "\"id\": \""     << a->id        << "\", ";
            ros                                            << "\"name\": \""   << a->m_sName   << "\" ";
            if ( ++cc < cm ) { ros << "},\n"; } else { ros << "}\n"; }
            }
          ros << std::string((nRI+ 0)*g_nJsonIndent, ' ') << ']' << '\n';
          }

        void print_json(long const nRI /* RelativeIndent */, std::ostream & ros)
          {
          ros << std::string((nRI+0)*g_nJsonIndent, ' ') << '{' << '\n';
          ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "\"Object Database Dump\": " << '\n';
          ros << std::string((nRI+2)*g_nJsonIndent, ' ') << '{' << '\n';
          ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "\"Objects\": " << '\n';
          ros << std::string((nRI+3)*g_nJsonIndent, ' ') << '[' << '\n';

          print_json(m_oThings, 3, ros);

          ros << std::string((nRI+2)*g_nJsonIndent, ' ') << '}' << '\n';
          ros << std::string((nRI+1)*g_nJsonIndent, ' ') << '}' << '\n';
//          ros << std::string((nRI+0)*g_nJsonIndent, ' ') << '}' << '\n';
          }

    protected:
      CThings  m_oThings;
      CAtoms   m_oAtoms;
      CReasons m_oReasons;

    }; // class COdb

} // namespace odb

// CODB_H
#endif
