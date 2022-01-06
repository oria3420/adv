/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Shira Taitelbaum, ID: 322207341
 *         Oriya Yehudai, ID: 211544150
 */

#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold=0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {

}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    //number of feature
    long numOfFeatures = ts.getNumOfFeatures();
    //number of rows
    long numOfVal = ts.getNumOfVals();
    for (long i = 0; i < numOfFeatures; i++) {
        float m = 0;
        long c = 0;
        for (long j = i + 1; j < numOfFeatures; j++) {
            vector<float> v1 = ts.getValVector(i);
            vector<float> v2 = ts.getValVector(j);
            float *a1 = v1.data();
            float *a2 = v2.data();
            float p = abs(pearson(a1, a2, numOfVal));
            if (p > m) {
                m = p;
                c = j;
            }
        }
        vector<Point> vecPoints = sharedPoints(ts.getValVector(i), ts.getValVector(c));
        addCorrelatedFeatures(ts, i, c, m, vecPoints);
    }
}

vector<Point> SimpleAnomalyDetector::sharedPoints(vector<float> vec1, vector<float> vec2) {
    vector<Point> vecPoints;
    for (long i = 0; i < vec1.size(); i++) {
        float x = vec1.at(i);
        float y = vec2.at(i);
        Point p = Point(x, y);
        vecPoints.push_back(p);
    }
    return vecPoints;
}

void
SimpleAnomalyDetector::addCorrelatedFeatures(const TimeSeries &ts, long i, long c, float m, vector<Point> vecPoints) {
    if (m > threshold) {
        long numOfPoints = vecPoints.size();
        float threshold = 0;
        Line line = linear_reg(vecPoints, numOfPoints);
        for (long j = 0; j < numOfPoints; j++) {
            Point p = Point(vecPoints.at(j).x, vecPoints.at(j).y);
            float tempReg = dev(p, line);
            if (tempReg > threshold) {
                threshold = tempReg;
            }
        }
        threshold = threshold * 1.1;
        string feature1 = ts.getFeature(i);
        string feature2 = ts.getFeature(c);
        correlatedFeatures corrf = {feature1, feature2, m, line, threshold};
        this->cf.push_back(corrf);
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> arVec;
    for (correlatedFeatures c: this->cf) {
        string feature1 = c.feature1;
        string feature2 = c.feature2;
        vector<float> vf1 = ts.getValVector(feature1);
        vector<float> vf2 = ts.getValVector(feature2);
        vector<Point> vecPoints = sharedPoints(vf1, vf2);
        for (long i = 0; i < vf1.size(); i++) {
            Point p = Point(vecPoints.at(i).x, vecPoints.at(i).y);
            if (isAnomaly(p, c)) {
                AnomalyReport ar = AnomalyReport(feature1 + "-" + feature2, i + 1);
                arVec.push_back(ar);
            }
        }
    }
    return arVec;
}

bool SimpleAnomalyDetector::isAnomaly(Point p, correlatedFeatures c) {
    float devNum = dev(p, c.lin_reg);
    if (devNum > c.threshold) {
        return true;
    }
    return false;
}

