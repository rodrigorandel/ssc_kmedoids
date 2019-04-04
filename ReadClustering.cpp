/*
 * ReadClustering.cpp
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
	int n[] = {150,600,3823,4435};

	FOR(i,0,sizeof(instances)/sizeof(string)){

		int* classes = new int[n[i]];

		stringstream gabfile;
		gabfile << "Instances/" << instances[i] << ".classes.txt";
		fstream gab(gabfile.str().c_str());
		FOR(j,0,n[i]){
			gab >> classes[j];
		}



		stringstream clfile;
		clfile << "InstancesConst/" << instances[i] << "1.clustering.cl.out.txt";
		fstream cl(clfile.str().c_str());






		int cluster,size;
		double centroid;
		int a,b,c,id;
		double v,v2;
		cout << endl << "-----------------" << instances[i]<< "-----------------" << endl;
		while(true){
			cl >> cluster;
			if(!cl.good())break;
			cl >> size >> centroid;
			int CLCertas = 0;
			int CLErradas = 0;
			FOR(k,0,size){
				cl >> id >> v;
				stringstream infile;
				infile << "InstancesConst/" << instances[i] << "1.out.txt";
				fstream in(infile.str().c_str());

				string tmp;
				getline(in,tmp);
				int cont = 0;
				while(cont < id){
					getline(in,tmp); cont++;
				}
				in >> a >> b >> c >> v2;
				if(v != v2){
					cout << "v != v2 1\n";
					cout << a << " " << b << " " << c << " " << v << " " << v2 << endl;
					exit(1);
				}
				if(c == -1){
					if(classes[a] != classes[b]){
						CLCertas++;
					}else{
						CLErradas++;
					}
				}else{
					cout << "c == 1\n";
					exit(1);
				}
			}
			cout << "\t\t\tCluster " << cluster << "(centroid = " << centroid << ")" <<  endl;
			cout << "\t\t\t\t-Cl-Certas = " << CLCertas << "/" << size << "(" << fixed << setprecision(2) << (100.0*CLCertas)/size << "%)" << endl;
			if(cluster==0)
				cout << "Cannot-link";
			else
				cout << "\t";
			cout << "\t\t\t-Cl-Erradas = " << CLErradas << "/" << size << "(" << fixed << setprecision(2) << (100.0*CLErradas)/size << "%)" << endl;
		}
		cout << endl;
		stringstream mlfile;
		mlfile << "InstancesConst/" << instances[i] << "1.clustering.ml.out.txt";
		fstream ml(mlfile.str().c_str());
		while(true){
			ml >> cluster;
			if(!ml.good())break;
			ml >> size >> centroid;
			int MLCertas = 0;
					int MLErradas = 0;
			FOR(k,0,size){
				ml >> id >> v;
				stringstream infile;
				infile << "InstancesConst/" << instances[i] << "1.out.txt";
				fstream in(infile.str().c_str());
				string tmp;
				getline(in,tmp);

				do{
					in >> a >> b >> c >> v2;
				}while(c != 1);
				int cont = 0;
				while(cont < id){
					in >> a >> b >> c >> v2; cont++;
				}

				if(v != v2){
					cout << "v != v2 2\n";
					exit(1);
				}
				if(c == 1){
					if(classes[a] == classes[b]){
						MLCertas++;
					}else{
						MLErradas++;
					}
				}else{
					cout << "c == -1\n";
					exit(1);
				}
			}
			cout << "\t\t\tCluster " << cluster << "(centroid = " << centroid << ")" <<  endl;
			cout << "\t\t\t\t-Ml-Certas = " << MLCertas << "/" << size << "(" << fixed << setprecision(2) << (100.0*MLCertas)/size << "%)" << endl;
			if(cluster==0)
				cout << "Must-link";
			else
				cout << "\t";
			cout << "\t\t\t-Ml-Erradas = " << MLErradas << "/" << size << "(" << fixed << setprecision(2) << (100.0*MLErradas)/size << "%)" << endl;
		}

	}

	return 0;
}



