/*
 * Main.cpp
 *
 *  Created on: 4 de abr de 2016
 *      Author: rodrigorandel
 */

#include <algorithm>
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>

#include "Constraints.h"
#include "Instance.h"
#include "Model.h"
#include "Solution.h"
#include "Vns.h"
#include "Helper.h"
using namespace std;

double timeLimit,value_exact;
int k_max, p_medians, x_execs;
int improvement;
char* instance_file = nullptr;
char* constraints_file = nullptr;
char* distances_file = nullptr;
char* solution_file = nullptr;
bool b = false;

Solution* bestSol = nullptr;
Model* model = nullptr;
Performance* performance = nullptr;
Instance* instance = nullptr;

long double ri,ari;

void ARI(long double* ri, long double* ari, Solution* s, Performance* performance);
void init(int argc, char* argv[]);
void executeExact();
void executeVNS();

int main(int argc, char* argv[]) {

	init(argc, argv);
//    cout << "lendo instancia" << endl;
	instance = new Instance(instance_file, p_medians,distances_file);
    b = 0;
	if(b){
		/** --- EXACT MODEL-- */
		executeExact();
	}

	/** --- VNS_PPMSS --- */
    executeVNS();

	//cout << "RL = " << setprecision(2) << 100*(bestSol->getCost()-(value_exact))/bestSol->getCost() << "%" << endl;

	if(b){
//		if(solution_file != nullptr){
//			long double ri,ari;
//			ARI(&ri,&ari,bestSol,performance);
//			cout << "-1\t" << fixed << setprecision(3) << ari  << "\t" << ri << endl;
//		}
		cout << model->isInteger() << " " << value_exact <<  endl;
		double v;
		string name;
		model->getCannotLinkRelaxationGap(&v, &name);
		//cout << "Cannot-link = " << name << " = " << v << endl;
		//cout << "GAP = " << setprecision(2) << 100*(bestSol->getCost()-(value_exact+v))/bestSol->getCost() << "%" << endl;

		double v2;
		string name2;
		model->getMustLinkRelaxationGap(&v2, &name2);
		//cout << "Must-link = " << name2 << " = " << v2 << endl;
		//cout << "GAP = " << setprecision(2) << 100*(bestSol->getCost()-(value_exact+v2))/bestSol->getCost() << "%" << endl;



	}else{
//        cout << scientific << setprecision(8) << bestSol->getCost();
		if(solution_file != nullptr){
			//long double ri,ari;
			//ARI(&ri,&ari,bestSol,performance);
			//cout << setprecision(2) << fixed << "RI = " << ri << "\tARI = " << ari << endl;
//            cout << "\t" << fixed << setprecision(3) << ari/x_execs  << "\t" << ri/x_execs << endl;
            cout << fixed << setprecision(2) << ari << endl;
        }

	}


	delete bestSol;
	delete model;
	delete instance;
	delete performance;
	delete[] instance_file;
	delete[] constraints_file;
	if(distances_file != nullptr){
		delete [] distances_file;
	}
	if(solution_file != nullptr){
		delete [] solution_file;
	}
	return 0;
}


void executeVNS(){
	performance = new Performance();
//     cout << "lendo constraints" << endl;
	Constraints* constraints = new Constraints(constraints_file, instance);
//    cout << "iniciando solucao" << endl;
	Solution* sol = new Solution(instance, constraints);
	bestSol = new Solution(instance, constraints);

//    cout << "vai comecar" << endl;
	Vns *vns = new Vns(instance, constraints, performance);

	FOR(i,0,x_execs){
		vns->run(sol, k_max, improvement, i + 1, timeLimit);
//        cout << "ok" << endl;
		long double old_ri = ri;
		long double old_ari = ari;
		ARI(&ri,&ari,sol,performance);
		ri += old_ri;
		ari += old_ari;
		if (sol->getCost() <= performance->bestCost) {
			*bestSol = *sol;
		}
	}

	/* Just to check... */
//	cout << "BEST SOLUTION FOUND " <<  bestSol->isFeasible() <<  endl;
	//bestSol->print();
	//cout << "------------------- Config = " << constraints_file << " ------------------" << endl << endl;
	//performance->printLatexTable(x_execs, value_exact);
	//performance->print(instance, improvement, x_execs, k_max);
	//if(solution_file != nullptr){
		//long double ri,ari;
		//ARI(&ri,&ari,bestSol,performance);
		//cout << setprecision(2) << fixed << "RI = " << ri << "\tARI = " << ari << endl;
		//cout << setprecision(2) << fixed << ari << endl;
	//}
	//cout << endl << endl;

//	int a,b,cluster;
//	double d;
//	cluster = bestSol->maxDiameter(&a,&b,&d);
//	cout << "DIAMETER  [" << a << "," << b << "] = " << d << " NO CLUSTER " << cluster << endl;
//	cluster = bestSol->minSplit(&a,&b,&d);
//	cout << "SPLIT  [" << a << "," << b << "] = " << d << " NO CLUSTER " << cluster << endl;


	delete constraints;
	delete sol;
	delete vns;
}

void executeExact(){
	model = new Model(instance);
	model->buildModel();
	model->addConstraints(constraints_file);
    model->exportModel();
	model->solve();
	value_exact = model->getOptValue();
}

void init(int argc, char* argv[]) {
	x_execs = 1;
	k_max = p_medians = -1;
	timeLimit = -1;
	value_exact = -DBL_MAX;

	static struct option long_options[] = {
			{ "help", no_argument, 0, 'h' },
			{"k_max", required_argument, 0, 'k' },
			{ "improvement",required_argument, 0, 'a' },
			{ "instance_file", required_argument,0, 'i' },
			{ "constraints_file", required_argument, 0, 'c' },
			{"p_median", required_argument, 0, 'p' },
			{ "x_execs",required_argument, 0, 'x' },
			{ "time_limit", required_argument, 0,'t' },
			{"distance_matrix", required_argument,0,'d'},
			{"solution_file", required_argument,0,'s'},
			{"value_exact", required_argument,0,'v'},
			{"basd",required_argument,0,'b'}};
	int option;
	option = opterr = 0;

	/* parse options using getopt */
	while (true) {
		option = getopt_long(argc, argv, "hk:a:i:c:p:x:t:d:s:v:b:", long_options,
				nullptr);
		if (option == -1)
			break;
		switch (option) {
		case 'k':
			k_max = atoi(optarg);
			break;
		case 't':
			timeLimit = (double) atof(optarg);
			break;
		case 'p':
			p_medians = atoi(optarg);
			break;
		case 'a':
			improvement = atoi(optarg);
			break;
		case 'b':
			b = atoi(optarg);
			break;
		case 'x':
			x_execs = atoi(optarg);
			break;
		case 'i':
			instance_file = new char[256];
			strcpy(instance_file, optarg);
			break;
		case 'c':
			constraints_file = new char[256];
			strcpy(constraints_file, optarg);
			break;
		case 'd':
			distances_file = new char[256];
			strcpy(distances_file, optarg);
			break;
		case 's':
			solution_file = new char[256];
			strcpy(solution_file, optarg);
			break;
		case 'v':
			value_exact = (double) atof(optarg);
			break;
		case 'h': /* help */
			fprintf(stderr,
					"\nUSAGE"
							"\n\t%s -i <instance_file> -c <constraints_file> -p <p_medians> -k <k_max> -s <improvement> -t <t_max> [-x <times>] \n"
							"\nDESCRIPTION"
							"\n\tConstrainted P-Median Problem.\n"
							"\nOPTIONS"
							"\n\t-i I, --instance_file=I   \n\t\tSpecifies the data set to be used.\n"
							"\n\t-c C, --constraints_file=C \n\t\tSpecifies the constraints to be applied.\n"
							"\n\t-d D, --distance_matrix=D \n\tSpecifies the distance matrix nxm of the problem.\n"
							"\n\t-p P, --p_median=P \n\t\tSpecifies the number of medians to be opened\n\t\tEqual or greater than 2.\n"
							"\n\t-t T, --time_limit=T \n\t\tSpecifies the time limit for VNS.\n"
							"\n\t-k K, --k_max=K \n\t\tSets the maximum VNS parameter k.\n\t\tMust be lower or equal to p.\n"
							"\n\t-a A, --improvement=A \n\t\tSpecifies the improvement type used on trying to restore feasibility.\n"
							"\n\t\t 0  First Improvement."
							"\n\t\t 1  Best Improvement.\n"
							"\n\t-x X, \n\t\tNumber of executions.\n"
							"\n", argv[0]);
			exit(0); /* success */
		case '?': /* wrong options */
			fprintf(stderr,
					"Wrong options : Try `%s --help' 1for more information.\n\n",
					argv[0]);
                cout << option << endl;
			exit(1); /* failure */
		default:
			abort();
		}
	}

	if (k_max < 0 || p_medians < 2 || timeLimit < 0 || x_execs < 0
			|| (improvement != 0 && improvement != 1)
			|| instance_file == nullptr || constraints_file == nullptr) {
		fprintf(stderr,
				"Wrong options 2: Try `%s --help' 2for more information.\n\n",
				argv[0]);
		exit(1);
	}

}

void ARI(long double* ri, long double* ari, Solution* s, Performance* performance){

	fstream fcin(solution_file);
	if(!fcin.good()){
		cout << "Solution file " << solution_file << " not found" << endl;
		return;
	}
//    cout << "ok 2" << endl;
	int *norm = new int[instance->getM()];
	FOR(i,0,instance->getM()){
		norm[i] = -1;
	}
	int pos = 0;
	FOR(i,0,instance->getM()){
		if(s->getPos()[i] < instance->getP() && norm[i] == -1){
			norm[i] = pos++;
		}
	}

	int old_N = instance->getOLD_N();
	int* sol_vns = new int[old_N];
	int* clusters = new int[old_N];

	int* sol_correct = new int[old_N];
	for(int i=0;i<old_N;i++){
		fcin >> sol_correct[i];
	}

	int i=0;
	FOR_EACH_P(list<Point*>, u, instance->getUsers()){
		clusters[(*u)->getId()] = s->closest(i);
		sol_vns[(*u)->getId()] = norm[clusters[(*u)->getId()]];
		FOR_EACH_P(list<Point*>, u2, (*u)->getMembers()){
			clusters[(*u2)->getId()] = clusters[(*u)->getId()];
			sol_vns[(*u2)->getId()] = norm[clusters[(*u)->getId()]];
		}
		i++;
	}

//	cout << "\tMEDIAN\tVNS\tCORRETO" << endl;
//	FOR(i,0,old_N){
//		cout << "P" << i << "\t" << clusters[i] << "\t" << sol_vns[i] << "\t" << sol_correct[i] << endl;
//	}
//	cout << endl;

//	cout << setprecision(2) << fixed << "ARI = " << performance->ARI(old_N,instance->getP(),sol_vns,sol_correct) << endl;
	performance->ARI(ri,ari,old_N,instance->getP(),sol_vns,sol_correct);
	delete[] norm;
	delete[] sol_vns;
	delete[] clusters;
	delete[] sol_correct;
}
