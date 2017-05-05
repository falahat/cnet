//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_AFACTIVATIONFUNCTION_H
#define CNET_AFACTIVATIONFUNCTION_H

using namespace std;
#include <vector>
#include <math.h>

class AFActivationFunction {

    void evaluate(vector<double> input, vector<double> output) {
    }

    void derivative(vector<double> input, vector<double> output) {
    }
};

class ReLU: AFActivationFunction {
    void evaluate(vector<double> input, vector<double> output) {
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] > 0) {
                output[i] = input[i];
            } else {
                output[i] = 0;
            }
        }
    }

    void derivative(vector<double> input, vector<double> output) {
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] > 0) {
                output[i] = 1;
            } else {
                output[i] = 0;
            }
        }
    }
};

#endif //CNET_AFACTIVATIONFUNCTION_H
