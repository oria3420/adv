//
// Created by oriya on 10/14/21.
//
#include <vector>

#ifndef UNTITLED1_ANOMALY_DETECTION_UTIL_H
#define UNTITLED1_ANOMALY_DETECTION_UTIL_H


// returns the variance of X and Y
float var(float *x, int size);

// returns the covariance of X and Y
float cov(float *x, float *y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size);

class Line {
public:
    // fields
    float a, b;

    // default constructor
    Line() : a(0), b(0) {}

    // constructor
    Line(float a, float b) : a(a), b(b) {}

    // f computes line equation
    float f(float x) {
        return a * x + b;
    }
};

class Point {
public:
    // fields
    float x, y;

    // constructor
    Point(float x, float y) : x(x), y(y) {}
};

// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size);

Line linear_reg(std::vector<Point> vecPoints, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size);

// returns the deviation between point p and the line
float dev(Point p, Line l);

#endif //UNTITLED1_ANOMALY_DETECTION_UTIL_H
