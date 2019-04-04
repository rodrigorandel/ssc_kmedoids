#ifndef _POINT_H
#define _POINT_H

#include <list>

using namespace std;

class Point {

private:
	double* coords;
	int dimension;
	int id;
	list<Point*> members;

public:
	Point(int id);
	Point(int id, int dimension);
	void setCoord(int, double);
	double getCoord(int);
	double* getCoords();
	double euclDist(Point*);
	double squaredDist(Point*);
//	double tangentDist(Point*);
	int getDimension();
	void setId(int);
	int getId();
	void insertPoint(Point*);
	void insertPoints(list<Point*>*);
	list<Point*>* getMembers();
	void print();
	~Point();
};
#endif
