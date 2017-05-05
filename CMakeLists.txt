cmake_minimum_required(VERSION 3.7)
project(CNet)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Net.cpp Net.h Layer.cpp Layer.h AFActivationFunction.cpp AFActivationFunction.h)
add_executable(CNet ${SOURCE_FILES})