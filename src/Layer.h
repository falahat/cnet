
/// @author Aryan Falahatpisheh

#ifndef CNET_LAYER_H
#define CNET_LAYER_H

#include "AFActivationFunction.h"
#include "AFMatrix.h"
#include <iostream>


class Layer {
public:

    /// The size of the vector that this layer takes as input
    int lenIn;

    /// The size of the vector that this layer outputs
    int lenOut;

    /// The intermediate gradients of the loss, `deltas[i] = d(Error)/d(sum_i)`
    double* deltas;

    /// The weights which are multiplied against the input values. This has `lenOut` rows and `lenIn` cols.
    AFMatrix<double>* weights;

    /// The weight gradients. `weightGradient[i,j] = d(Error)/d(weights[i,j])`. Same shape as `weights`.
    AFMatrix<double>* weightGradient;

    /// The activation function `g` such that `outputValues = g(weights * InputVals). Note that g takes a vector.
    AFActivationFunction activationFunction;


    /**
     *
     * @param lenIn The input length of this layer
     * @param lenOut The output size of this layer
     * @param activationFn Pass an AFActivationFunction by value so this layer knows how to calculate output values.
     */
    Layer(int lenIn,int lenOut, AFActivationFunction activationFn) {
        this->lenIn = lenIn;
        this->lenOut = lenOut;
        this->activationFunction = activationFn;

        if (!(this->deltas = new double[lenOut] )) {// TODO: Correct syntax?
            std::cout << "Not enough memory for deltas" << std::endl;
        };
        if (!(this->weights = new AFMatrix<double>(lenOut, lenIn))) {// TODO: Correct syntax?
            std::cout << "Not enough memory for weights" << std::endl;
        };
        if (!(this->weightGradient = new AFMatrix<double>(lenOut, lenIn))) {// TODO: Correct syntax?
            std::cerr << "Not enough memory for weight gradient" << std::endl;
        };

        this->randomizeWeights();
    }

    ~Layer() {
        delete this->weights;
        delete this->weightGradient;
        delete [] this->deltas;
    }


    void randomizeWeights() {
        for (int i = 0; i < lenIn*lenOut; ++i)
        {
            this->weights->vals[i] = (double)(rand() % 10000)/10000.0;
        }
    }

    double* forwardPass(double* inputVals) {
        return 0;
    }

    double* backpropagate(double* inputVals) {
        return 0;
    }

    double* backpropagateBase(double* inputVals) {
        return 0;
    }

    void updateWeights() {
        
    }


};


#endif //CNET_LAYER_H
