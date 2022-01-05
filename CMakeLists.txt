cmake_minimum_required(VERSION 3.20)
project(ex4)

set(CMAKE_CXX_STANDARD 11)

add_executable(ex5 MainTrain.cpp timeseries.cpp SimpleAnomalyDetector.cpp anomaly_detection_util.cpp minCircle.cpp HybridAnomalyDetector.cpp CLI.cpp expectedOutput.txt input.txt)


