/*
 * CreateDistanceMatrix.cpp
 *
 *  Created on: 2017-07-31
 *      Author: randrodr
 */

#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "Point.h"
#include "Helper.h"

using namespace std;


int main(int argc, char *argv[]){

	fstream in(argv[1]); //data
	fstream cons(argv[2]); //constraints
	int k = atoi(argv[3]); //cluster

	int n,m;
	in >> n >> m;

	double** dist = new double*[n];
	FOR(i,0,n){
		dist[i] = (double*) calloc(n,sizeof(double));
	}


	int a, b, c;
	int lines = 0;
	while (cons.good()) {
		cons >> a;
		if(!cons.good()){
			break;
		}
		cons >> b >> c;
		lines++;
	}
	cons.clear();
	cons.seekg(0, ios::beg);
	double W = n/(k*lines);
	while (cons.good()) {
		cons >> a;
		if(!cons.good()){
			break;
		}
		cons >> b >> c;
		dist[a][b]= c*W;
		dist[b][a] = dist[a][b];
	}







	Point** points = new Point*[n];
	double att;
	FOR(i,0,n){
		points[i] = new Point(i,m);
		FOR(j,0,m){
			in >> att;
			points[i]->setCoord(j,att);
		}
	}

	cout << setprecision(6);
	FOR(i,0,n){
		FOR(j,0,n){
			dist[i][j] += exp(-(points[i]->squaredDist(points[j]))/2); //exp kernel
			if(i==j){
				dist[i][j] += -1; //
			}
		}
		cout << endl;
	}





}

