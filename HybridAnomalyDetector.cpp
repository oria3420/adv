
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {

}

HybridAnomalyDetector::~HybridAnomalyDetector() {

}

void HybridAnomalyDetector:: addCorrelatedFeatures(const TimeSeries &ts, long i, long c, float m, vector<Point> vecPoints){
    SimpleAnomalyDetector::addCorrelatedFeatures(ts,i,c,m, vecPoints);
    if(m>0.5 && m<0.9){
        Point* arrPoints = &vecPoints[0];
        Circle circle = findMinCircle(&arrPoints, vecPoints.size());
        float cX = circle.center.x;
        float cY = circle.center.y;
        correlatedFeatures corrf;
        corrf.feature1 = ts.getFeature(i);
        corrf.feature2 = ts.getFeature(c);
        corrf.corrlation = m;
        corrf.threshold = circle.radius*1.1;
        corrf.centerX = cX;
        corrf.centerY= cY;
        this->cf.push_back(corrf);
    }
}
