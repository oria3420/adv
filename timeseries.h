

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <utility>

class TimeSeries{

public:

    const char* CSVfileName;
    vector<pair<string , vector<float>>> dataBase;

	explicit TimeSeries(const char* CSVfileName):CSVfileName(CSVfileName){
        this->dataBase=createDataBase();
	}

    vector<pair<string, vector<float>>> createDataBase();
    long getNumOfFeatures() const;
    long getNumOfVals() const;
    string getFeature(long i) const;
    vector<float> getValVector(string feature) const;
    vector<string> getFeatureArray() const;
    vector<float> getValVector(long i) const;
};



#endif /* TIMESERIES_H_ */
