/*
 * Main.cpp
 *
 *  Created on: 4 de abr de 2016
 *      Author: rodrigorandel
 */

//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <algorithm>
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Constraints.h"
#include "Instance.h"
//#include "Model.h"
#include "Solution.h"
#include "Vns.h"
#include "Helper.h"
using namespace std;

double timeLimit,value_exact;
int k_max, p_medians, x_execs;
int improvement;
bool read_initial_sulution;
bool write_solution;
char* instance_file = nullptr;
char* constraints_file = nullptr;
char* distances_file = nullptr;

Solution* bestSol = nullptr;
//Model* model = nullptr;
Instance* instance = nullptr;

void init(int argc, char* argv[]);
void executeVNS();


int main(int argc, char* argv[]) {

	init(argc, argv);
	instance = new Instance(instance_file, p_medians,distances_file);

	/** --- VNS_PPMSS --- */
    executeVNS();


	delete bestSol;
//	delete model;
	delete instance;
	delete[] instance_file;
	delete[] constraints_file;
	if(distances_file != nullptr){
		delete [] distances_file;
	}
	return 0;
}


void executeVNS(){
	Constraints* constraints = new Constraints(constraints_file, instance);
	Solution* sol = new Solution(instance, constraints);
//	if(read_initial_sulution){
//		ifstream ifs("solution");
//		boost::archive::text_iarchive ia(ifs);
//		ia >> *sol;
//		cout << "leu solucao base " << sol->getCost() << endl;
//		sol->updateFeasibility();
//	}

	bestSol = new Solution(instance, constraints);

	Vns *vns = new Vns(instance, constraints);
	double sum = 0;
	FOR(i,0,x_execs){
		vns->run(sol, k_max, improvement, i + 1, timeLimit, read_initial_sulution);
		sum += sol->getCost();
		if (*sol < *bestSol) {
			*bestSol = *sol;
		}
	}
//	if(write_solution){
//		cout << "vai escrever solucao base " << bestSol->getCost() << endl;
//		ofstream ofs("solution");
//		boost::archive::text_oarchive oa(ofs);
//		oa << *bestSol;
//		cout << "Facilities" << endl;
//		FOR(j,0,instance->getM())
//		{
//			cout << "\t" << bestSol->getFacilities()[j];
//		}
//		cout << "\nPOS" << endl;
//		FOR(j,0,instance->getM())
//		{
//			cout << "\t" << bestSol->getPos()[j];
//		}
//		cout << "\nC1" << endl;
//		FOR(j,0,instance->getN())
//		{
//			cout << "\t" << bestSol->getC1()[j];
//		}
//		cout << "\nC2" << endl;
//		FOR(j,0,instance->getN())
//		{
//			cout << "\t" << bestSol->getC2()[j];
//		}
//	}

//	cout << "escrever cost " << bestSol->getCost() << endl;
//	ofstream ofs("out.txt");
	cout << sum/x_execs << endl;
//	ofs.close();

	delete constraints;
	delete sol;
	delete vns;
}

void init(int argc, char* argv[]) {
	x_execs = 1;
	k_max = p_medians = -1;
	timeLimit = 1;
	improvement = 1;
	read_initial_sulution = false;
	write_solution = false;

	static struct option long_options[] = {
			{ "help", no_argument, 0, 'h' },
			{"k_max", required_argument, 0, 'k' },
			{ "improvement",required_argument, 0, 'a' },
			{ "instance_file", required_argument,0, 'i' },
			{ "constraints_file", required_argument, 0, 'c' },
			{"p_median", required_argument, 0, 'p' },
			{ "x_execs",required_argument, 0, 'x' },
			{ "time_limit", required_argument, 0,'t'},
			{"distance_matrix", required_argument,0,'d'},
			{"read_initial_solution", required_argument,0,'r'},
			{"write_solution", required_argument,0,'w'}};

	int option;
	option = opterr = 0;

	/* parse options using getopt */
	while (true) {
		option = getopt_long(argc, argv, "hk:a:i:c:p:x:t:d:r:w:", long_options,
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
		case 'x':
			x_execs = atoi(optarg);
			break;
		case 'r':
			read_initial_sulution = atoi(optarg) != 0;
			break;
		case 'w':
			write_solution = atoi(optarg) != 0;
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
