//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_NET_H
#define CNET_NET_H

#include <vector>
#include "Layer.h"
#include "AFFunctions.h"

// TODO: Compiler Macro??
template <
        typename T,
        int NUMLAYERS,
        size_t IN_SIZE_0,
        int FN_0,
        size_t IN_SIZE_1,
        int FN_1,
        size_t OUT_SIZE,
        int LOSSFN>
class Net {
public:
    int numLayers;
    array<AbstractLayer,NUMLAYERS> layers; // TODO: is this chill?
    // TODO: Free the activation and loss functions
    AFLossFunction<double, OUT_SIZE> *lossFn;

    Net() {
        this->layers = layers; // TODO: is this copying or changing reference or what?
        this->numLayers = NUMLAYERS;
        this->lossFn = createLossFunction<double, OUT_SIZE>(LOSSFN);
        AFActivationFunction<T,IN_SIZE_0> *fn0 = createActivationFunction<double,IN_SIZE_0>(FN_0);
        AFActivationFunction<T,IN_SIZE_1> *fn1 = createActivationFunction<double,IN_SIZE_1>(FN_1);

        // TODO: Make some compiler MACRO for where we just do a for loop
        layers[0] = new Layer<IN_SIZE_0, IN_SIZE_1>(IN_SIZE_0 , IN_SIZE_1, fn0);
        layers[1] = new Layer<IN_SIZE_1, OUT_SIZE>(IN_SIZE_1 , OUT_SIZE, fn1);
    }

    double trainSingle(array<T, IN_SIZE_0> *inputVals, array<T, OUT_SIZE> *expectedVals) {

        // Forward propogation
        this->layers[0]->forwardPass(inputVals);
        for (int i = 1; i < NUMLAYERS; ++i) {
            this->layers[i]->forwardPass(this->layers[i-1].outputVals);
        }
        array<T, OUT_SIZE> *actualVals = this->layers[NUMLAYERS-1].outputVals;

        // Backpropogation and loss calculation
        double loss = this->lossFn->evaluate(actualVals, expectedVals);
        this->layers[NUMLAYERS-1].backpropogateBase(actualVals, expectedVals);
        for (int i = NUMLAYERS-2; i >= 0; i--) {
            this->layers[i]->backpropogate(this->layers[i+1].deltas, this->layers[i+1].weights);
        }
        
        return loss;
    }

    double trainEpoch(vector<array<T, IN_SIZE_0>> *allInputVals, vector<array<T, OUT_SIZE>> *allExpectedVals) {
        for (int i = 0; i < allInputVals->size(); ++i) {
            array<T, IN_SIZE_0> *inputVals = &(*allInputVals)[i];
            array<T, OUT_SIZE> *expectedVals = &(*allInputVals)[i];
            double loss = this->trainSingle(inputVals, expectedVals);
            cout << "The loss for this turn is " << loss << endl;
        }
    }

    double train(vector<array<T, IN_SIZE_0>> *allInputVals, vector<array<T, OUT_SIZE>> *allExpectedVals, int numEpochs) {
        for (int i = 0; i < numEpochs; ++i) {
            this->trainEpoch(allInputVals, allExpectedVals);
        }
    }

};


#endif //CNET_NET_H
