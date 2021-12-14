/*
 * timeseries.cpp
 *
 * Author: Shira Taitelbaum, ID: 322207341
 *         Oriya Yehudai, ID: 211544150
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-string-compare"

#include "timeseries.h"
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <algorithm>

long TimeSeries::getNumOfFeatures() const {
    return this->dataBase.size();
}

vector<float> TimeSeries::getValVector(long i) const {
    return this->dataBase.at(i).second;
}

vector<float> TimeSeries::getValVector(string feature) const {
    vector <string> featuresArray = getFeatureArray();
    for (long i = 0; i < featuresArray.size(); i++) {
        if (featuresArray.at(i) == (feature)) {
            return dataBase.at(i).second;
        }
    }
}

vector <string> TimeSeries::getFeatureArray() const {
    vector <string> featuresArray;
    for (long i = 0; i < this->dataBase.size(); i++) {
        string f = this->dataBase.at(i).first;
        featuresArray.push_back(f);
    }
    return featuresArray;
}

long TimeSeries::getNumOfVals() const {
    if (!this->dataBase.empty()) {
        return this->dataBase.at(1).second.size();
    }
    return 0;
}

string TimeSeries::getFeature(long i) const {
    return this->dataBase.at(i).first;
}

vector <pair<string, vector < float>>>

TimeSeries::createDataBase() {
    vector < pair < string, vector < float>>> dataBase;
    ifstream myFile(this->CSVfileName);
    if (!myFile.is_open()) {
        throw runtime_error("");
    }
    string line;
    string feature;
    float val;
    if (myFile.good()) {
        getline(myFile, line);
        stringstream ss(line);
        while (getline(ss, feature, ',')) {
            dataBase.push_back({feature, vector < float > {}});
        }
    }
    while (getline(myFile, line)) {
        stringstream ss(line);
        long colIdx = 0;
        while (ss >> val) {
            dataBase.at(colIdx).second.push_back(val);
            if (ss.peek() == ',') {
                ss.ignore();
            }
            colIdx++;
        }
    }
    myFile.close();
    return dataBase;
}


#pragma clang diagnostic pop