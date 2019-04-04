/*
 * Helper.h
 *
 *  Created on: 28/03/2016
 *      Author: rodrigorandel
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "bibrand2.h"
#include <float.h>
#include <cstdlib>
#include <cmath>

#define FOR(i,s,f) for(int i=s;i<f;i++)
#define FOR_EACH(T,it,l) for(T::iterator it=l.begin();it!=l.end();it++)
#define FOR_EACH_P(T,it,l) for(T::iterator it=l->begin();it!=l->end();it++)

template<typename Iterator>
inline void permutation(Iterator first, Iterator last, Bibrand* bibrand) {
	if (first != last) {
		for (Iterator i = first + 1; i != last; ++i) {
			Iterator j = first + bibrand->get_rand((i - first));
			if (i != j)
				std::iter_swap(i, j);
		}
	}
}

#endif /* HELPER_H_ */
