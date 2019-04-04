/*
 * Constraints.h
 *
 *  Created on: 1 de abr de 2016
 *      Author: rodrigorandel
 */

#ifndef CONSTRAINTS_H_
#define CONSTRAINTS_H_

#include <list>

#include "Instance.h"
#include "Point.h"

using namespace std;

#define CANNOT_LINK -1

class Constraints {
private:
	list<int>* cannot_link;
	int* mapRoot;
	int N;

public:
	Constraints(char* constraint_file, Instance *inst);
	~Constraints();

	void insertMustLink(int a, int b, list<Point*>* users);
	void insertCannotLink(int a, int b, list<Point*>* users);
	list<int>* getCannotLink(int u);
	int* findRoot(int u);
};

#endif /* CONSTRAINTS_H_ */
