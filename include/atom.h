#ifndef CATOM_H
#define CATOM_H

/*  atom.h
 *
 *  Created on: 26.12.2017
 *      Author: manfred morgner
 */

#include <deque>
#include <ostream>
#include <string>
#include <memory>

#include "generic.h"


namespace odb {

typedef int T;

/**
 * An Atom, from whitch Things are built from
 */
class CAtom : public Identifiable<CAtom>
    {

    protected:
                 CAtom();
    public:
        virtual ~CAtom(){};

                 CAtom(CAtom const & src)
		 {
			 *this = src;
		 }

                 CAtom & operator=(CAtom const & src)
		 {
		   m_tData = src.m_tData;
		 }

//	template<typename T> // T for 'data'
                 CAtom(T data)
		 : m_tData(data)
		 {
		 }

	T const & Data() { return m_tData; }

//    protected:
	T m_tData;

    }; // class CAtom

using CAtoms = std::deque<std::shared_ptr<CAtom>>;

} // namespace odb

// CATOM_H
#endif
