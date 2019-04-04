/*
 * ExecGraphics.cpp
 *
 *  Created on: 2017-02-13
 *      Author: randrodr
 */

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

#include "Helper.h"

using namespace std;

typedef struct{
	double ari;
	double ri;
	double delta;
	int times;
}Indices;

int main(){

	string instances[] = {"irisCL","irisML","controlCL","controlML"};
	int p[] = {3,3,6,6};
	int t[] = {2,2,3,3};
	int lines[] = {20,20,50,50};

	FOR(i,0,sizeof(instances)/sizeof(string)){
		vector<Indices> ranks(lines[i]);
		FOR(k,0,lines[i]){
			ranks[0].ari = 0;
			ranks[0].ri = 0;
			ranks[0].delta = 0;
			ranks[0].times = 0;
		}
		double sumsARI = 0;
		double sumsRI = 0;
		int times = 0;
		FOR(W,1,11){
			stringstream ranked;
			ranked << "SpecialResults/" << instances[i] << W << ".out.ranked.txt";
			fstream rankedFile(ranked.str().c_str());

			int rank = 0;
			FOR(l,0,lines[i]+1){
				int a,b,c;
				double v;
				rankedFile >> a;
				if(!rankedFile.good()){
					break;
				}
				if(a==-1){
					rank = 0;
					double tmp;
					rankedFile >> tmp;
					sumsARI += tmp;
					rankedFile >> tmp;
					sumsRI += tmp;
					times++;
					break;
				}
				rankedFile >> b >> c >> v;

				int a2,b2,c2;
				int pos=0;
				double v2,ari,ri;
				stringstream sorted;
				sorted << "SpecialResults/" << instances[i] << W << ".out.txt";
				fstream sortedFile(sorted.str().c_str());
				do{
					sortedFile >> a2 >> b2 >> c2 >> v2 >> ari >> ri;
				}while(a!=a2 || b!=b2 || c!=c2);

				ranks[rank].ari += ari;
				ranks[rank].ri += ri;
				ranks[rank].delta += v;
				ranks[rank].times++;

				rank++;
			}
		}
		cout << "%% INSTANCE: "<< instances[i] << endl  << "ARI = " << sumsARI/times << ";"<<endl << "RI = " << sumsRI/times <<  ";"<<endl;
		//cout << "%% CANNOT-LINK" << endl;
		{
			stringstream x,ari,ri,delta;
			x << "x = [";
			ari << "ari = [";
			ri << "ri = [";
			delta << "delta = [";
			int k=0;
			FOR(k,0,ranks.size()){
				ari << ranks[k].ari/ranks[k].times << " ";
				ri << ranks[k].ri/ranks[k].times << " ";
				delta << ranks[k].delta/ranks[k].times << " ";
				x << k+1 << " ";
			}
			ari << "];";
			ri << "];";
			delta << "];";
			x << "];";
			cout << x.str() << endl << ari.str() << endl << ri.str() << endl << delta.str() << endl;
			cout << "figure;\nplot(abs(delta),ari,'-p');hold on;grid on;"
					"\ntitle(\'"<<instances[i]<<"\');\nxlabel(\'|Dual \\Delta|\');ylabel(\'ARI\');"
					"\nline([min(abs(delta)),max(abs(delta))],[ARI,ARI],\'Color\',\'red\',\'LineStyle\',\'--\');" << endl;
		}

		//exit(1);

	}


	return 0;
}
