/*
 * Performance.cpp
 *
 *  Created on: 19 de abr de 2016
 *      Author: rodrigorandel
 */

#include "Performance.h"

#include <cmath>
#include <iomanip>
#include <iostream>

#include "Helper.h"

using namespace std;

Performance::Performance() {
	count_iterations_vns = count_feasibility_restored =	count_feasibility_fail = count_already_feasible = 0;

	alocation = shaking = restoring = searching = totalTime = bestSolutionsCosts = bestSolutionsTimes = 0;

	bestTime = DBL_MAX;
	bestCost = DBL_MAX;
}

Performance::~Performance() {
}

void Performance::print(Instance* instance, bool best,
		int x_execs, int k_max) {

	cout
	<< "\n------------------------ PERFORMANCE ------------------------- \n";
	cout << "                 f = " << scientific << setprecision(7) << bestCost << endl;
	cout << "                 P = " << instance->getP() << endl;
	cout << "        IMPROVMENT = " << (best ? "BEST" : "FIRST") << endl;
	cout << "             K_MAX = " << k_max << endl;
	cout << "              TIME = " << setprecision(2) << fixed << bestTime	<< endl;
	cout << "          AVG COST = " << scientific << setprecision(7) << bestSolutionsCosts / x_execs << endl;
	cout << "          AVG TIME = " << setprecision(2) << fixed	<< bestSolutionsTimes / x_execs << endl;
	cout << "    VNS ITERATIONS = " << count_iterations_vns / x_execs << endl;
	cout << "      SHAKING TIME = " << setprecision(2) << fixed	<< shaking / x_execs << endl;
	cout << " LOCAL SEARCH TIME = " << setprecision(2) << fixed	<< searching / x_execs << endl;
	cout << "  FEASIBILITY TIME = " << setprecision(2) << fixed	<< restoring / x_execs << endl;
	cout << "     UPDATING TIME = " << setprecision(2) << fixed	<< alocation / x_execs << endl;
	cout << "    VNS TOTAL TIME = " << setprecision(2) << fixed	<< totalTime / x_execs << endl;
	cout << " RESTORED SOLUTION = " << count_feasibility_restored << "/" << count_feasibility_restored + count_feasibility_fail << " - " << setprecision(2) << fixed << 100.0 * count_feasibility_restored	/ (count_feasibility_restored + count_feasibility_fail)	<< "%" << endl;
	cout << "   ALREADY FISEBLE = " << count_already_feasible << endl;
	cout << "------------------------------------------------------------- \n";

}

void Performance::printLatexTable(int x_execs, double value_exact) {

	double erro = 100*(bestCost - value_exact)/value_exact;
	if(erro < 1e-3){
		cout << "\\textbf{0.00\\%}";
	}else{
		cout << setprecision(3) << fixed << erro << "\\%";
	}
	cout << " & " << setprecision(2) << fixed << bestTime;

	erro = 100*((bestSolutionsCosts / x_execs) - value_exact)/value_exact;
	if(erro < 1e-3){
		cout << " & \\textbf{0.00\\%}";
	}else{
		cout << setprecision(3) << fixed << " & " << erro << "\\%";
	}
	cout << " & " << setprecision(2) << fixed << bestSolutionsTimes / x_execs;
//	cout << " & " << setprecision(2) << fixed << ((bestSolutionsCosts / x_execs) - bestCost)/bestCost;
	cout << " & " << count_iterations_vns / x_execs;
	cout << " & " << (count_feasibility_restored-count_already_feasible) / x_execs << " ("<< (int) (100* ceil((double) count_feasibility_restored/ (count_feasibility_restored+ count_feasibility_fail))) << "\\%)";

	cout << " & " << setprecision(2) << fixed << searching / x_execs;
	cout << " & " << setprecision(2) << fixed << restoring / x_execs;
	cout << " & " << setprecision(2) << fixed << shaking / x_execs;
	cout << " & " << setprecision(2) << fixed << alocation / x_execs;
	cout << " \\\\ " << endl;

}

long double Performance::ARI(long double* ri,long double* ari, int N, int P, int* u, int* v) {

	double a,b,c,d;
	a = b = c = d = 0.0;

	FOR(i,0,N){
		FOR(j,i+1,N){
			if(u[i]==u[j]){
				if(v[i]==v[j]){
					a++;
				}else{
					b++;
				}
			}else{
				if(v[i]==v[j]){
					c++;
				}else{
					d++;
				}
			}
		}
	}

	long double expectedIndex = (a+b)*(a+c) + (c+d)*(b+d);
	long double bin = nchoosek(N,2);
	*ari = (bin*(a+d) - expectedIndex) / (pow(bin,2) -expectedIndex);

	*ri = (a+d)/(a+b+c+d);
//	cout << "RI = " << setprecision(2) << fixed << ri << "\t";
	return *ari;


//	int contingenceTable[P][P];
//	int a[P],b[P];
//	FOR(i,0,P){
//		a[i] = 0;
//	}
//	FOR(i,0,P){
//		b[i] = 0;
//	}
//	FOR(i,0,P){
//		FOR(j,0,P){
//			contingenceTable[i][j] = 0;
//		}
//	}
//	FOR(i,0,N){
//		contingenceTable[u[i]][v[i]]++;
//	}
//	FOR(i,0,P){
//		FOR(j,0,P){
//			a[i] += contingenceTable[i][j];
//			b[j] += contingenceTable[i][j];
//		}
//	}

//	long double ari;
//	long double index = 0.0;
//	long double expectedIndex = 0.0;
//	long double maxIndex = 0.0;
//	long double sum1 = 0.0;
//	long double sum2 = 0.0;
//
//	FOR(i,0,P){
//		if(a[i] > 1){
//			sum1 += nchoosek(a[i], 2);
//		}
//	}
//	FOR(i,0,P){
//		if(b[i] > 1){
//			sum2 += nchoosek(b[i], 2);
//		}
//	}
//	FOR(i,0,P){
//		FOR(j,0,P){
//			if(contingenceTable[i][j] > 1){
//				index += nchoosek(contingenceTable[i][j], 2);
//			}
//		}
//	}
//
//	expectedIndex = (sum1*sum2)/nchoosek(N, 2);
//	maxIndex = 0.5 * (sum1 + sum2);
//	ari = (index - expectedIndex) / (maxIndex - expectedIndex);
//	return ari;
}

long double Performance::fat(int n){
	long double f = 1.0;
	for(int i = 1; i<=n; i++){
		f *= i;
	}
	return f;
}

long double Performance::nchoosek(int n, int k){
	return (n*(n-1))/2;
//	return fat(n) / (fat(n-k) * fat(k));
}
