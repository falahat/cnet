
/// @author Aryan Falahatpisheh

#ifndef CNET_LAYER_H
#define CNET_LAYER_H

#include "AFFunctions.h"

#include "AFMatrix.h"
#include <iostream>


class Layer {

/**
 * Hello!
 */
    public:
        /// The size of the vector that this layer takes as input
        int lenIn;

        /// The size of the vector that this layer outputs
        int lenOut;

        /// The values that this layer receives from the previous layer LEN_IN
        vector<double> *inputVals;

        /// The sums after the weights are multiplied by input value`. LEN_OUT
        vector<double> *sums;

        /// The intermediate gradients of the loss, `deltas[i] = d(Error)/d(sum_i)` LEN_OUT
        vector<double> *deltas;

        /// The values after the sums are put through the activation function. LEN_OUT
        vector<double> *outputVals;

        /// The weights which are multiplied against the input values. This has `lenOut` rows and `lenIn` cols.
        AFMatrix<double> *weights;

        /// The weight gradients. `weightGradient[i,j] = d(Error)/d(weights[i,j])`. Same shape as `weights`.
        AFMatrix<double> *weightGradient;

        /// The activation function `g` such that `outputValues = g(weights * InputVals). Note that g takes a vector.
        AFActivationFunction<double> *activationFunction;


        /**
         *
         * @param lenIn The input length of this layer
         * @param lenOut The output size of this layer
         * @param activationFn Pass an AFActivationFunction by value so this layer knows how to calculate output values.
         */
        Layer(size_t lenIn, size_t lenOut, AFActivationFunction<double> *activationFunction) {
            this->lenIn = lenIn;
            this->lenOut = lenOut;
            this->activationFunction = activationFunction;

            this->inputVals = new vector<double>();
            inputVals->reserve(lenIn);
            inputVals->assign(lenIn, 0);

            this->sums = new vector<double>();
            this->sums->reserve(lenOut);
            this->sums->assign(lenOut, 0);

            this->deltas = new vector<double>();
            this->deltas->reserve(lenOut);
            this->deltas->assign(lenOut, 0);

            this->outputVals = new vector<double>();
            this->outputVals->reserve(lenOut);
            this->outputVals->assign(lenOut, 0);

            if (!(this->weights = new AFMatrix<double>(lenOut, lenIn))) {// TODO: Correct syntax?
                std::cout << "Not enough memory for weights" << std::endl;
            };
            if (!(this->weightGradient = new AFMatrix<double>(lenOut, lenIn))) {// TODO: Correct syntax?
                std::cerr << "Not enough memory for weight gradient" << std::endl;
            };

            this->randomizeWeights();
        }

        ~Layer() {
            delete this->inputVals;
            delete this->sums;
            delete this->deltas;
            delete this->outputVals;
            delete this->weights;
            delete this->weightGradient;
        }

        void randomizeWeights() {
            // TODO: Clarify references and pointers
            vector<double> *weightVals = this->weights->vals;
            for (int i = 0; i < lenIn * lenOut; ++i) {
                (*weightVals)[i] = (double) (rand() % 10000) / 10000.0;;
            }
        }

        /**
         * Will perform the forward pass on this Layer. Will take in `inputVals`, calculate weighted sums, and then pass
         * that result to this layer's activation function. The output will be written to `outputVals`.
         * @param inputVals
         * @param outputVals - outputVals[i] = this->weights.row(i).innerProduct(inputVals).
         */
        void forwardPass(vector<double> *inputVals, vector<double> *outputVals) {
            for (int i = 0; i < this->lenIn; ++i) {
                (*this->inputVals)[i] = (*inputVals)[i];
            }
            this->weights->innerProduct(inputVals, this->sums);
            this->activationFunction->evaluate(this->sums, outputVals);
        }


        /**
         * Will perform the forward pass on this Layer. Will take in `inputVals`, calculate weighted sums, and then pass
         * that result to this layer's activation function. The output will be written to `outputVals`.
         * @param inputVals
         * @param outputVals - outputVals[i] = this->weights.row(i).innerProduct(inputVals).
         */
        void forwardPass(vector<double> *inputVals) {
            this->forwardPass(inputVals, this->outputVals);
        }

        /**
         * Performs backpropogation algorithm. Writes this layer's new d(Err)/d(Sums) into `newDeltas`.
         * @tparam LEN_OUT_NEXT The next layer's output length
         * @param nextDeltas The next layer's d(Err)/d(sums);
         * @param nextWeights
         * @param newDeltas
         */
        void backpropagate(vector<double> *nextDeltas,
                           AFMatrix<double> *nextWeights,
                           vector<double> *newDeltas) {
            vector<double> valDerivatives; // d(Error)/d(actualVals)
            valDerivatives.reserve(this->lenOut);
            valDerivatives.assign(this->lenOut,0);
            this->activationFunction->derivative(this->sums, &valDerivatives);
            for (int i = 0; i < this->lenOut; ++i) {
                double currVal = 0;
                for (int j = 0; j < nextDeltas->size(); ++j) {
                    currVal += (*nextDeltas)[j] * (nextWeights->getValue(j, i)); // TODO: is the indexing right?
                }
                (*newDeltas)[i] = (currVal) * valDerivatives[i];
            }
        }

        /**
         * Performs backpropogation algorithm and writes output to `this->deltas`.
         * @tparam LEN_OUT_NEXT
         * @param nextDeltas
         * @param nextWeights
         */
        void backpropagate(vector<double> *nextDeltas,
                           AFMatrix<double> *nextWeights) {
            this->backpropagate(nextDeltas, nextWeights, this->deltas);
        }

        /**
         * The backprop algorithm for the last layer. First calculates `d(Err)/d(outputVals)`, which is the derivative
         * of the loss function w.r.t to `actualVals`. It then calculates d(Err)/d(sums)`.
         *
         * @param actualVals
         * @param expectedVals
         * @param newDeltas
         */
        void backpropagateBase(vector<double> *actualVals,
                               vector<double> *expectedVals,
                               AFLossFunction<double> *lossFn,
                               vector<double> *newDeltas) {

            vector<double> lossDerivatives(this->lenOut); // d(Error)/d(actualVals)
            lossDerivatives.reserve(this->lenOut);
            lossDerivatives.assign(this->lenOut, 0);
            lossFn->derivative(actualVals, expectedVals, &lossDerivatives);

            vector<double> valDerivatives(this->lenOut); // d(actualVals)/d(sums)
            valDerivatives.reserve(this->lenOut);
            valDerivatives.assign(this->lenOut, 0);

            this->activationFunction->derivative(this->sums, &valDerivatives);
            // TODO: Vectorize this operation
            for (int i = 0; i < this->lenOut; ++i) {
                (*newDeltas)[i] = lossDerivatives[i] * valDerivatives[i];
            }
        }

        /**
         * The backprop algorithm for the last layer. First calculates `d(Err)/d(outputVals)`, which is the derivative
         * of the loss function w.r.t to `actualVals`. It then calculates d(Err)/d(sums)`.
         *
         * @param actualVals
         * @param expectedVals
         * @param newDeltas
         */
        void backpropagateBase(vector<double> *actualVals,
                               vector<double> *expectedVals,
                               AFLossFunction<double> *lossFn) {

            this->backpropagateBase(actualVals, expectedVals, lossFn, this->deltas);
        }

        void updateWeights(double learningRate) {

            for (int row = 0; row < this->lenOut; ++row) {
                for (int col = 0; col < this->lenIn; ++col) {
                    double newVal = (*this->deltas)[row] * (*this->inputVals)[col];
                    this->weightGradient->setValue(row, col, newVal * learningRate);
                }
            }

            this->weights->subtract(this->weightGradient, this->weights);
        }
};



#endif //CNET_LAYER_H
