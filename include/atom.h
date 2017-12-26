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


namespace modb {

/**
 * An Atom, from whitch Things are built from
 */
template<typename T> // T for 'data'
class CAtom : public Identifiable<CAtom<T>>
    {
    protected:
                 CAtom();
    public:
        virtual ~CAtom(){};
                 CAtom(CAtom<T> const & src)
		 {
			 *this = src;
		 }

                 CAtom & operator=(CAtom<T> const & src)
		 {
		   m_tData = src.data;
		 }

                 CAtom(T data)
		 : m_tData(m_tData)
		 {
		 }

    protected:
	T m_tData;

    }; // class CAtom

template<typename T>
using CAtoms = std::deque<std::shared_ptr<CAtom<T>>>;

} // namespace modb

// CATOM_H
#endif
