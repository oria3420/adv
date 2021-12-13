/*
 * minCircle.cpp
 *
 * Author: Shira Taitelbaum, ID: 322207341
 *         Oriya Yehudai, ID: 211544150
 */

#include "minCircle.h"
#include <cmath>

using namespace std;

float distance(Point p1, Point p2) {
    float x = p1.x - p2.x;
    float y = p1.y - p2.y;
    return sqrt(x * x + y * y);
}

Circle findMinCircle(Point **points, size_t size) {
    vector<Point> P = arrayToVec(points, size);
    vector<Point> R;
    return welzl(P, R, size);
}

Circle welzl(vector<Point> &P, vector<Point> R, size_t size) {
    if (size == 0 || R.size() == 3) {
        Circle circle = trivialCircle(R);
        return circle;
    }
    int i = rand() % size;
    Point p = P.at(i);
    swap(P.at(i), P.at(size - 1));
    // P.erase(P.begin() + i);
    Circle d = welzl(P, R, size - 1);
    if (inCircle(d, p)) {
        return d;
    }
    R.push_back(p);
    return welzl(P, R, size - 1);
}

bool inCircle(Circle circle, Point point) {
    return distance(circle.center, point) < circle.radius;
}

Circle trivialCircle(vector<Point> R) {
    if (R.empty()) {
        Circle circle(Point(0, 0), 0);
        return circle;
    } else if (R.size() == 1) {
        Circle circle(R.at(0), 0);
        return circle;
    } else if (R.size() == 2) {
        Point p1 = R.at(0);
        Point p2 = R.at(1);
        float radius = distance(p1, p2) / 2;
        float xCenter = (p1.x + p2.x) / 2;
        float yCenter = (p1.y + p2.y) / 2;
        Circle circle(Point(xCenter, yCenter), radius);
        return circle;
    }
    Point p1 = R.at(0);
    Point p2 = R.at(1);
    Point p3 = R.at(2);
    Point center = centerFromPoints(p1, p2, p3);
    center.x += p1.x;
    center.y += p1.y;
    Circle circle(center, distance(center, p1));
    return circle;
}


Point centerFromPoints(Point p1, Point p2, Point p3) {
    float bx = p2.x - p1.x;
    float by = p2.y - p1.y;
    float cx = p3.x - p1.x;
    float cy = p3.y - p1.y;
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    float xPoint = (cy * B - by * C) / (2 * D);
    float yPoint = (bx * C - cx * B) / (2 * D);
    return Point(xPoint, yPoint);
}

vector<Point> arrayToVec(Point **points, int size) {
    vector<Point> pointsVec;
    for (int i = 0; i < size; i++) {
        float x = points[i]->x;
        float y = points[i]->y;
        Point p = Point(x, y);
        pointsVec.push_back(p);
    }
    return pointsVec;
}
