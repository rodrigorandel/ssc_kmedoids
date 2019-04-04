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



	string instances[] = {"iris","control","optical"};
	int p[] = {3,6,10};
	int t[] = {10,60,300};
	int lines[] = {40,160,500};

	FOR(i,0,sizeof(instances)/sizeof(string)){

		FOR(W,1,3){
			stringstream targets[3];
			targets[0] << "t"<<i << W<<" = [";

			stringstream outputs[3];
			outputs[0] << "o"<<i << W<<" = [";

			stringstream ranked;
			ranked << "ResultsConfusion/" << instances[i] << W << ".out.ranked.txt";
			fstream rankedFile(ranked.str().c_str());

			double normARI,normRI,t;
			rankedFile >> t >> normARI >> normRI;
			FOR(l,0,lines[i]){
				int a,b,c;
				double v;
				rankedFile >> a;
				if(!rankedFile.good()){
					break;
				}
				rankedFile >> b >> c >> v;

				int a2,b2,c2;
				int pos=0;
				double v2,ari,ri;
				stringstream sorted;
				sorted << "ResultsConfusion/" << instances[i] << W << ".out.txt";
				fstream sortedFile(sorted.str().c_str());
				do{
					sortedFile >> a2 >> b2 >> c2 >> v2 >> ari >> ri;
					pos++;
				}while(a!=a2 || b!=b2 || c!=c2);

				//if(v < -1e-6){
				if(((t-v2)/t) >= 0.01){
					targets[0] << "1 ";
					targets[1] << "0 ";
					targets[2] << "0 ";
				}else{
					targets[0] << "0 ";
					targets[1] << "1 ";
					targets[2] << "0 ";
				}
//				if(ari > normARI){
//					outputs[0] << "1 ";
//					outputs[1] << "0 ";
//					outputs[2] << "0 ";
//				}else if(abs(ari-normARI) <= 1e-6){
//					outputs[0] << "0 ";
//					outputs[1] << "1 ";
//					outputs[2] << "0 ";
//				}else{
//					outputs[0] << "0 ";
//					outputs[1] << "0 ";
//					outputs[2] << "1 ";
//				}
				if(ari > normARI){
					outputs[0] << "1 ";
					outputs[1] << "0 ";
					outputs[2] << "0 ";
				}else if(abs(ari-normARI) <= 1e-6){
					outputs[0] << "0 ";
					outputs[1] << "1 ";
					outputs[2] << "0 ";
				}else{
					outputs[0] << "0 ";
					outputs[1] << "0 ";
					outputs[2] << "1 ";
				}
			}

			targets[0] << ";";
			targets[1] << ";";
			targets[2] << "];";
			outputs[0] << ";";
			outputs[1] << ";";
			outputs[2] << "];";

			cout << "%% INSTANCE: "<< instances[i] << W << endl;
			cout << targets[0].str() << targets[1].str() << targets[2].str() << endl << outputs[0].str() << outputs[1].str() << outputs[2].str() << endl;
			cout << "figure;\nplotconfusion(t" <<i << W<<",o"<<i << W<<",\'" << instances[i] << W << "\');";
			//cout << "xticks([1 2 3]); yticklabels({\'ARI\\uparrow\',\'ARI=\',\'ARI\\downarrow\',\'\'});yticks([1 2 3 4]);	xticklabels({\'\\Delta < 0\',\'\\Delta = 0\',\'\'})" << endl;
			cout << "saveas(gcf,\'confusion-"<<instances[i] << W<<".jpg\');" << endl;

		}

//		cout << "%% INSTANCE: "<< instances[i] << endl  << "ARI = " << sumsARI/times << ";"<<endl << "RI = " << sumsRI/times <<  ";"<<endl;
//		//cout << "%% CANNOT-LINK" << endl;
//		{
//			stringstream x,ari,ri,delta;
//			x << "x = [";
//			ari << "ari = [";
//			ri << "ri = [";
//			delta << "delta = [";
//			int k=0;
//			FOR(k,0,ranks.size()){
//				ari << ranks[k].ari/ranks[k].times << " ";
//				ri << ranks[k].ri/ranks[k].times << " ";
//				delta << ranks[k].delta/ranks[k].times << " ";
//				x << k+1 << " ";
//			}
//			ari << "];";
//			ri << "];";
//			delta << "];";
//			x << "];";
//			cout << x.str() << endl << ari.str() << endl << ri.str() << endl << delta.str() << endl;
//			cout << "figure;\nplot(abs(delta),ari,'-p');hold on;grid on;"
//					"\ntitle(\'"<<instances[i]<<"\');\nxlabel(\'|Dual \\Delta|\');ylabel(\'ARI\');"
//					"\nline([min(abs(delta)),max(abs(delta))],[ARI,ARI],\'Color\',\'red\',\'LineStyle\',\'--\');" << endl;
//		}

		//exit(1);

	}
	//plot << ");" << endl;
	//cout << plot.str();


	return 0;
}
