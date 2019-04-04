/*
 * Main.cpp
 *
 *  Created on: 16 de mai de 2017
 *      Author: rodrigorandel
 */

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

using namespace std;

#define FOR(i,a_,b_) for(int i=a_;i<b_;i++)

int main(){

//	for(int i=0;i<10;i++){
//		for(int j=i+1;j<10;j++){
//			cout << i << " " << j << " " << -1 << endl;
//		}
//	}
//	exit(1);


	string inst[] = {"iris","control","optical","statlog"};
	int size[] = {150,600,3823,4435};
	int total[] = {30,80,300,500};

	FOR(i,0,sizeof(inst)/sizeof(string)){

		int MLCertas = 0;
		int MLErradas = 0;
		int CLCertas = 0;
		int CLErradas = 0;


		int* classes = new int[size[i]];

		stringstream gabfile;
		gabfile << "Instances/" << inst[i] << ".classes.txt";
		fstream gab(gabfile.str().c_str());
		FOR(j,0,size[i]){
			gab >> classes[j];
		}

		stringstream outfile;
		outfile << "InstancesConst/"  << inst[i] << "1.out.txt";
		fstream out(outfile.str().c_str());

		double v1, v2;
		out >> v1 >> v2;

		int a,b,c;
		double d;

		while(true){
			out >> a;
			if(!out.good()) break;
			out >> b >> c >> d;
			if(d < -1e-6){
				if(c == 1){
					if(classes[a] == classes[b]){
						MLCertas++;
					}else{
						MLErradas++;
					}
				}else{
					if(classes[a] != classes[b]){
						CLCertas++;
					}else{
						CLErradas++;
					}
				}
			}
		}
		//int totalErradas = total[i]/2;
		int totalErradas = ceil(0.05*total[i]);
		int totalCertas = total[i]-totalErradas;

		cout << endl << "-----------------" << inst[i]<< "-----------------" << endl;
		cout << "ML-Certas = " << MLCertas << "/" << totalCertas << "(" << fixed << setprecision(2) << (100.0*MLCertas)/totalCertas << "%)" << endl;
		cout << "ML-Erradas = " << MLErradas << "/" << totalErradas << "(" << fixed << setprecision(2) << (100.0*MLErradas)/totalErradas << "%)" << endl;
		cout << endl;
		cout << "Cl-Certas = " << CLCertas << "/" << totalCertas << "(" << fixed << setprecision(2) << (100.0*CLCertas)/totalCertas << "%)" << endl;
		cout << "Cl-Erradas = " << CLErradas << "/" << totalErradas << "(" << fixed << setprecision(2) << (100.0*CLErradas)/totalErradas << "%)" << endl;


	}



	return 0;
}
