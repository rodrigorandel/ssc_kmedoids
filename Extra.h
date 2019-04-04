/*
 * LocalSearch.h
 *
 *  Created on: 28/03/2016
 *      Author: rodrigorandel
 */

#ifndef EXTRA_H_
#define EXTRA_H_

#include <utility>
#include <list>

using namespace std;

class Extra {
public:
	Extra();
	~Extra();
	void init();
	void add(int f, double value);

	list<pair<int, double>> facilities;
};

#endif /* EXTRA_H_ */
