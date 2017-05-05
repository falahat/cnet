cmake_minimum_required(VERSION 3.7)
project(CNet)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/Net.cpp src/Net.h src/Layer.cpp src/Layer.h src/AFActivationFunction.cpp src/AFActivationFunction.h src/AFMatrix.cpp src/AFMatrix.h)
add_executable(CNet ${SOURCE_FILES})