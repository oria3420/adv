

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector(float threshold): SimpleAnomalyDetector(threshold){}

    virtual ~HybridAnomalyDetector();

    virtual void addCorrelatedFeatures(const TimeSeries &ts, long i, long c, float m, vector<Point> vecPoints);

    virtual bool isAnomaly(Point p, correlatedFeatures c);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
