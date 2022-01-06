

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <cstring>

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
    int numOfRows=0;
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
    int uploadFile(const string& fileName) {
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
    explicit UploadFile(DefaultIO *dio) : Command(dio, "upload a time series csv file") {}

    void execute(commonDetails* info) override {
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
    explicit Algorithm(DefaultIO *dio) : Command(dio, "algorithm settings") {}

    void execute(commonDetails* info) override {
        bool valid = false;
        dio->write("The current correlation threshold is ");
        dio->write(info->threshold);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        string newThreshold = dio->read();
        float i;
        if(!newThreshold.empty()){
            i = stof(newThreshold);
        }
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
    explicit DetectAnomalies(DefaultIO *dio) : Command(dio, "detect anomalies") {}

    void execute(commonDetails* info) override {
        if(!info->ar.empty()){
            info->ar.clear();
        }
        if(!info->mergedAnomlies.empty()){
            info->mergedAnomlies.clear();
        }
        TimeSeries train("anomalyTrain.csv");
        HybridAnomalyDetector hybridDetect;
        hybridDetect.setCorr(info->threshold);
        hybridDetect.learnNormal(train);

        TimeSeries test("anomalyTest.csv");
        dio->write("anomaly detection complete.\n");
        info->ar = hybridDetect.detect(test);
        mergeAnomolies(info);
    }

    static void mergeAnomolies(commonDetails* info) {
        info->mergedAnomlies.emplace_back(info->ar.at(0).timeStep, info->ar.at(0).timeStep);
        for (int i = 1; i < info->ar.size(); i++) {
            AnomalyReport* prev = &info->ar.at(i );
            AnomalyReport* current = &info->ar.at(i);
            if (prev->timeStep + 1 == current->timeStep && prev->description == current->description) {
                int index = info->mergedAnomlies.size();
                info->mergedAnomlies.at(index-1).second = current->timeStep;
            } else {
                info->mergedAnomlies.emplace_back(current->timeStep, current->timeStep);
            }
        }
    }
};

class DisplayResults : public Command {
public:
    explicit DisplayResults(DefaultIO *dio) : Command(dio, "display results") {}

    void execute(commonDetails* info) override {
        for (auto & i : info->ar) {
            dio->write(i.timeStep);
            dio->write("\t" + i.description + "\n");
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
    explicit AnalyzeResults(DefaultIO *dio) : Command(dio, "upload anomalies and analyze results") {}

    void execute(commonDetails* info) override {
        if(!timeSteps.empty()){
            timeSteps.clear();
        }
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

        float truePositiveRate = (1000*TP/P)/1000.0;
        float falsePositiveRate =(1000*FP/N)/1000.0;

        dio->write(truePositiveRate);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(falsePositiveRate);
        dio->write("\n");
        N = 0;
        TP = 0;
        FP = 0;
        P = 0;
    }

    void analyze(commonDetails* info) {
        for (auto & mergedAnomlie : info->mergedAnomlies) {
            bool isTP = false;
            long ma1 = mergedAnomlie.first;
            long ma2 = mergedAnomlie.second;
            for (auto & timeStep : timeSteps) {
                long ts1 = timeStep.first;
                long ts2 = timeStep.second;
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
        int x;
        int y;
        if(!sx.empty()){
            x = stoi(sx);
        }
        if(!sy.empty()){
            y = stoi(sy);
        }
        timeSteps.emplace_back(x, y);
        return y - x + 1;
    }

};

class Exit : public Command {
public:
    explicit Exit(DefaultIO *dio) : Command(dio, "exit") {}

    void execute(commonDetails* info) override {
    }
};


#endif /* COMMANDS_H_ */
