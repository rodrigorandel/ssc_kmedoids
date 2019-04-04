/*
 * Exec.cpp
 *
 *  Created on: 22 de abr de 2016
 *      Author: rodrigorandel
 */

#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <omp.h>
#include <iomanip>
#include <algorithm>
#include <vector>

#include "Helper.h"
#include "Run.h"

using namespace std;

void* exec(void* instance);
//void execPthread();
void execOmp();

int main() {
	int a;


	execOmp();

	//	system("tar -zcf ../Results_vns.tar.gz ../Results_vns/");
	//	system("email -r rodrigorandel@gmail.com -a ../Results_vns.tar.gz");
	return 0;
}

typedef struct{

	int a,b,c;
	double v;
	double ari,ri;
} Out;

bool myfunction (Out i,Out j) {
	if(i.ari > j.ari){
		return true;
	}else if(i.ari == j.ari){
		if(i.ri > j.ri) return true;
		else if(i.ri == j.ri) return (i.v<j.v);
		else return false;
	}else{
		return false;
	}

}

void execOmp(){

	string instances[] = {"iris","control","optical"};
	int p[] = {3,6,10};
//	int t[] = {300,200,100};
//	int lines[] = {40,160,500};
//	system("rm *.temp.txt");
//#pragma omp parallel for schedule(dynamic,1) collapse(2)
//#pragma omp parallel for num_threads(3)
	FOR(i,0,sizeof(instances)/sizeof(string)){
//		FOR(c,1,3){
//			FOR(l,0,lines[i]){
//				stringstream inst;
//				inst << "InstancesConfusion/" << instances[i] << c << ".txt";
//				ifstream fin(inst.str().c_str());
//
//				ofstream temp;
//				stringstream tempfile;
//				tempfile << instances[i]<< c <<".temp.txt";
//				temp.open(tempfile.str().c_str());
//
//				string line;
//				FOR(k,0,lines[i]){
//					getline(fin,line);
//					if(k!=l){
//						temp << line << endl;
//					}else{
//						stringstream out;
//						out << "echo " << line << " >> ResultsConfusion/" << instances[i] << c << ".out.txt";
//						system(out.str().c_str());
//					}
//
//				}
//				temp.close();
//
//				stringstream run;
//				run << "./main -i Instances/" << instances[i]  << ".data.txt"
//						" -c " << tempfile.str() << " "
//						" -s Instances/" << instances[i] << ".classes.txt"
//						" -p" << p[i] << " "
//						" -x" << t[i] << " "
//						" -k10 -a1 -t0 -b0";
//				if(i==2){
//					run << " -d Instances/" << instances[i] << ".distance.txt";
//				}
//
//				run << " >> ResultsConfusion/" << instances[i] << c << ".out.txt";
//				cout << run.str() << endl;
//				system(run.str().c_str());
//
//				//exit(1);
//
//
//			}
//
//			vector<Out> outs;
//
//			stringstream inst;
//			inst << "ResultsConfusion/" << instances[i] << c << ".out.txt";
//			fstream out(inst.str().c_str());
//
//			stringstream outsorted;
//			FOR(l,0,lines[i]){
//				Out o;
//				out >> o.a >> o.b >> o.c >> o.v >> o.ari >> o.ri;
//				outs.push_back(o);
//			}
//
//			sort(outs.begin(),outs.end(), myfunction);
//			stringstream final;
//			FOR(w,0,outs.size()){
//				final << outs[w].a << "\t" << outs[w].b << "\t" << outs[w].c << "\t\t";
//				final << scientific << setprecision(9) << outs[w].v;
//				final << fixed << setprecision(3) << "\t\t" << outs[w].ari << "\t\t" << outs[w].ri << endl;
//			}
//			stringstream inst2;
//			inst2 << "ResultsConfusion/" << instances[i] << c << ".out.txt";
//			ofstream out2(inst2.str().c_str(), ofstream::trunc);
//			out2 << final.str();


			stringstream run;
			run << "./main -i Instances/" << instances[i] << ".data.txt"
					" -c InstancesConst/" << instances[i] << "1.const.txt"
					" -s Instances/" << instances[i] << ".classes.txt"
					//" -d Instances/" << instances[i] << ".distance.txt"
					" -p" << p[i] << " "
					" -x" << 1 << " "
					" -k10 -a1 -t0 -b1";

			if(i==2){
				run << " -d Instances/" << instances[i] << ".distance.txt";
			}

			run << " > InstancesConst/" << instances[i] << "1.out.txt";
			cout << run.str() << endl;
			system(run.str().c_str());

		//}
	}
}



//void execPthread(){
//	Run* yeast = new Run{
//			"ionosphere.data.txt",
//			{"ionosphere1.txt","ionosphere2.txt"},
//			{2},
//			{0},{4},1,60}; //a,k,x e t sao desnecessarios
//
//	Run* iris = new Run{
//		"iris.data.txt",
//		{"iris1.txt","iris2.txt"},
//		{3},
//		{0},{4},1,60}; //a,k,x e t sao desnecessarios
//
//	Run* iris = new Run{
//			"iris.data.txt",
//			{"iris1.txt","iris2.txt"},
//			{3},
//			{0},{4},1,60}; //a,k,x e t sao desnecessarios
//
//	pthread_t thread_yale;
//	pthread_create(&thread_yale,nullptr,exec,(void*) yalefaces);
//	pthread_join(thread_yale,nullptr);
//}

void* exec(void* instance) {
	Run* run = (Run*) instance;
	cout << *run << endl;
	stringstream base;
	base << "./main -i" << run->data << " -x" << run->x << " -t" << run->t;
	FOR_EACH(initializer_list<char*>,cons,run->cons){
		stringstream rm;
		rm << "rm " << *cons << ".out";
		system(rm.str().c_str());
		stringstream sscons;
		sscons << base.str() << " -c" << *cons;
		FOR_EACH(initializer_list<int>,p,run->p) {
			stringstream ssp;
			ssp << sscons.str() << " -p" << *p;
			FOR_EACH(initializer_list<int>,k,run->k) {
				stringstream ssk;
				ssk << ssp.str() << " -k" << *k;
				FOR_EACH(initializer_list<int>,s,run->s) {
					stringstream sss;
					sss << ssk.str() << " -s" << *s;
					sss << " >> " << *cons << ".out";
					cout << sss.str() << endl;
					system(sss.str().c_str());
				}
			}
		}
	}
	return nullptr;

}
