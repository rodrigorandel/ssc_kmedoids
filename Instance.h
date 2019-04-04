/*
 * Instance.h
 *
 *  Created on: 28/03/2016
 *      Author: rodrigorandel
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <vector>
#include "Point.h"

using namespace std;

class Instance {
private:
	/** Number of users */
	int N;
	/** Number of locations */
	int M;
	/** Number of open locations */
	int P;
	/** Number of dimensions */
	int S;

	int OLD_N;

	/**
	 * its a list because some of its elements may be removed after
	 *
	 */
	list<Point*>* users;
	Point** locations;

	vector<vector<double>> dists;
	vector<vector<double>> distance_matrix;

public:
	Instance(char * instace_file, int p, char * distances_file);
	~Instance();

	double distance(int u, int f);
	double intern_dist(int u1, int u2);

	int getM();
	int getN();
	int getOLD_N();
	int getP();
	int getS();

	list<Point*>* getUsers();
	Point** getLocations();
	void applyConstraints();
	void print();
};

#endif /* INSTANCE_H_ */
