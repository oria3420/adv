
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {

}

HybridAnomalyDetector::~HybridAnomalyDetector() {

}

Point **vecToArray(vector<Point> vec) {
    int numOfVals = vec.size();
    Point **arr = new Point *[vec.size()];
    for (int i = 0; i < numOfVals; i++) {
        arr[i] = new Point(vec[i].x, vec[i].y);
    }
    return arr;
}

void
HybridAnomalyDetector::addCorrelatedFeatures(const TimeSeries &ts, long i, long c, float m, vector<Point> vecPoints) {
    SimpleAnomalyDetector::addCorrelatedFeatures(ts, i, c, m, vecPoints);
    if (m > 0.5 && m < 0.9) {
        Point **arrPoints = vecToArray(vecPoints);
        Circle circle = findMinCircle(arrPoints, vecPoints.size());
        correlatedFeatures corrf;
        corrf.feature1 = ts.getFeature(i);
        corrf.feature2 = ts.getFeature(c);
        corrf.corrlation = m;
        corrf.threshold = circle.radius * 1.1;
        corrf.centerX = circle.center.x;
        corrf.centerY = circle.center.y;
        this->cf.push_back(corrf);
    }
}

bool HybridAnomalyDetector::isAnomaly(Point p, correlatedFeatures c) {
    if (c.corrlation >= 0.9 && SimpleAnomalyDetector::isAnomaly(p, c)) {
        return true;
    } else if (c.corrlation > 0.5 && c.corrlation < 0.9 && distance(p, Point(c.centerX, c.centerY)) > c.threshold) {
        return true;
    }
    return false;
}
