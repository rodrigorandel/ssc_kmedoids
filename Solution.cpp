/*
 * Solution.cpp
 *
 *  Created on: 30/03/2016
 *      Author: rodrigorandel
 */

#include "Solution.h"

#include <iomanip>
#include <cmath>
#include <iostream>
#include <list>
#include <fstream>

#include "Helper.h"

using namespace std;

Solution::Solution(Instance* inst, Constraints* constr) {
	instance = inst;
	constraints = constr;
	facilities = new int[instance->getM()];
	pos = new int[instance->getM()];
	c1 = new int[instance->getN()];
	c2 = new int[instance->getN()];
	blocked = new bool*[instance->getN()];
	FOR(i,0,instance->getN())
	{
		blocked[i] = new bool[instance->getM()];
	}
	cost = DBL_MAX;
	time_found = 0;
	feasible = false;
}

Solution::Solution(const Solution& o) {
	instance = o.instance;
	constraints = o.constraints;
	facilities = new int[instance->getM()];
	pos = new int[instance->getM()];
	c1 = new int[instance->getN()];
	c2 = new int[instance->getN()];
	blocked = new bool*[instance->getN()];
	cout << instance->getM() << " " << instance->getN() << endl;
	FOR(i,0,instance->getN())
	{
		blocked[i] = new bool[instance->getM()];
	}

	FOR(i,0,instance->getM())
	{
		facilities[i] = o.facilities[i];
		pos[i] = o.pos[i];
	}
	FOR(i,0,instance->getN())
	{
		c1[i] = o.c1[i];
		c2[i] = o.c2[i];
	}
	FOR(i,0,instance->getN())
	{
		FOR(j,0,instance->getM())
						{
			blocked[i][j] = o.blocked[i][j];
						}
	}
	time_found = o.time_found;
	feasible = o.feasible;
	cost = o.cost;
}

Solution::~Solution() {
	delete[] facilities;
	delete[] pos;
	delete[] c1;
	delete[] c2;

	FOR(i,0,instance->getN())
	{
		delete[] blocked[i];
	}
	delete[] blocked;
}

Solution& Solution::operator=(const Solution& o) {
	instance = o.instance;
	constraints = o.constraints;
	FOR(i,0,instance->getM())
	{
		facilities[i] = o.facilities[i];
		pos[i] = o.pos[i];
	}
	FOR(i,0,instance->getN())
	{
		c1[i] = o.c1[i];
		c2[i] = o.c2[i];
	}
	FOR(i,0,instance->getN())
	{
		FOR(j,0,instance->getM())
						{
			blocked[i][j] = o.blocked[i][j];
						}
	}
	time_found = o.time_found;
	feasible = o.feasible;
	cost = o.cost;
	return *this;
}

void Solution::swap(int fi, int fr) {

	int pos_fi = pos[fi];
	int pos_fr = pos[fr];

	if (pos_fr >= instance->getP() || pos_fi < instance->getP()) {
		cerr << "ERROR!" << endl;
		cout << "FR: " << fr << " POS: " << pos_fr << endl;
		cout << "FI: " << fi << " POS: " << pos_fi << endl;
		exit(1);
	}

	facilities[pos_fi] = fr;
	facilities[pos_fr] = fi;

	pos[fi] = pos_fr;
	pos[fr] = pos_fi;
}

bool Solution::operator<(const Solution& other) {
	return (this->cost - other.cost) < -1e-6;
}

void Solution::setClosest(int u, int f) {
	int left = c1[u];
	c1[u] = f;
	cost += dist(u, f) - dist(u, left);

	if(blocked[u][f]){
		feasible = false;
	}

	FOR_EACH_P(list<int>,cnl,constraints->getCannotLink(u)){
		bool stillRestricted = false;
		FOR_EACH_P(list<int>,cnl2,constraints->getCannotLink(*cnl)) {
			if(c1[*cnl2] == left) {
				stillRestricted = true;
				break;
			}
		}
		if(!stillRestricted) {
			blocked[*cnl][left] = false;
		}
		blocked[*cnl][ c1[u]] = true;
	}

}

void Solution::print() {
	cout
	<< "==========================================================================================================\n";
	cout << "OPENED FACILITIES = [ ";
	FOR(j,0,instance->getM())
	{
		if (pos[j] < instance->getP()) {
			cout << j << " ";

		}
	}
	cout << "]" << endl << endl;
	cout << "Users:\t";
	//	FOR_EACH_P(list<Point*>,i,instance->getUsers()){
	//		cout << "\t|\tP" << (*i)->getId();
	//	}
	FOR(i,0,instance->getN())
	{
		cout << "\t|\t" << i;
	}
	cout << "\nAssigned to:";
	FOR(i,0,instance->getN())
	{
		cout << "\t|\t" << c1[i];
	}
	cout << endl;
	cout << "Second:\t";
	FOR(i,0,instance->getN())
	{
		cout << "\t|\t" << c2[i];
	}
	cout << endl;
	cout << "Cost:\t";
	FOR(i,0,instance->getN())
	{
		cout << scientific << setprecision(5) << "\t|  " << dist(i, c1[i]);
	}
	cout << endl;
	cout << endl << "************* Constraints *************" << endl;
	FOR(u,0,instance->getN())
	{
		if (constraints->getCannotLink(u)->empty())
			continue;
		cout << "USER " << u << " BLOCKED FROM:\t[";
		FOR(f,0,instance->getM())
		{
			if (blocked[u][f]) {
				cout << " " << f;
			}
		}
		cout << " ]" << endl;
	}

	cout << endl;
	cout << setprecision(6) << scientific << "Cost = " << cost << endl;
	cout
	<< "=========================================================================================================\n\n";
}

void Solution::createRandomSolution(Bibrand* bibrand) {
//	list<int> allMedians;
//	FOR(i,0,instance->getM()){
//		allMedians.push_back(i);
//	}
//	ifstream fcin("initial_sol.txt");
//	FOR(i,0,instance->getP()){
//		fcin >> facilities[i];
//		pos[facilities[i]] = i;
//		allMedians.remove(facilities[i]);
//	}
//	int i = instance->getP();
//	FOR_EACH(list<int>,it,allMedians){
//		facilities[i] = *it;
//		pos[facilities[i]] = i++;
//	}
	FOR(j,0,instance->getM()){
		facilities[j] = pos[j] = j;
	}
	FOR(j,0,instance->getM()){
		int fr = facilities[bibrand->get_rand(instance->getP() - 1)];
		int fi = facilities[bibrand->get_rand_ij(instance->getP(),
				instance->getM() - 1)];
		swap(fi, fr);
	}
	cost = 0;
	FOR(u,0,instance->getN()){
		int f1;
		double d1 = DBL_MAX;
		FOR(j,0,instance->getP()){
			if (dist(u, facilities[j]) < d1) {
				f1 = facilities[j];
				d1 = dist(u, facilities[j]);
			}
		}
		int f2;
		double d2 = DBL_MAX;
		FOR(j,0,instance->getP()){
			if (f1 != facilities[j] && dist(u, facilities[j]) < d2) {
				f2 = facilities[j];
				d2 = dist(u, facilities[j]);
			}
		}
		cost += d1;
		c1[u] = f1;
		c2[u] = f2;
	}
	FOR(u,0,instance->getN()){
		FOR(j,0,instance->getM()){
			blocked[u][j] = false;
		}
	}
	FOR(u,0,instance->getN()){
		FOR_EACH_P(list<int>,cnl,getConstraints()->getCannotLink(u)){
			blocked[u][c1[*cnl]] = true;
//			cout << "Blocking " << u << " from " << c1[*cnl] << " because of " << *cnl << endl;
		}
	}

	feasible = true;
	FOR(u,0,instance->getN()){
		if(blocked[u][c1[u]]){
			feasible = false;
		}
	}
}

void Solution::updateFeasibility(){
	FOR(u,0,instance->getN()){
		FOR(j,0,instance->getM()){
			blocked[u][j] = false;
		}
	}
	FOR(u,0,instance->getN()){
		FOR_EACH_P(list<int>,cnl,getConstraints()->getCannotLink(u)){
			blocked[u][c1[*cnl]] = true;
			//			cout << "Blocking " << u << " from " << c1[*cnl] << " because of " << *cnl << endl;
		}
	}

	feasible = true;
	FOR(u,0,instance->getN()){
		if(blocked[u][c1[u]]){
			feasible = false;
		}
	}
}

int Solution::maxDiameter(int* u1, int* u2, double* maxDist) {
	*maxDist = -1;
	int i=0;
	int cluster = -1;;
	FOR_EACH_P(list<Point*>,p1,instance->getUsers()){
		list<Point*>::iterator p2 = next(p1);
		int j = i+1;
		for(;p2!=instance->getUsers()->end();p2++){
			if (c1[i] == c1[j]){
				if(instance->intern_dist((*p1)->getId(), (*p2)->getId()) > *maxDist) {
					*u1 = (*p1)->getId();
					*u2 = (*p2)->getId();
					*maxDist = instance->intern_dist((*p1)->getId(), (*p2)->getId());
					cluster = c1[i];
				}
				FOR_EACH_P(list<Point*>,p3,(*p2)->getMembers()){
					if(instance->intern_dist((*p1)->getId(), (*p3)->getId()) > *maxDist){
						*u1 = (*p1)->getId();
						*u2 = (*p3)->getId();
						*maxDist = instance->intern_dist((*p1)->getId(), (*p3)->getId());
					}
				}
			}
			j++;
		}
		i++;
	}
	return cluster;
}

int Solution::minSplit(int* u1, int* u2, double* minDist) {

	*minDist = DBL_MAX;
	int cluster = -1;
	int i=0;
	FOR_EACH_P(list<Point*>,p1,instance->getUsers()){
		list<Point*>::iterator p2 = next(p1);
		int j = i+1;
		for(;p2!=instance->getUsers()->end();p2++){
			if (c1[i] != c1[j]){
				if(instance->intern_dist((*p1)->getId(), (*p2)->getId()) < *minDist) {
					*u1 = (*p1)->getId();
					*u2 = (*p2)->getId();
					*minDist = instance->intern_dist((*p1)->getId(), (*p2)->getId());
					cluster = c1[j];
				}
				FOR_EACH_P(list<Point*>,p3,(*p2)->getMembers()){
					if(instance->intern_dist((*p1)->getId(), (*p3)->getId()) < *minDist){
						*u1 = (*p1)->getId();
						*u2 = (*p3)->getId();
						*minDist = instance->intern_dist((*p1)->getId(), (*p3)->getId());
						cluster = c1[j];
					}
				}
			}
			j++;
		}
		i++;
	}
	return cluster;
}

int* Solution::getFacilities() {
	return facilities;
}

int* Solution::getPos() {
	return pos;
}

int Solution::closest(int u) {
	return c1[u];
}

int Solution::second(int u) {
	return c2[u];
}

double Solution::dist(int u, int f) {
	return instance->distance(u, f);
}

void Solution::setSecondClosest(int u, int f) {
	c2[u] = f;
}

int* Solution::getC1() {
	return c1;
}
int* Solution::getC2() {
	return c2;
}

Instance* Solution::getInstance() {
	return instance;
}

Constraints* Solution::getConstraints() {
	return constraints;
}

double Solution::getCost() {
	return cost;
}

void Solution::setCost(double cost) {
	this->cost = cost;
}

bool Solution::isFeasible() {
	return feasible;
}

void Solution::setFeasible(bool feasible) {
	this->feasible = feasible;
}

void Solution::calculateCost() {
	cost = 0;
	FOR(u,0,instance->getN())
	{
		cost += dist(u, c1[u]);
	}
}
