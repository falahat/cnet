
/// @author Aryan Falahatpisheh

#ifndef CNET_LAYER_H
#define CNET_LAYER_H

#include "AFFunctions.h"

#include "AFMatrix.h"
#include <iostream>

class AbstractLayer {
public:
    //TODO: Are these void* s okay?
    void* outputVals;
    void* deltas;
    void* weights;
    AbstractLayer() {

    }

    virtual void forwardPass(void* dudInput) {

    }


};
template <size_t LEN_IN, size_t LEN_OUT>
class Layer : public AbstractLayer{

/**
 * Hello!
 */
public:
    /// The size of the vector that this layer takes as input
    int lenIn;

    /// The size of the vector that this layer outputs
    int lenOut;

    /// The values that this layer receives from the previous layer
    array<double, LEN_IN> *inputVals;

    /// The sums after the weights are multiplied by input value`
    array<double, LEN_OUT> *sums;

    /// The intermediate gradients of the loss, `deltas[i] = d(Error)/d(sum_i)`
    array<double, LEN_OUT> *deltas;

    /// The values after the sums are put through the activation function.
    array<double, LEN_OUT> *outputVals;

    /// The weights which are multiplied against the input values. This has `lenOut` rows and `lenIn` cols.
    AFMatrix<double, LEN_OUT, LEN_IN> *weights;

    /// The weight gradients. `weightGradient[i,j] = d(Error)/d(weights[i,j])`. Same shape as `weights`.
    AFMatrix<double, LEN_OUT, LEN_IN> *weightGradient;

    /// The activation function `g` such that `outputValues = g(weights * InputVals). Note that g takes a vector.
    AFActivationFunction<double,LEN_OUT> *activationFunction;


    /**
     *
     * @param lenIn The input length of this layer
     * @param lenOut The output size of this layer
     * @param activationFn Pass an AFActivationFunction by value so this layer knows how to calculate output values.
     */
    Layer(size_t lenIn,size_t lenOut, int activationFnCode) {
        this->lenIn = lenIn;
        this->lenOut = lenOut;
        this->activationFunction = createActivationFunction<double, LEN_OUT>(activationFnCode);

        if (!(this->inputVals = new array<double, LEN_IN> )) {// TODO: Correct syntax?
            std::cout << "Not enough memory for inputVals" << std::endl;
        };
        if (!(this->outputVals = new array<double, LEN_OUT> )) {// TODO: Correct syntax?
            std::cout << "Not enough memory for outputVals" << std::endl;
        };
        if (!(this->deltas = new array<double, LEN_OUT> )) {// TODO: Correct syntax?
            std::cout << "Not enough memory for deltas" << std::endl;
        };
        if (!(this->sums = new array<double, LEN_OUT> )) {// TODO: Correct syntax?
            std::cout << "Not enough memory for sums" << std::endl;
        };
        if (!(this->weights = new AFMatrix<double, LEN_OUT, LEN_IN>())) {// TODO: Correct syntax?
            std::cout << "Not enough memory for weights" << std::endl;
        };
        if (!(this->weightGradient = new AFMatrix<double, LEN_OUT, LEN_IN>()) ) {// TODO: Correct syntax?
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
        array<double, LEN_IN*LEN_OUT> *weightVals = this->weights->vals;
        for (int i = 0; i < lenIn*lenOut; ++i)
        {
            (*weightVals)[i] = (double)(rand() % 10000)/10000.0;;
        }
    }

    /**
     * Will perform the forward pass on this Layer. Will take in `inputVals`, calculate weighted sums, and then pass
     * that result to this layer's activation function. The output will be written to `outputVals`.
     * @param inputVals
     * @param outputVals - outputVals[i] = this->weights.row(i).innerProduct(inputVals).
     */
    void forwardPass(array<double, LEN_IN> *inputVals, array<double, LEN_OUT> *outputVals) {
        for (int i = 0; i < LEN_IN; ++i) {
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
    template <size_t OTHER_OUT>
    void forwardPass(array<double, LEN_IN> *inputVals, Layer<LEN_OUT, OTHER_OUT> layer) {
        for (int i = 0; i < LEN_IN; ++i) {
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
    void forwardPass(array<double, LEN_IN> *inputVals) {
        this->forwardPass(inputVals, this->outputVals);
    }

    /**
     * Performs backpropogation algorithm. Writes this layer's new d(Err)/d(Sums) into `newDeltas`.
     * @tparam LEN_OUT_NEXT The next layer's output length
     * @param nextDeltas The next layer's d(Err)/d(sums);
     * @param nextWeights
     * @param newDeltas
     */
    template <size_t LEN_OUT_NEXT>
    void backpropagate(array<double, LEN_IN> *nextDeltas,
                       AFMatrix<double, LEN_OUT_NEXT, LEN_IN> *nextWeights,
                       array<double, LEN_OUT> *newDeltas) {
        array<double, LEN_OUT> valDerivatives; // d(Error)/d(actualVals)
        this->activationFunction->derivative(this->sums, &valDerivatives);
        for (int i = 0; i < LEN_OUT; ++i) {
            double currVal = 0;
            for (int j = 0; j < LEN_OUT_NEXT; ++j) {
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
    template <size_t LEN_OUT_NEXT>
    void backpropagate(array<double, LEN_IN> *nextDeltas,
                       AFMatrix<double, LEN_OUT_NEXT, LEN_IN> *nextWeights) {
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
    void backpropagateBase(array<double, LEN_OUT> *actualVals,
                           array<double, LEN_OUT> *expectedVals,
                           AFLossFunction<double,LEN_OUT> *lossFn,
                           array<double, LEN_OUT> *newDeltas) {

        array<double, LEN_OUT> lossDerivatives; // d(Error)/d(actualVals)
        lossFn->derivative(actualVals, expectedVals, &lossDerivatives);

        array<double, LEN_OUT> valDerivatives; // d(actualVals)/d(sums)
        this->activationFunction->derivative(this->sums, &valDerivatives);
        // TODO: Vectorize this operation
        for (int i = 0; i < LEN_OUT; ++i) {
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
    void backpropagateBase(array<double, LEN_OUT> *actualVals,
                           array<double, LEN_OUT> *expectedVals,
                           AFLossFunction<double,LEN_OUT> *lossFn) {

        this->backpropagateBase(actualVals, expectedVals, lossFn, this->deltas);
    }

    void updateWeights(double learningRate) {
        for (int row = 0; row < LEN_OUT; ++row) {
            for (int col = 0; col < LEN_IN; ++col) {
                double newVal = (*this->deltas)[row] * (*this->inputVals)[col];
                this->weightGradient->setValue(row, col, newVal * learningRate);
            }
        }

        this->weights->subtract(this->weightGradient, this->weights);
    }


};


#endif //CNET_LAYER_H
