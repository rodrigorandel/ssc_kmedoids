/*
 * Solution.h
 *
 *  Created on: 30/03/2016
 *      Author: rodrigorandel
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "bibrand2.h"
#include "Constraints.h"
#include "Instance.h"

class Solution {
private:
	Instance* instance;
	Constraints* constraints;

	/**
	 * [0,p-1] open
	 * [p,m-1] close
	 */
	int* facilities;
	/**
	 * Stores the position of a facility
	 * pos[f] = j, facilities[j] = f;
	 */
	int* pos;

	/** Stores the closest location for each user*/
	int* c1;
	/** Stores the second closest location for each user*/
	int* c2;

	double cost;

	bool feasible;

	double time_found;

public:
	Solution(Instance*, Constraints* constraints);
	Solution(const Solution& o);
	~Solution();

	bool **blocked;

	int closest(int u);
	int second(int u);
	void setClosest(int u, int f);
	void setSecondClosest(int u, int f);
	double dist(int u, int f);
	void swap(int fi, int fr);
	void print();
	void calculateCost();
	int maxDiameter(int* u1, int* u2, double* dist);
	int minSplit(int* u1, int* u2, double* dist);

	int* getFacilities();
	int* getPos();
	int* getC1();
	int* getC2();
	double getCost();
	void setCost(double cost);
	Constraints* getConstraints();
	Instance* getInstance();

	void createRandomSolution(Bibrand* bibrand);
	void createSolution();

	bool operator<(const Solution& o);
	Solution& operator=(const Solution& other);
	bool isFeasible();
	void setFeasible(bool feasible);
};

#endif /* SOLUTION_H_ */
