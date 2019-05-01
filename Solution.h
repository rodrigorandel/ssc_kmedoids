/*
 * Solution.h
 *
 *  Created on: 30/03/2016
 *      Author: rodrigorandel
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <fstream>

#include "bibrand2.h"
#include "Constraints.h"
#include "Instance.h"

class Solution {
private:
//	friend class boost::serialization::access;
//	// When the class Archive corresponds to an output archive, the
//	// & operator is defined similar to <<.  Likewise, when the class Archive
//	// is a type of input archive the & operator is defined similar to >>.
//	template<class Archive>
//	void serialize(Archive & ar, const unsigned int version)
//	{
//		ar & cost;
//		for(int i=0;i< instance->getM(); i++)
//			ar & facilities[i];
//		for(int i=0;i< instance->getM(); i++)
//			ar & pos[i];
//		for(int i=0;i< instance->getN(); i++)
//			ar & c1[i];
//		for(int i=0;i< instance->getN(); i++)
//			ar & c2[i];
//	}

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

	void updateFeasibility();

	int* getFacilities();
	int* getPos();
	int* getC1();
	int* getC2();
	double getCost();
	void setCost(double cost);
	Constraints* getConstraints();
	Instance* getInstance();

	void createRandomSolution(Bibrand* bibrand);
//	void readBaseSolution();
//	void writeSolution();

	bool operator<(const Solution& o);
	Solution& operator=(const Solution& other);
	bool isFeasible();
	void setFeasible(bool feasible);
};

#endif /* SOLUTION_H_ */
