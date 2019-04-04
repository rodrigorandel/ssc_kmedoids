/*
 * Model.cpp
 *
 *  Created on: 4 de abr de 2016
 *      Author: rodrigorandel
 */

#include "Model.h"

#include <iostream>
#include <algorithm>

#include "Constraints.h"
#include "Helper.h"

using namespace std;

Model::Model(Instance *inst) {
	this->inst = inst;
	model = IloModel(env, "P-median");
	solver = IloCplex(model);
	solver.setParam(IloCplex::RootAlg,CPX_ALG_DUAL);
	solver.setParam(IloCplex::TiLim,86400);
	solver.setOut(env.getNullStream());
	solver.setParam(solver.Threads,8);

	N = inst->getN();
	M = inst->getM();
	P = inst->getP();
}

Model::~Model() {
	env.end();
}

void Model::buildModel() {

	x = IloArray<IloNumVarArray>(env, N);
//	x = IloArray<IloBoolVarArray>(env,N);
	FOR(i,0,N)
	{
		x[i] = IloNumVarArray(env, M, 0, 1);
//		x[i] = IloBoolVarArray(env,M);
		FOR(j,0,M)
		{
			stringstream s;
			s << "x_" << i << "_" << j;
			x[i][j].setName(s.str().c_str());
		}
	}
	y = IloNumVarArray(env, M, 0, 1);
//	y = IloBoolVarArray(env,M);
	FOR(j,0,M)
	{
		stringstream s;
		s << "y_" << j;
		y[j].setName(s.str().c_str());
	}

	FOR(i,0,N)
	{ // (2)
		model.add(IloSum(x[i]) == 1);
	}
	FOR(i,0,N)
	{ // (3)
		FOR(j,0,M)
		{
			model.add(x[i][j] <= y[j]);
		}
	}
	model.add(IloSum(y) == P); //(4)

	IloExpr obj = IloExpr(env);
	FOR(i,0,N)
	{
		FOR(j,0,M)
		{
			obj += x[i][j] * inst->intern_dist(i, j);
		}
	}
	this->model.add(IloMinimize(env, obj));

}

void Model::solve() {
	solver.solve();
}

void Model::print() {
	FOR(j,0,M)
	{
		if (solver.getValue(y[j]) > 1e-4) {
			cout << y[j].getName() << " = " << solver.getValue(y[j]) << endl;
		}
	}
	cout << scientific << setprecision(5) << "Objective value = "
			<< solver.getObjValue() << endl;
}

bool Model::isInteger() {

	FOR(i,0,N)
	{
		FOR(j,0,M)
		{
			if (solver.getValue(x[i][j]) > 1e-4
					&& solver.getValue(x[i][j]) < 0.99) {
				//cout << x[i][j].getName() << " = " << solver.getValue(x[i][j]) << endl;
				return false;
			}
		}
	}
	FOR(j,0,M)
	{
		if (solver.getValue(y[j]) > 1e-4 && solver.getValue(y[j]) < 0.99) {
			//cout << y[j].getName() << " = " << solver.getValue(y[j]) << endl;
			return false;
		}
	}
	return true;
}

void Model::exportModel() {
	solver.exportModel("model.lp");
}

void Model::addConstraints(char* constraint_file) {
	ifstream fcin(constraint_file);
	if (!fcin.good()) {
		cout << "Constraint file '" << constraint_file << "' not found" << endl;
		exit(1);
	}

	if(!fcin.eof()){
		mustLink = IloArray<IloRangeArray>(env);
		cannotLink = IloArray<IloRangeArray>(env);
	}


	int a, b, c;
	while (fcin.good()) {
		fcin >> a;
		if(!fcin.good()){
			break;
		}
		fcin >> b >> c;
		if (c == CANNOT_LINK) {
			IloRangeArray cnl(env);
			FOR(j,0,inst->getM()){
				IloRange r = x[a][j] + x[b][j] - y[j] <= 0.1;
				stringstream s;
				//s << "x_" << a << "_" << j << "_diff_x_" << b << "_" << j;
				s << a << " " << b << " -1";
				r.setName(s.str().c_str());
				cnl.add(r);
			}
			cannotLink.add(cnl);
		} else {
			IloRangeArray ml(env);
			FOR(j,0,inst->getM()){
				IloRange r = x[a][j] - x[b][j] <= 0.1;
				stringstream s;
				s << a << " " << b << " 1";
				//s << "x_" << a << "_" << j << "_equal_x_" << b << "_" << j;
				r.setName(s.str().c_str());
				ml.add(r);

				IloRange r1 = x[b][j] - x[a][j] <= 0.1;
				stringstream s1;
				s1 << "x_" << b << "_" << j << "_equal_x_" << a << "_" << j;
				r1.setName(s1.str().c_str());
				ml.add(r1);
			}
			mustLink.add(ml);
		}
	}

	if (mustLink.getSize() > 0) {
		FOR(i,0,mustLink.getSize()){
			model.add(mustLink[i]);
		}
	} else {
		mustLink.clear();
	}
	if (cannotLink.getSize() > 0) {
		FOR(i,0,cannotLink.getSize()){
			model.add(cannotLink[i]);
		}
	} else {
		cannotLink.clear();
	}
}

typedef struct{
	double gap;
	string name;
}Const;

bool compare(Const a, Const b){
	return (a.gap<b.gap);
}

void Model::getCannotLinkRelaxationGap( double* gap, string* name){
	vector<Const> consts;
	*name = "none";
	*gap = -1e-6;

	FOR(i,0,cannotLink.getSize()){
		double sum = 0;
		FOR(j,0,cannotLink[i].getSize()){
			sum += solver.getDual(cannotLink[i][j]);
			//cout << cannotLink[i][j].getName() << "\t\t" << solver.getDual(cannotLink[i][j]) << endl;
		}
		Const c;
		c.gap = sum;
		c.name = cannotLink[i][0].getName();
		consts.push_back(c);

		if(sum < *gap){
			*gap = sum;
			*name = cannotLink[i][0].getName();
		}
	}
	sort(consts.begin(),consts.end(),compare);

	//cout << "-------------- Cannot-Link-----------------" << endl;
	FOR(i,0,consts.size()){
		while(consts[i].name.find(" ") != string::npos){
			consts[i].name.replace(consts[i].name.find(" "),1,"\t");
		}
		cout << consts[i].name << "\t\t" << consts[i].gap << endl;
	}
	//cout << endl;


}

void Model::getMustLinkRelaxationGap(double* gap, string* name){
	vector<Const> consts;
	*name = "none";
	*gap = -1e-6;
	FOR(i,0,mustLink.getSize()){
		double sum = 0;
		FOR(j,0,mustLink[i].getSize()){
			sum += solver.getDual(mustLink[i][j]);
			//cout << mustLink[i][j].getName() << "\t\t" << solver.getDual(mustLink[i][j]) << endl;
		}
		//cout << endl;
		//if(sum < -1e-6){
			Const c;
			c.gap = sum;
			c.name = mustLink[i][0].getName();
			consts.push_back(c);
		//}
		if(sum < *gap){
			*gap = sum;
			*name = mustLink[i][0].getName();
		}
	}
	sort(consts.begin(),consts.end(),compare);
	//cout << "-------------- Must-Link-----------------" << endl;
	FOR(i,0,consts.size()){
		while(consts[i].name.find(" ") != string::npos){
			consts[i].name.replace(consts[i].name.find(" "),1,"\t");
		}
		cout << consts[i].name << "\t\t" << consts[i].gap << endl;
	}
	//cout << endl;
}

double Model::getOptValue() {
	return solver.getObjValue();
}
