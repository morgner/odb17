#ifndef CREASON_H
#define CREASON_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <map>

#include "thing.h"
#include "generic.h"


void print(odb::CReasons & container);

namespace odb {

// using namespace std::string_literals;


/**
 * A Reason to link two Things (Unidirectional)
 */
class CReason : public Identifiable<CReason>
  {
  public:
    static constexpr auto g_csNameUnnamedReason{"unnamedReason"};
  public:
	  CReason();
	  CReason(std::string const & crsName);

	  std::string const & operator = (std::string const & sName);

      friend std::ostream & operator << (std::ostream & ros, CReason const & croReason);

	  operator std::string const & ();

      void RelationAdd( PThing poThingFrom, PThing poThingTo );

      void Dump();

      std::string const & NameGet();

  protected:
    std::string m_sName{ g_csNameUnnamedReason };
    std::multimap<PThing, PThing> m_mRelations;

  }; // class CReason

} // namespace odb

// CREASON_H
#endif
