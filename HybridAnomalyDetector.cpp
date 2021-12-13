
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {

}

HybridAnomalyDetector::~HybridAnomalyDetector() {

}

Point** vecToArray(vector<Point> vec){
    int numOfVals = vec.size();
    Point** arr =new Point*[vec.size()];
    for(int i=0;i<numOfVals;i++){
        arr[i]=new Point(vec[i].x,vec[i].y);
    }
    return arr;
}

void HybridAnomalyDetector:: addCorrelatedFeatures(const TimeSeries &ts, long i, long c, float m, vector<Point> vecPoints){
    SimpleAnomalyDetector::addCorrelatedFeatures(ts,i,c,m, vecPoints);
    if(m>0.5 && m<0.9){
        Point** arrPoints = vecToArray(vecPoints);
        Circle circle = findMinCircle(arrPoints, vecPoints.size());
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

int HybridAnomalyDetector::isAnomaly(Point p, correlatedFeatures c) {
    bool isAnomalySimple = SimpleAnomalyDetector::isAnomaly(p,c);
    if(isAnomalySimple && c.threshold>=0.9){
        return 1;
    }
    bool isAnomalyHybrid;
    float dis = distance(p, Point(c.centerX, c.centerY));
    if ( dis > c.threshold) {
        isAnomalyHybrid = true;
    }
    else if(isAnomalyHybrid && c.threshold>0.5 && c.threshold<0.9){
        return 1;
    }
    return 0;
}
