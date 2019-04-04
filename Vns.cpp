/*
 * Vns.cpp
 *
 *  Created on: 4 de abr de 2016
 *      Author: rodrigorandel
 */

#include "Vns.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Helper.h"

using namespace std;

Vns::Vns(Instance* inst, Constraints* constraints, Performance* perfor) {
	N = inst->getN();
	M = inst->getM();
	P = inst->getP();

	bestImprovement = false;
	isRestore = isShaking = isLocalSearch = false;

	affected = new bool[N];
	loss = new double[M];
	gain = new double[M];
	extra = new Extra[M];

	FOR(i,0,N){
		if(constraints->getCannotLink(i)->size() > 0){
			entitiesWithCL.push_back(i);
		}
	}

	performance = perfor;
	current = new Solution(inst, constraints);
	feasible = new Solution(inst, constraints);
	best = nullptr;
	bestTime = -1;
	t_max = -1;
}

Vns::~Vns() {
	delete current;
	delete feasible;
	delete[] loss;
	delete[] gain;
	delete[] affected;
	delete[] extra;
}

void Vns::run(Solution* bestFeasible, int KMAX, bool bestImprovement, int seed,
		double timeLimit) {
	initVNS(bestFeasible, bestImprovement, seed, timeLimit);
	int k = 1;
	bool improved;
	performance->clockTotal.Start();
	do {
		performance->count_iterations_vns++;
//        cout << performance->count_iterations_vns << endl;
		/** ============= SHAKING ============== **/
		performance->clockShaking.Start();
		shaking(k);
//		performance->clockShaking.Stop();
		/** ============= END SHAKING ============== **/

		/** ============= LOCAL SEARCH ============== **/
		performance->clockLocalSearch.Start();
		improved = localSearch();
		performance->clockLocalSearch.Stop();
		/** ============= END LOCAL SEARCH ============== **/
		k = (improved || k >= KMAX) ? 1 : k + 1;

	}
//	while(performance->clockTotal.GetTime() < t_max);
	while ((performance->clockFeasibility.GetTime()
			+ performance->clockLocalSearch.GetTime()
			+ performance->clockShaking.GetTime()
			+ performance->clockUpdatingBlocked.GetTime()) < t_max);

	performance->clockTotal.Stop();
	performance->totalTime += performance->clockTotal.GetTime();
	performance->alocation += performance->clockUpdatingBlocked.GetTime();
	performance->shaking += performance->clockShaking.GetTime();
	performance->restoring += performance->clockFeasibility.GetTime();
	performance->searching += performance->clockLocalSearch.GetTime();

	performance->bestSolutionsCosts += best->getCost();
	performance->bestSolutionsTimes += bestTime;
	if (best->getCost() < performance->bestCost) {
		performance->bestCost = best->getCost();
	}
	if (bestTime < performance->bestTime) {
		performance->bestTime = bestTime;
	}
}

void Vns::shaking(int k) {
	*current = *best;
//	cout << "ANTES" << endl;
//	current->print();
	verify(current);
//	cout << "\n DEPOIS " << endl;
//	current->print();
	FOR(i,0,k){
		int fi = current->getFacilities()[bibrand.get_rand_ij(P,M - 1)];
		int fr = current->getFacilities()[bibrand.get_rand_ij(0,P - 1)];
		swap(current, fi, fr);
		update(current, fi, fr);
	}

	performance->clockShaking.Stop();
	/** ============= RESTORE FEASIBILITY ============== **/
	if(!current->isFeasible()){
		performance->clockFeasibility.Start();
		restore(current);
		performance->clockFeasibility.Stop();
	}else{
		if(*current < *best){
			*best = *current;
		}
		performance->count_already_feasible++;
	}
	/** ============= END FEASIBILITY ============== **/

//	performance->clockShaking.Start();
}

bool Vns::localSearch() {
	initLocalSearch();
	int fi, fr;
	double profit;
	bool improved = false;
	double time = 0;
	while (time < t_max) {
		compute(true);
		findBestSwap(&fi, &fr, &profit);
		if (profit < 1e-6)
			break;
		FOR(i,0,N)		{
			if (fr == current->closest(i) || fr == current->second(i)|| current->dist(i, fi) < current->dist(i, current->second(i))) {
				affected[i] = true;
			} else {
				affected[i] = false;
			}
		}
		compute(false);
		swap(current, fi, fr);
		double old = current->getCost();
		update(current, fi, fr);

		gain[fr] = 0;
		loss[fi] = 0;


		performance->clockLocalSearch.Stop();
		/** ============= RESTORE FEASIBILITY ============== **/
		if(!current->isFeasible()){
			performance->clockFeasibility.Start();
			improved = restore(current);
			performance->clockFeasibility.Stop();
		}else{
			if(*current < *best){
				*best = *current;
			}
			performance->count_already_feasible++;
		}
		/** ============= END FEASIBILITY ============== **/
		time = performance->clockFeasibility.GetTime()
				+ performance->clockLocalSearch.GetTime()
				+ performance->clockShaking.GetTime()
				+ performance->clockUpdatingBlocked.GetTime();

		performance->clockLocalSearch.Start();
	}
	return improved;
}

bool Vns::restore(Solution *sm){
	bool improved = false;
	*feasible = *sm;
	restoreFeasibility(feasible,sm);
	if (feasible->isFeasible()) {
		performance->count_feasibility_restored++;
		if (*feasible < *best || !best->isFeasible()) {
			*best = *feasible;
			bestTime = performance->clockFeasibility.GetTime()
					+ performance->clockLocalSearch.GetTime()
					+ performance->clockShaking.GetTime()
					+ performance->clockUpdatingBlocked.GetTime();
			improved = true;
		}
	} else {
		performance->count_feasibility_fail++;
	}
	return improved;
}

void Vns::findBestSwap(int* fi, int* fr, double* profit) {

	int* facilities = current->getFacilities();
	int* pos = current->getPos();

	int f;
	double value;
	double max_gain = -DBL_MAX;
	FOR(i,P,M){
		f = facilities[i];
		value = gain[f];
		if (value > max_gain) {
			*fi = f;
			max_gain = value;
		}
	}

	double min_loss = DBL_MAX;
	FOR(i,0,P){
		f = facilities[i];
		value = loss[f];
		if (value < min_loss) {
			*fr = f;
			min_loss = value;
		}
	}

	*profit = gain[*fi] - loss[*fr];
	FOR(i,P,M){
		f = facilities[i];
		list<pair<int, double>>::iterator out = extra[f].facilities.begin();
		for (; out != extra[f].facilities.end(); out++) {
			if (pos[out->first] >= P)
				continue;
			if (out->second < -1e-4) {
				cout << "ERROR EXTRA(" << f << "," << out->first << ") = "
						<< out->second << endl;
//				exit(1);
			}
			value = gain[f] - loss[out->first] + out->second;
			if (value > *profit) {
				*profit = value;
				*fr = out->first;
				*fi = f;
			}
		}
	}

}

void Vns::compute(bool add) {
	int u, fr, fi, x;
	double d1, d2, profit;
	x = add ? 1 : -1;
	FOR(u,0,N){
		if (!affected[u])
			continue;

		fr = current->closest(u);
		d1 = current->dist(u, fr);
		d2 = current->dist(u, current->second(u));

		loss[fr] += x * (d2 - d1);
		FOR(i,P,M){
			fi = current->getFacilities()[i];
			if (current->dist(u, fi) < d2) {
				profit = d1 - current->dist(u, fi);
				if (profit > 0) {
					gain[fi] += x * profit;
					extra[fi].add(fr, x * (d2 - d1));
				} else {
					extra[fi].add(fr, x * (d2 - current->dist(u, fi)));
				}
			}
		}
	}
}

void Vns::swap(Solution* s, int fi, int fr) {
	s->swap(fi, fr);
}

void Vns::update(Solution* s, int fi, int fr) {

	list<int> modified;
	double d1, d2, din;
	FOR(i,0,N){
		d1 = s->dist(i, s->closest(i));
		d2 = s->dist(i, s->second(i));
		din = s->dist(i, fi);
		if (s->closest(i) == fr) {
			if (din < d2) {
				setClosest(s, i, fi);
			} else {
				setClosest(s, i, s->second(i));
				s->setSecondClosest(i, -1);
				modified.push_back(i);
			}
		} else if (din < d1) {
			s->setSecondClosest(i, s->closest(i));
			setClosest(s, i, fi);
		} else if (din < d2) {
			s->setSecondClosest(i, fi);
		} else if (s->second(i) == fr) {
			s->setSecondClosest(i, -1);
			modified.push_back(i);
		}
	}

	FOR_EACH(list<int>,u,modified){
		int fc = s->closest(*u);
		double min = DBL_MAX;
		FOR(f2,0,P) {
			int f = s->getFacilities()[f2];
			if(f!=fc && s->dist(*u,f) < min) {
				s->setSecondClosest(*u,f);
				min = s->dist(*u,f);
			}
		}
	}
}

void Vns::restoreFeasibility(Solution* s, Solution* sm) {
	int* facilities = s->getFacilities();
	bool improvment;
	double bestDist, dist;
	bool mustChange;
	int bestFacility;
	int user, f;

	do {
		improvment = false;
		permutation(entitiesWithCL.begin(), entitiesWithCL.end(), &bibrand);
		s->setFeasible(true);

		FOR(i,0,entitiesWithCL.size()){
			user = entitiesWithCL[i];
			f = s->closest(user);

			if (s->blocked[user][f]) {
				bestDist = DBL_MAX;
				mustChange = true;
			} else if(f != sm->closest(user)){
				bestDist = s->dist(user, f);
				mustChange = false;
			}else{
				continue;
			}
			bestFacility = -1;

			FOR(j,0,P){
//				cout << i << " " << j << endl;
				f = facilities[j];
				if (s->blocked[user][f]) continue;

				dist = s->dist(user, f);
				if (dist < bestDist) {
					improvment = true;
//					if (bestImprovement) {
						bestDist = dist;
						bestFacility = f;
//					} else {
//						bestFacility = f;
//						setClosest(s, user, f);
//						if(bestFacility != sm->closest(user)){
//							s->setSecondClosest(user,sm->closest(user));
//						}else{
//							s->setSecondClosest(user,sm->second(user));
//						}
//						break;
//					}
				}
			}
			if (bestFacility == -1) {
//				cout << "entrou" << endl;
				if (mustChange) {
					s->setFeasible(false);
				}
			} else if (bestImprovement && improvment) {
//				cout << "moving " << user << " to " << bestFacility << endl;
				setClosest(s, user, bestFacility);
				if(bestFacility != sm->closest(user)){
					s->setSecondClosest(user,sm->closest(user));
				}else{
					s->setSecondClosest(user,sm->second(user));
				}
			}
		}

	} while (improvment);

	FOR(u,0,N){
		int fc = s->closest(u);
		double min = DBL_MAX;
		FOR(j,0,P){
			f = facilities[j];
			if (f != fc && s->dist(u, f) < min) {
				s->setSecondClosest(u, f);
				min = s->dist(u, f);
			}
		}
	}
}

void Vns::setClosest(Solution* s, int u, int f) {

	if(performance->clockLocalSearch.isRunning()){
		isLocalSearch = true;
		performance->clockLocalSearch.Stop();
	}else if(performance->clockFeasibility.isRunning()){
		isRestore = true;
		performance->clockFeasibility.Stop();
	}else if(performance->clockShaking.isRunning()){
		isShaking = true;
		performance->clockShaking.Stop();
	}

	performance->clockUpdatingBlocked.Start();
	s->setClosest(u, f);
	performance->clockUpdatingBlocked.Stop();

	if(isLocalSearch){
		isLocalSearch = false;
		performance->clockLocalSearch.Start();
	}else if(isRestore){
		isRestore = false;
		performance->clockFeasibility.Start();
	}else if(isShaking){
		isShaking = false;
		performance->clockShaking.Start();
	}
}

void Vns::verify(Solution* s) {

	list<int> modified;
	FOR(i,0,entitiesWithCL.size()){
		int u = entitiesWithCL[i];
		if (s->dist(u, s->closest(u)) > s->dist(u, s->second(u))) {
			s->setClosest(u, s->second(u));
			modified.push_back(u);
		}
	}

	FOR_EACH(list<int>,u,modified){
		int fc = s->closest(*u);
		double min = DBL_MAX;
		FOR(f2,0,P) {
			int f = s->getFacilities()[f2];
			if(f!=fc && s->dist(*u,f) < min) {
				s->setSecondClosest(*u,f);
				min = s->dist(*u,f);
			}
		}
	}

}

void Vns::initVNS(Solution* bestFeasible, bool bestImprovement, int seed, double timeLimit) {

	this->bestImprovement = bestImprovement;
	bibrand = Bibrand(seed);
	bestTime = DBL_MAX;
	t_max = timeLimit;

	best = bestFeasible;
    
	best->createRandomSolution(&bibrand);
    
	if(!best->isFeasible()){
//        cout << "vai restorar" << endl;
		restore(best);
//        cout << "restourou " << endl;
		if(best->isFeasible()){
			bestTime = 0;
		}
	}else{
		bestTime = 0;
		performance->count_already_feasible++;
	}
	performance->clockTotal.Reset();
	performance->clockUpdatingBlocked.Reset();
	performance->clockShaking.Reset();
	performance->clockFeasibility.Reset();
	performance->clockLocalSearch.Reset();

#ifdef DEBUG
	cout << "\n\nINITIAL SOLUTION" << endl;
	best->print();
#endif
}

void Vns::initLocalSearch() {
	FOR(i,0,M)
	{
		loss[i] = gain[i] = 0;
		extra[i].init();
	}
	FOR(i,0,N)
	{
		affected[i] = true;
	}
}
