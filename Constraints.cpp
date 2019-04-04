/*
 * Constraints.cpp
 *
 *  Created on: 1 de abr de 2016
 *      Author: rodrigorandel
 */

#include "Constraints.h"

#include <iostream>
#include <fstream>

#include "Helper.h"
#include "Instance.h"
#include "Point.h"

Constraints::Constraints(char* constraint_file, Instance *inst) {
	N = inst->getN();

	mapRoot = new int[N];
	cannot_link = new list<int> [N];

	FOR(i,0,inst->getN())
	{
		mapRoot[i] = i;
	}

	list<Point*>* users = inst->getUsers();

	ifstream fcin(constraint_file);
	if (!fcin.good()) {
		cerr << "Constraint file '" << constraint_file << "' not found" << endl;
		exit(1);
	}
//	int pmin;
//	fcin >> pmin;
//	if(pmin > inst->getP()){
//		cerr << "ERROR: P minimum is " << pmin << endl;
//		exit(1);
//	}
	int a, b, c;
	while (fcin.good()) {
		fcin >> a;
		if(!fcin.good()){
			break;
		}
		fcin >> b >> c;
		if (c != CANNOT_LINK) {
			insertMustLink(a, b, users);
		}
	}
	FOR_EACH_P(list<Point*>,u,users){
	(*u)->getMembers()->remove(*u);
	(*u)->getMembers()->sort();
	(*u)->getMembers()->unique();
}
	fcin.clear();
	fcin.seekg(0, ios::beg);
//	fcin >> pmin;
	while (fcin.good()) {
		fcin >> a;
		if(!fcin.good()){
			break;
		}
		fcin >> b >> c;
		if (c == CANNOT_LINK) {
			insertCannotLink(a, b, users);
		}
	}
	FOR(i,0,N)
	{
		cannot_link[i].sort();
		cannot_link[i].unique();
	}

	fcin.close();
	inst->applyConstraints();
}
Constraints::~Constraints() {
	delete[] mapRoot;
	delete[] cannot_link;
}

void Constraints::insertCannotLink(int a, int b, list<Point*>* users) {

	int rootA = *(findRoot(a));
	int rootB = *(findRoot(b));

	int indexA=-1, indexB=-1;
	int cont = 0;
	bool findAB = false;
	FOR_EACH_P(list<Point*>,u,users){
	if((*u)->getId() == rootA) {
		indexA = cont;
		if(findAB) break;
		findAB = true;
	} else if((*u)->getId() == rootB) {
		indexB = cont;
		if(findAB) break;
		findAB = true;
	}
	cont++;
}

	cannot_link[indexA].push_back(indexB);
	cannot_link[indexB].push_back(indexA);
}

void Constraints::insertMustLink(int a, int b, list<Point*>* users) {

	int* rootA = findRoot(a);
	int* rootB = findRoot(b);

	if (*rootA > *rootB) {
		int* tmp = rootA;
		rootA = rootB;
		rootB = tmp;
	}

	if (*rootA == *rootB)
		return;

	Point* userA = nullptr;
	Point* userB = nullptr;
	bool findAB = false;
	FOR_EACH_P(list<Point*>,u,users){
	if((*u)->getId() == *rootA) {
		userA = (*u);
		if(findAB) break;
		findAB = true;
	} else if((*u)->getId() == *rootB) {
		userB = (*u);
		if(findAB) break;
		findAB = true;
	}
}

	userA->insertPoint(userB);
	userA->insertPoints(userB->getMembers());
	userB->getMembers()->clear();
	*rootB = *rootA;
	users->remove(userB);
}

list<int>* Constraints::getCannotLink(int u) {
	return &(cannot_link[u]);
}

int* Constraints::findRoot(int u) {
	int* root = (mapRoot[u] == u) ? mapRoot + u : findRoot(mapRoot[u]);
	mapRoot[u] = *root;
	return root;
}
