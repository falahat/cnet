#include <iostream>
#include "Loader.h"
#include "Net.h"

int main() {
    std::cout << "Hello, Beginning Program!" << std::endl;

    std::cout << "Loading Data..." << std::endl;
    Loader *loader = new Loader();
    vector<vector<double>> *X = new vector<vector<double>>();
    vector<vector<double>> *Y = new vector<vector<double>>();
    loader->loadLineData(X,Y, 3.5, 0.0);
    std::cout << "Data Loaded." << std::endl << std::endl;

    std::cout << "Creating Net..." << std::endl;
    Net<double> *net;
    AFActivationFunction<double> *relu = new ReLU<double>();
    vector<Layer*> *layers = new vector<Layer*>();
    layers->push_back(new Layer(1,1,relu));
    layers->push_back(new Layer(1,1,relu));

    AFLossFunction<double> *lossFunction = new AFSquareLossFunction<double>();

    net = new Net<double>(layers, lossFunction); // TODO: Explicitly declare?
    std::cout << "Net Created." << std::endl << std::endl;

    std::cout << "Training Net..." << std::endl;
    net->train(X, Y, 10);
    std::cout << "Net Trained." << std::endl << std::endl;

    return 0;

}