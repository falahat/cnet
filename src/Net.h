//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_NET_H
#define CNET_NET_H


#define LEARNING_RATE 0.0001
#include <vector>
#include "Layer.h"
#include "AFFunctions.h"
#include "AFMatrix.h"

// TODO: Compiler Macro??
template <typename T>
class Net {
public:
    int numLayers;
    vector<Layer*> *layers; // TODO: is this chill?
    // TODO: Free the activation and loss functions
    AFLossFunction<double> *lossFn;

    Net(vector<Layer*> *layers, AFLossFunction<double> *lossFunction) {
        this->layers = new vector<Layer*>(numLayers); // TODO: is this copying or changing reference or what?
        this->numLayers = layers->size();
        this->lossFn = lossFunction;
        this->layers = layers;
    }

    double trainSingle(vector<T> *inputVals, vector<T> *expectedVals) {
        // Forward propogation
        (*this->layers)[0]->forwardPass(inputVals);
        for (int i = 1; i < this->numLayers; ++i) {
            (*this->layers)[i]->forwardPass((*this->layers)[i-1]->outputVals);
        }
        vector<T> *actualVals = (*this->layers)[this->numLayers-1]->outputVals;
//        actualVals->reserve((*this->layers)[this->numLayers-1]->outputVals->size());
//        actualVals->assign((*this->layers)[this->numLayers-1]->outputVals->size(), 0);

        // Backpropogation and loss calculation
        double loss = this->lossFn->evaluate(actualVals, expectedVals);
        (*this->layers)[this->numLayers-1]->backpropagateBase(actualVals, expectedVals, this->lossFn);
        (*this->layers)[this->numLayers-1]->updateWeights(LEARNING_RATE);
        for (int i = this->numLayers-2; i >= 0; i--) {
            (*this->layers)[i]->backpropagate((*this->layers)[i+1]->deltas, (*this->layers)[i+1]->weights);
            (*this->layers)[i]->updateWeights(LEARNING_RATE);
        }

        cout << "Training..." << endl;
        cout << "Input: " << vectorToString<double>(*inputVals).str() << endl;
        cout << "Expected: " << vectorToString<double>(*expectedVals).str() << endl;
        cout << "Actual: " << vectorToString<double>(*actualVals).str() << endl;
        cout << "Loss: " << loss << endl;
        cout << "Weights: ";
        for (int i = 0; i < this->numLayers; ++i) {
            cout << (*this->layers)[i]->weights->toString().str() << endl;
        }
        cout << "Weight Gradients: ";
        for (int i = 0; i < this->numLayers; ++i) {
            cout << (*this->layers)[i]->weightGradient->toString().str() << endl;
        }

        cout << "Outputs: ";
        for (int i = 0; i < this->numLayers; ++i) {
            cout << vectorToString<double>(*(*this->layers)[i]->outputVals).str() << endl;
        }

        cout << "Inputs: ";
        for (int i = 0; i < this->numLayers; ++i) {
            cout << vectorToString<double>(*(*this->layers)[i]->inputVals).str() << endl;
        }

        cout << "Sums: ";
        for (int i = 0; i < this->numLayers; ++i) {
            cout << vectorToString<double>(*(*this->layers)[i]->sums).str() << endl;
        }

        cout << endl;

        return loss;
    }

    double trainEpoch(vector<vector<T>> *allInputVals, vector<vector<T>> *allExpectedVals) {
        for (int i = 0; i < allInputVals->size(); ++i) {
            shuffleInplace(allInputVals, allExpectedVals);
            vector<T> *inputVals = &(*allInputVals)[i];
            vector<T> *expectedVals = &(*allExpectedVals)[i];
            double loss = this->trainSingle(inputVals, expectedVals);
            cout << "The loss for this turn is " << loss << endl;
        }
    }

    double train(vector<vector<T>> *allInputVals, vector<vector<T>> *allExpectedVals, int numEpochs) {
        for (int i = 0; i < numEpochs; ++i) {
            this->trainEpoch(allInputVals, allExpectedVals);
        }
    }

};


#endif //CNET_NET_H
