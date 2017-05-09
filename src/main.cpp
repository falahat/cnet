#include <iostream>
#include "Loader.h"
#include "Net.h"

int main() {
    std::cout << "Hello, Beginning Program!" << std::endl;

    std::cout << "Loading Data..." << std::endl;
    Loader *loader = new Loader();
    vector<array<double,2>> *X = new vector<array<double,2>>();
    vector<array<double,1>> *Y = new vector<array<double,1>>();
    loader->loadXorData(X,Y);
    std::cout << "Data Loaded." << std::endl << std::endl;

    std::cout << "Creating Net..." << std::endl;
    // Type, Num Layers, L1 Input Size, L1 function, L2 Input Size, L2 Function, Output Size, Loss Fn
    Net<double,2,2,0,3,0,1,0> *net;
    net = new Net<double,2,2,0,3,0,1,0>(); // TODO: Explicitly declare?
    std::cout << "Net Created." << std::endl << std::endl;

    std::cout << "Training Net..." << std::endl;
    net->train(X, Y, 5);
    std::cout << "Net Trained." << std::endl << std::endl;

    return 0;

}