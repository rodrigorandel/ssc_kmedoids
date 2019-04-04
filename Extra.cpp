/*
 * Extra.cpp
 *
 *  Created on: 28/03/2016
 *      Author: rodrigorandel
 */

#include "Extra.h"

Extra::Extra() {
}

Extra::~Extra() {
}

void Extra::init() {
	facilities.clear();
}

void Extra::add(int f, double value) {
	list<pair<int, double>>::iterator it = facilities.begin();
	while (it != facilities.end() && it->first < f)
		it++;

	if (it == facilities.end() || it->first > f) {
		facilities.insert(it, pair<int, double>(f, value));
		return;
	}

	value += it->second;

	if (value < 1e-6 && value > -1e-6) {
		facilities.erase(it);
		return;
	}

	it->second = value;
}
