cmake_minimum_required(VERSION 3.20)
project(ex4)

set(CMAKE_CXX_STANDARD 11)

add_executable(ex4 MainTrain.cpp timeseries.cpp SimpleAnomalyDetector.cpp anomaly_detection_util.cpp minCircle.cpp HybridAnomalyDetector.cpp)


