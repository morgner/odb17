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


namespace odb {

static std::string const g_sUnNamed = "unNamed";

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

                 COdb & operator=(COdb const & src)
		 {
		 }

	std::shared_ptr<CThing> MakeThing(std::string const & crsName = g_sUnNamed)
	{
        auto p = std::make_shared<odb::CThing>(crsName);
        m_oThings.emplace_back( p );
	return p;
	}

	template<typename T>
	std::shared_ptr<CAtom> const MakeAtom(T data, std::string const & crsName = CAtom::g_csNameUnnamedAtom)
	{
	auto p = std::make_shared<odb::CAtom>(data, crsName);
	m_oAtoms.emplace_back( p );
	return p;
	}

	void Dump()
	{
	print(m_oThings);
	print(m_oAtoms);
	}

	void print(CAtoms const & deq)
	  {
	  for (auto && e : deq)
	    {
	    std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\t' << " data: " << *e << '\n';
	    }
	  }

	template<typename T>
	void print(std::deque<T> const & deq)
	  {
	  for (auto && e : deq)
	    {
	    std::cout << e->type << '\t' << " id: " << e->id << '\t' << " name: " << e->NameGet() << '\n';
	    }
	  }

    protected:
	CThings  m_oThings;
	CAtoms   m_oAtoms;

    }; // class COdb

} // namespace odb

// CODB_H
#endif
