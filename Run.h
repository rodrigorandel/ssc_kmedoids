/*
 * Run.h
 *
 *  Created on: 26 de abr de 2016
 *      Author: rodrigorandel
 */

#ifndef RUN_H_
#define RUN_H_

#include <string>
#include <iostream>
#include "Helper.h"
#include <initializer_list>

using namespace std;

class Run {
public:

	Run(string data, initializer_list<char*> cons, initializer_list<int> p,
			initializer_list<int> s, initializer_list<int> k, int x, float t) {
		this->data = data;
		this->cons = cons;
		this->p = p;
		this->s = s;
		this->k = k;
		this->x = x;
		this->t = t;
	}

	string data;
	initializer_list<char*> cons;
	initializer_list<int> p;
	initializer_list<int> s;
	initializer_list<int> k;
	int x;
	float t;

	friend ostream& operator<<(ostream& os, const Run& obj) {
		os << "Dataset: \t" << obj.data << endl;
		os << "Constraints \t{ ";
		FOR_EACH(initializer_list<char*>,i,obj.cons){
		os << *i << ", ";
	}
		os << "}" << endl;
		os << "p \t\t{ ";
		FOR_EACH(initializer_list<int>,i,obj.p){
		os << *i << ", ";
	}
		os << "}" << endl;
		os << "s \t\t{ ";
		FOR_EACH(initializer_list<int>,i,obj.s){
		os << *i << ", ";
	}
		os << "}" << endl;
		os << "k \t\t{ ";
		FOR_EACH(initializer_list<int>,i,obj.k){
		os << *i << ", ";
	}
		os << "}" << endl;
		os << "X_EXECS: \t" << obj.x << endl;
		os << "T_MAX: \t" << obj.t << endl;
		return os;
	}
};

#endif /* RUN_H_ */
