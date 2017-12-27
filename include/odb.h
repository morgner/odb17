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


namespace modb {

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
        auto p = std::make_shared<modb::CThing>();
        m_oThings.push_back( p );
	return p;
	}

	std::shared_ptr<CAtom<int>> const MakeAtomInt(int data, std::string const & crsName = g_sUnNamed)
	{
	auto p = std::make_shared<modb::CAtom<int>>(data);
	m_oAtomsInt.push_back( p );
	return p;
	}

	std::shared_ptr<CAtom<char>> const MakeAtomChar(char data, std::string const & crsName = g_sUnNamed)
	{
	auto p = std::make_shared<modb::CAtom<char>>(data);
	m_oAtomsChar.push_back( p );
	return p;
	}

	void Dump()
	{
	print(m_oThings);
	print(m_oAtomsChar);
	print(m_oAtomsInt);
	}

	void print(CAtoms<char> const & deq)
	  {
	  for (auto && e : deq)
	    {
	    std::cout << e->type << " id: " << e->id << " data: " << e->Data() << '\n';
	    }
	  }

	void print(CAtoms<int> const & deq)
	  {
	  for (auto && e : deq)
	    {
	    std::cout << e->type << " id: " << e->id << " data: " << e->Data() << '\n';
	    }
	  }

	template<typename T>
	void print(std::deque<T> const & deq)
	  {
	  for (auto && e : deq)
	    {
	    std::cout << e->type << " id: " << e->id << /* " name: " << e->Name() << */ '\n';
	    }
	  }

    protected:
	CThings             m_oThings;
	CAtoms<char>        m_oAtomsChar;
	CAtoms<int>         m_oAtomsInt;
	CAtoms<long>        m_oAtomsLong;
	CAtoms<long long>   m_oAtomsLongLong;
	CAtoms<double>      m_oAtomsDouble;
	CAtoms<std::string> m_oAtomsString;

    }; // class COdb

} // namespace modb

// CODB_H
#endif
