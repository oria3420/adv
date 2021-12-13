

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void addCorrelatedFeatures(const TimeSeries &ts, long i, long c, float m, vector<Point> vecPoints);
    virtual int isAnomaly(Point p, correlatedFeatures c);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
