#include "CLI.h"


CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commandsVec.push_back(new UploadFile(dio));
    commandsVec.push_back(new Algorithm(dio));
    commandsVec.push_back(new DetectAnomalies(dio));
    commandsVec.push_back(new DisplayResults(dio));
    commandsVec.push_back(new AnalyzeResults(dio));
    commandsVec.push_back(new Exit(dio));
}

void CLI::start(){
    commonDetails info;
    int choice=0;
    while(choice!=5){
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        dio->write("1.upload a time series csv file\n"
                   "2.algorithm settings\n"
                   "3.detect anomalies\n"
                   "4.display results\n"
                   "5.upload anomalies and analyze results\n"
                   "6.exit\n");
        string input = dio->read();
        choice = input[0]-'0'-1;
        if(choice >=0 && choice <commandsVec.size()){
            commandsVec.at(choice)->execute(&info);
        }
    }
}


CLI::~CLI() {
    for(auto & i : commandsVec){
        delete i;
    }
}



