/*
 * Performance.h
 *
 *  Created on: 19 de abr de 2016
 *      Author: rodrigorandel
 */

#ifndef PERFORMANCE_H_
#define PERFORMANCE_H_

#include "Instance.h"
#include "tempsC++.h"

using namespace std;

class Performance {
public:
	Performance();
	~Performance();

	unsigned int count_iterations_vns;
	unsigned int count_feasibility_restored;
	unsigned int count_feasibility_fail;
	unsigned int count_already_feasible;

	double alocation, shaking, restoring, searching;
	double totalTime;

	double bestCost;
	double bestTime;

	double bestSolutionsCosts;
	double bestSolutionsTimes;

	ChronoCPU clockTotal;
	ChronoCPU clockShaking;
	ChronoCPU clockFeasibility;
	ChronoCPU clockLocalSearch;
	ChronoCPU clockUpdatingBlocked;

	/**
	 * prints the performance
	 */
	void print(Instance* instance, bool best, int x_execs,
			int k_max);

	void printLatexTable(int x_execs, double value_exact);

	long double ARI(long double* ri, long double* ari, int N, int P, int* sol_vns, int* sol_correct);
	static long double nchoosek(int, int);
	static long double fat(int);


};

#endif /* PERFORMANCE_H_ */
