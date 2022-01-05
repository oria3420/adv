

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <iomanip>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <cstring>

#include <mutex>
#include <fstream>
#include <iostream>

using namespace std;

struct commonDetails {
    int numOfRows;
    float threshold = 0.9;
    vector<AnomalyReport> ar;
    vector<pair<long, long>> mergedAnomlies;
};

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
    int uploadFile(string fileName) {
        int numOfRows = 0;
        ofstream file(fileName);
        string line = read();
        while (line != "done") {
            file << line + "\n";
            line = read();
            numOfRows++;
        }
        file.close();
        return numOfRows - 1;
    }
};

// you may add here helper classes


// you may edit this class
class Command {
protected:
    DefaultIO *dio;

public:
    string descr;

    Command(DefaultIO *dio, string descr) : dio(dio), descr(descr) {}

    virtual void execute(commonDetails* info) = 0;

    virtual ~Command() {}
};

// implement here your command classes
class UploadFile : public Command {
public:
    UploadFile(DefaultIO *dio) : Command(dio, "upload a time series csv file") {}

    void execute(commonDetails* info) {
        dio->write("Please upload your local train CSV file.\n");
        info->numOfRows = dio->uploadFile("anomalyTrain.csv\n");
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        dio->uploadFile("anomalyTest.csv\n");
        dio->write("Upload complete.\n");
    }
};

class Algorithm : public Command {
public:
    Algorithm(DefaultIO *dio) : Command(dio, "algorithm settings") {}

    void execute(commonDetails* info) {
        bool valid = false;
        dio->write("The current correlation threshold is ");
        dio->write(info->threshold);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        string newThreshold = dio->read();
        float i = stof(newThreshold);
        while (!valid) {
            if (i > 1 || i <= 0) {
                dio->write("please choose a value between 0 and 1.\n");
            } else {
                info->threshold = i;
                valid = true;
            }
        }
    }
};

class DetectAnomalies : public Command {
public:
    DetectAnomalies(DefaultIO *dio) : Command(dio, "detect anomalies") {}

    void execute(commonDetails* info) {
        TimeSeries train("anomalyTrain.csv");
        HybridAnomalyDetector hybridDetect(info->threshold);
        hybridDetect.learnNormal(train);

        TimeSeries test("anomalyTest.csv");
        dio->write("anomaly detection complete.\n");
        info->ar = hybridDetect.detect(test);
    }
};

class DisplayResults : public Command {
public:
    DisplayResults(DefaultIO *dio) : Command(dio, "display results") {}

    void execute(commonDetails* info) {
        for (int i = 0; i < info->ar.size(); i++) {
            dio->write(info->ar.at(i).timeStep);
            dio->write("\t" + info->ar.at(i).description + "\n");
        }
        dio->write("Done.\n");
    }
};

class AnalyzeResults : public Command {
    vector<pair<long, long>> timeSteps;
    long N = 0;
    long TP = 0;
    long FP = 0;
    long P = 0;
public:
    AnalyzeResults(DefaultIO *dio) : Command(dio, "upload anomalies and analyze results") {}

    void execute(commonDetails* info) {
        mergeAnomolies(info);
        dio->write("Please upload your local anomalies file.\n");
        string line = dio->read();
        long n = info->numOfRows;
        while (line != "done") {
            int diff = createTimeStepsVec(line);
            line = dio->read();
            P++;
            n = n - diff;
        }
        N = n;
        analyze(info);
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        float tpr=((int)(1000.0*TP/P))/1000.0f;
        float fpr=((int)(1000.0*FP/N))/1000.0f;
        dio->write(tpr);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(fpr);
        dio->write("\n");
        N = 0;
        TP = 0;
        FP = 0;
        P = 0;
        timeSteps.clear();
        info->mergedAnomlies.clear();
    }

    void mergeAnomolies(commonDetails* info) {
        info->mergedAnomlies.emplace_back(info->ar.at(0).timeStep, info->ar.at(0).timeStep);
        for (int i = 1; i < info->ar.size(); i++) {
            AnomalyReport prev = info->ar.at(i - 1);
            AnomalyReport current = info->ar.at(i);
            if (prev.timeStep + 1 == current.timeStep && prev.description == current.description) {
                int index = info->mergedAnomlies.size();
                info->mergedAnomlies.at(index-1).second = current.timeStep;
            } else {
                info->mergedAnomlies.emplace_back(current.timeStep, current.timeStep);
            }
        }
    }

    void analyze(commonDetails* info) {
        for (int i = 0; i < info->mergedAnomlies.size(); i++) {
            bool isTP = false;
            long ma1 = info->mergedAnomlies.at(i).first;
            long ma2 = info->mergedAnomlies.at(i).second;
            for (int j = 0; j < timeSteps.size(); j++) {
                long ts1 = timeSteps.at(j).first;
                long ts2 = timeSteps.at(j).second;
                if (!((ts2 < ma1) || (ts1 > ma2))) {
                    TP++;
                    isTP = true;
                    break;
                }
            }
            if(!isTP){
                FP++;
            }
        }
    }

    int createTimeStepsVec(string line) {
        string sx;
        string sy;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                if(i-1 != 0){
                    sx = line.substr(0, i);
                }
                else{
                    sx = line[0];
                }
                if(i+1 != line.length() - 1){
                    sy = line.substr(i + 1, line.length() - 1);
                }
                else{
                    sy = line[i + 1];
                }
                break;
            }
        }
        int x = stoi(sx);
        int y = stoi(sy);
        timeSteps.push_back({x, y});
        return y - x + 1;
    }

};

class Exit : public Command {
public:
    Exit(DefaultIO *dio) : Command(dio, "exit") {}

    void execute(commonDetails* info) {
    }
};


#endif /* COMMANDS_H_ */
