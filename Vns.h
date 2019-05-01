/*
 * Vns.h
 *
 *  Created on: 4 de abr de 2016
 *      Author: rodrigorandel
 */

#ifndef VNS_H_
#define VNS_H_


#include <vector>

#include "bibrand2.h"
#include "Constraints.h"
#include "Extra.h"
#include "Instance.h"
#include "Solution.h"



using namespace std;

class Vns {
public:
	Vns(Instance* inst, Constraints* constraints);
	~Vns();
	void run(Solution* bestFeasible, int KMAX, bool bestImprovement, int seed,
			double timeLimit, bool read_initial_sulution);

private:


	int N, M, P;
	Solution *current, *best, *feasible;
	bool* affected;
	double *loss, *gain;
	Extra* extra;
	Bibrand bibrand;
	vector<int> entitiesWithCL;

	double t_max;
	bool bestImprovement;



	void shaking(int k);
	void verify(Solution* s);
	void initVNS(Solution *best, bool bestImprovement, int seed,
			double timeLimit, bool read_initial_sulution);
	bool restore(Solution *s);

	void initLocalSearch();
	void findBestSwap(int*fi, int*fr, double* profit);
	bool localSearch();
	void compute(bool add);
	void swap(Solution* s, int fi, int fr);
	void update(Solution* s, int fi, int fr);
	void setClosest(Solution*s, int u, int f);
	void restoreFeasibility(Solution* s,Solution* sm);
};

#endif /* VNS_H_ */
