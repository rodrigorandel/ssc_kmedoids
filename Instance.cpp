/*
 * Instance.cpp
 *
 *  Created on: 28/03/2016
 *      Author: rodrigorandel
 */

#include "Instance.h"

#include <iostream>
#include <iomanip>
#include <fstream>

#include "Helper.h"

using namespace std;




Instance::Instance(char *instance_file, int p, char * distances_file) {
	P = p;
	N = S = 0;

	ifstream fcin(instance_file);
	if (!fcin.good()) {
		cerr << "Instance file '" << instance_file << "' not found" << endl;
		exit(1);
	}
	fcin >> N >> S;
	M =  OLD_N = N;
	if (P > N) {
		cerr << "Number of medians is greater than the number of entities."	<< endl;
		exit(1);
	}

	users = new list<Point*>;
	if(distances_file == nullptr){
		locations = new Point*[M];
		double att;
		FOR(i,0,N){
			Point* user = new Point(i,S);
			locations[i] = new Point(i,S);
			FOR(l,0,S){
				fcin >> att;
				locations[i]->setCoord(l, att);
				user->setCoord(l, att);
			}
			users->push_back(user);
		}
	}else{
		locations = nullptr;
		FOR(i,0,N){
			Point* user = new Point(i);
			users->push_back(user);
		}
	}

//	norm(users,S);
//	exit(1);

	fcin.close();
	dists.resize(N);

	if(distances_file == nullptr){
		int i = 0;
		FOR_EACH_P(list<Point*>, u, users){
			dists[i] = vector<double>(M);
			FOR(j,0,M) {
				//cout << setprecision(7) << scientific << locations[j]->squaredDist(*u) << "\t";
				dists[i][j] = locations[j]->squaredDist(*u);
				//dists[i][j] = locations[j]->euclDist(*u);
			}
			//cout << endl;
			i++;
		}
		//exit(1);
	}else{
		fcin.open(distances_file);
		if (!fcin.good()) {
			cerr << "Distance Matrix file '" << distances_file << "' not found" << endl;
			exit(1);
		}
		FOR(i,0,N){
			dists[i] = vector<double>(M);
			FOR(j,0,M){
				fcin >> dists[i][j];
			}
		}
		fcin.close();
	}
}

Instance::~Instance() {
	FOR_EACH_P(list<Point*>, u, users){
		delete *u;
	}
	delete users;
	if(locations != nullptr){
		FOR(j,0,M) {
			delete locations[j];
		}
		delete [] locations;
	}
}

void Instance::applyConstraints() {

	N = users->size();
	distance_matrix.resize(N);
	int i=0;
	FOR_EACH_P(list<Point*>, u, users){
		distance_matrix[i] = vector<double>(dists[(*u)->getId()]);
		FOR_EACH_P(list<Point*>,su,(*u)->getMembers()) {
			FOR(j,0,M) {
				distance_matrix[i][j] += dists[(*su)->getId()][j];
			}
		}
		i++;
	}
}

double Instance::distance(int u, int f) {
	return distance_matrix[u][f];
}

double Instance::intern_dist(int u1, int u2){
	return dists[u1][u2];
}

int Instance::getM() {
	return M;
}

int Instance::getN() {
	return N;
}

int Instance::getOLD_N() {
	return OLD_N;
}

int Instance::getP() {
	return P;
}

int Instance::getS() {
	return S;
}

Point** Instance::getLocations() {
	return locations;
}

list<Point*>* Instance::getUsers() {
	return users;
}

void Instance::print() {
	cout << setprecision(3) << fixed;
	cout << "\nPoints:" << endl;
	FOR_EACH_P(list<Point*>, u, users){
		(*u)->print();
	}
	cout
	<< "--------------------------------------------------------------------"
	<< endl;
	cout << "\nDists\n";
	FOR(j,0,M)
	{
		cout << "\t" << j;
	}
	cout << endl;
	FOR(i,0,N)
	{
		cout << i;
		FOR(j,0,M)
		{
			cout << "\t" << distance_matrix[i][j];
		}
		cout << endl;
	}
}
