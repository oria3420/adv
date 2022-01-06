

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector(): SimpleAnomalyDetector(){}

    virtual ~HybridAnomalyDetector();

    virtual void addCorrelatedFeatures(const TimeSeries &ts, long i, long c, float m, vector<Point> vecPoints);

    virtual bool isAnomaly(Point p, correlatedFeatures c);

    void setCorr(float threshold){
        this->threshold=threshold;
    }

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
