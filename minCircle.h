
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

Circle findMinCircle(Point **points, size_t size);

// you can add here additional methods
vector<Point> arrayToVec(Point **points, int size);

Circle welzl(vector<Point> &P, vector<Point> R, size_t size);

Circle welzl(vector<Point> P);

bool inCircle(Circle circle, Point point);

Point centerFromPoints(Point p1, Point p2, Point p3);

Circle trivialCircle(vector<Point> R);

float distance(Point p1, Point p2);


#endif /* MINCIRCLE_H_ */
