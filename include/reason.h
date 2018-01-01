#ifndef CREASON_H
#define CREASON_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

// #include <deque>
// #include <string>
// #include <memory>

// #include <iostream>
// #include <vector>
// #include <array>

#include "generic.h"


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
	  CReason(std::string const & crsName)
	    : m_sName(crsName.length() ? crsName : g_csNameUnnamedReason )
	    {
	    }

	  auto const & operator = (std::string const & sName)
	    {
	    return m_sName = sName;
	    }

      friend auto & operator << (std::ostream & ros, CReason const & croReason)
	    {
	    ros << croReason.m_sName; // << '\n';
	    return ros;
	    }

	  operator std::string const & ()
	    {
	    return m_sName;
	    }

      auto const & NameGet() { return m_sName; }

  protected:
    std::string m_sName{ g_csNameUnnamedReason };

  }; // class CReason

using PReason = std::shared_ptr<CReason>;
using CReasons = std::deque<PReason>;

} // namespace odb

// CREASON_H
#endif
