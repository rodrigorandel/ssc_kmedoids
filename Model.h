/*
 * Model.h
 *
 *  Created on: 4 de abr de 2016
 *      Author: rodrigorandel
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <ilcplex/ilocplex.h>
#include <string>
#include "Instance.h"

using namespace std;

class Model {
public:
	Model(Instance *inst);
	~Model();

	void buildModel();
	void solve();
	double getOptValue();
	void print();
	bool isInteger();
	void exportModel();
	void addConstraints(char* constraint_file);

	void getCannotLinkRelaxationGap(double* value, string* name);
	void getMustLinkRelaxationGap(double* value, string* name);
private:

	IloEnv env;
	IloModel model;
	IloCplex solver;

	IloArray<IloNumVarArray> x;
	IloNumVarArray y;
//	IloArray<IloBoolVarArray> x;
//	IloBoolVarArray y;

	IloArray<IloRangeArray> mustLink;
	IloArray<IloRangeArray> cannotLink;

	Instance *inst;
	int N, M, P;
};

#endif /* MODEL_H_ */
