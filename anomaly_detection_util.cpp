/*
 * anomaly_detection_util.cpp
 *
 * Author: Shira Taitelbaum, ID: 322207341
 *         Oriya Yehudai, ID: 211544150
 */

#include <cmath>
#include "anomaly_detection_util.h"


/***
 *  average: This function computes the average of some values
 *  args: *x- array of float values
 *        size - amount of elements
 ***/
float average(float *x, int size) {
    float sum = 0;
    // calculate the sum of the values
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    // divide the sum by number of elements to get the average
    return sum / size;
}

/***
 *  var: This function computes the variance of X (by using a formula)
 *  args: *x- array of float values
 *        size - amount of elements
 ***/
float var(float *x, int size) {
    // the next code is according to a given formula in the ex's PFFs
    float u = average(x, size);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += (x[i] - u) * (x[i] - u);
    }
    return sum / size;
}

/***
 *  var: This function computes the covariance of X and Y (by using a formula)
 *  args: *x- array of float values
 *        *y- array of float values
 *        size - amount of elements
 ***/
float cov(float *x, float *y, int size) {
    // the next code is according to a given formula in the ex's PFFs
    float averX = average(x, size);
    float averY = average(y, size);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i] * y[i];
    }
    float averXY = sum / size;
    return averXY - averX * averY;
}

/***
 *  pearson: This function computes the correlation of X and Y (by using a formula)
 *  args: *x- array of float values
 *        *y- array of float values
 *        size - amount of elements
 ***/

float pearson(float *x, float *y, int size) {
    // the next code is according to a given formula in the ex's PFFs
    float sqrtVarX = sqrt(var(x, size));
    float sqrtVarY = sqrt(var(y, size));
    return cov(x, y, size) / (sqrtVarX * sqrtVarY);
}

/***
 *  linear_reg: This function computes line's equation by array of points and size
 *  args: **points- array of points
 *        size - amount of elements
 ***/
Line linear_reg(Point **points, int size) {
    // an array to include the x values of the points
    float x[size];
    // an array to include the y values of the points
    float y[size];
    // copy the values of the points to X and Y array
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    // the next code is according to a given formula in the ex's PFFs
    float averX = average(x, size);
    float averY = average(y, size);
    float a = cov(x, y, size) / var(x, size);
    float b = averY - a * averX;
    // return the line's equation
    return Line(a, b);
}

Line linear_reg(std::vector<Point> vecPoints, int size) {
    // an array to include the x values of the points
    float x[size];
    // an array to include the y values of the points
    float y[size];
    // copy the values of the points to X and Y array
    for (int i = 0; i < size; i++) {
        x[i] = vecPoints.at(i).x;
        y[i] = vecPoints.at(i).y;
    }
    // the next code is according to a given formula in the ex's PFFs
    float averX = average(x, size);
    float averY = average(y, size);
    float a = cov(x, y, size) / var(x, size);
    float b = averY - a * averX;
    // return the line's equation
    return Line(a, b);
}

/***
 *  dev: This function returns the deviation between point p and the line (by using a formula)
 *  args: point - the point
 *        line - the line
 ***/
float dev(Point point, Line line) {
    // the next code is according to a given formula in the ex's PFFs
    float fx = line.f(point.x);
    float y = point.y;
    float diff = fx - y;
    if (diff < 0) {
        diff = -diff;
    }
    return diff;
}

/***
 *  dev: This function returns the deviation between point p and the line equation of the points
 *       by using the other dev function
 *  args: point - the point
 *        line - the line
 ***/
float dev(Point p, Point **points, int size) {
    // compute the line equation
    Line line = linear_reg(points, size);
    // use dev function with different arguments
    dev(p, line);
}
