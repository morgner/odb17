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

        template<typename T>
        void print_json(T const & container, long const nRI, std::ostream & ros)
          {
          for (auto const & e:container)
            {
            ros << std::string((nRI+0)*g_nJsonIndent, ' ') << "\"type\": \"" << e->type      << "\", ";
            ros                                            << "\"id\": \""   << e->id        << "\", ";
            ros                                            << "\"name\": \"" << e->NameGet() << "\"\n";
            ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "\"atoms\": [\n";
// Atoms
            ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "{\"name\": \"\", \"id\": \"\", \"prefix\": \"\", \"suffix\": \"\", \"format\": \"\"},\n";
            ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "{\"name\": \"\", \"id\": \"\", \"prefix\": \"\", \"suffix\": \"\", \"format\": \"\"}\n";

            ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "]\n";
// Links
            ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "\"links\": [\n";

            ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "{\"reason-id\": \"\", \"thing-id\": \"\"},\n";
            ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "{\"reason-id\": \"\", \"thing-id\": \"\"}\n";

            ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "]\n";
            }
          }

        void print_json(long const nRI /* RelativeIndent */, std::ostream & ros)
          {
          ros << std::string((nRI+0)*g_nJsonIndent, ' ') << '{' << '\n';
          ros << std::string((nRI+1)*g_nJsonIndent, ' ') << "\"Object Database Dump:\"" << '\n';
          ros << std::string((nRI+2)*g_nJsonIndent, ' ') << '{' << '\n';
          ros << std::string((nRI+2)*g_nJsonIndent, ' ') << "\"All Objects:\"" << '\n';
          ros << std::string((nRI+3)*g_nJsonIndent, ' ') << '{' << '\n';

          print_json(m_oThings, 3, ros);

          ros << std::string((nRI+2)*g_nJsonIndent, ' ') << '}' << '\n';
          ros << std::string((nRI+1)*g_nJsonIndent, ' ') << '}' << '\n';
          ros << std::string((nRI+0)*g_nJsonIndent, ' ') << '}' << '\n';
          }

    protected:
      CThings  m_oThings;
      CAtoms   m_oAtoms;
      CReasons m_oReasons;

    }; // class COdb

} // namespace odb

// CODB_H
#endif
