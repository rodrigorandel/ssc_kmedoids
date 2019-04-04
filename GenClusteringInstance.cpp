/*
 * GenClusteringInstance.cpp
 *
 *  Created on: 2017-05-18
 *      Author: randrodr
 */



#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <omp.h>
#include <iomanip>
#include <algorithm>
#include <vector>

#include "Helper.h"


using namespace std;
int main(){

	string instances[] = {"iris","control","optical","statlog"};
	int size[] = {60,160,600,1000};

	FOR(i,0,sizeof(instances)/sizeof(string)){

		stringstream infile;
		infile << "InstancesConst/" << instances[i] << "1.out.txt";
		fstream in(infile.str().c_str());

		stringstream clfile;
		clfile << "InstancesConst/" << instances[i] << "1.out.cl.txt";
		ofstream cl(clfile.str().c_str());

		stringstream mlfile;
		mlfile << "InstancesConst/" << instances[i] << "1.out.ml.txt";
		ofstream ml(mlfile.str().c_str());

		cl << size[i]/2 << " 1" << endl;
		ml << size[i]/2 << " 1" << endl;

		double d,v;
		in >> d >> v;

		int a,b,c;
		while(true){
			in >> a;
			if(!in.good()) break;
			in >> b >> c >> v;
			//if(v < -1e-6){
				if(c == -1){
					cl << v << endl;
				}else{
					ml << v << endl;
				}
			//}
		}

	}

	return 0;
}
