#include "Point.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Helper.h"

using namespace std;

Point::Point(int ID) {
	id = ID;
	dimension = 0;
	coords = nullptr;
}

Point::Point(int ID, int s) {
	id = ID;
	dimension = s;
	coords = new double[dimension];
	//    coords = (double*)calloc(dimension, sizeof(double));
}

void Point::setCoord(int k, double value) {

	if (k >= dimension) {
		cerr << "Error on acessing the dimension of the point" << endl;
	} else {
		coords[k] = value;
	}

}

double Point::getCoord(int k) {

	if (k >= dimension) {
		cerr << "Error on acessing the dimension of the point" << endl;
		return 0;
	} else {
		return coords[k];
	}
}

//double Point::tangentDist(Point* p) {
//	double background = 0.0;
//	int choice[] = { 1, 1, 1, 1, 1, 1, 0, 0, 0 };
//	return tangentDistance(coords, p->getCoords(), 243, 320, choice, background);
//}

double Point::euclDist(Point* p) {

	double sum = 0;
	for (int i = 0; i < dimension; i++) {
		sum += pow(coords[i] - p->getCoord(i), 2);
	}
	return sqrt(sum);
}

double Point::squaredDist(Point* p) {

	double sum = 0;
	for (int i = 0; i < dimension; i++) {
		sum += pow(coords[i] - p->getCoord(i), 2);
	}
	return sum;
}


int Point::getDimension() {
	return dimension;
}

void Point::setId(int i) {
	id = i;
}

int Point::getId() {
	return id;
}

list<Point*>* Point::getMembers() {
	return &members;
}

void Point::print() {
	cout << "P" << id << "( ";
	for (int l = 0; l < dimension; l++) {
		if (l != dimension - 1) {
			cout << coords[l] << " ";
		} else {
			cout << coords[l];
		}
	}
	cout << ") [ ";
	list<Point*>::iterator it = members.begin();
	for (; it != members.end(); it++) {
		cout << (*it)->getId() << ", ";
	}
	cout << "] " << members.size() + 1 << endl;
}

Point::~Point() {
	FOR_EACH(list<Point*>,m,members){
		delete *m;
	}
	delete [] coords;
}

void Point::insertPoint(Point* p) {
	members.push_back(p);
}

void Point::insertPoints(list<Point*>* ps) {
	members.insert(members.begin(), ps->begin(), ps->end());
}

double* Point::getCoords() {
	return coords;
}
