//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_AFACTIVATIONFUNCTION_H
#define CNET_AFACTIVATIONFUNCTION_H

using namespace std;
#include <math.h>
#include <vector>

//TODO: THis is annoyingly verbose.

template <typename T>
class AFActivationFunction {
public:
    AFActivationFunction() {

    }
    virtual void evaluate(vector<T> *input, vector<T> *output) {
    }

    virtual void derivative(vector<T> *input, vector<T> *output) {
    }
};

template <typename T>
class ReLU : public AFActivationFunction<T> {
public:
    void evaluate(vector<T> *input, vector<T> *output) {
        for (int i = 0; i < input->size(); ++i) {
            if ((*input)[i] > 0) {
                (*output)[i] = (*input)[i];
            } else {
                (*output)[i] = 0;
            }
        }
    }

    void derivative(vector<T> *input, vector<T> *output) {
        for (int i = 0; i < input->size(); ++i) {
            if ((*input)[i] > 0) {
                (*output)[i] = 1;
            } else {
                (*output)[i] = 0;
            }
        }
    }
};


template <typename T>
class LeakyReLU : public AFActivationFunction<T> {
public:
    double leakFactor;
    LeakyReLU() {

    }

    LeakyReLU(double leakFactor) {
        this->leakFactor = leakFactor;
    }
    void evaluate(vector<T> *input, vector<T> *output) {
        for (int i = 0; i < input->size(); ++i) {
            if ((*input)[i] > 0) {
                (*output)[i] = (*input)[i];
            } else {
                (*output)[i] = this->leakFactor*(*input)[i];
            }
        }
    }

    void derivative(vector<T> *input, vector<T> *output) {
        for (int i = 0; i < input->size(); ++i) {
            if ((*input)[i] > 0) {
                (*output)[i] = 1;
            } else {
                (*output)[i] = this->leakFactor;
            }
        }
    }
};

template <typename T>
class IdentityFunction: public AFActivationFunction<T> {
public:
    void evaluate(vector<T> *input, vector<T> *output) {
        // TODO: How are some ways to copy two arrays?
        for (int i = 0; i < input->size(); ++i) {
            (*output)[i] = (*input)[i];
        }
    }

    void derivative(vector<T> *input, vector<T> *output) {
        for (int i = 0; i < input->size(); ++i) {
            (*output)[i] = 1;
        }
    }
};


template <typename T>
class Softmax: public AFActivationFunction<T> {
public:
    void evaluate(vector<T> *input, vector<T> *output) {
        // TODO: How are some ways to copy two arrays?
        // TODO: This won't handle 0-length arrays

//        T maxVal = (*input)[0];
        T sum = 0;
        for (int i = 0; i < input->size(); ++i) {
            T currInput = (*input)[i];
            // TODO: Add numerical stability
            T currVal = currInput;
            (*output)[i] = currVal;
            sum += currVal;
        }

        for (int i = 0; i < input->size(); ++i) {
            (*output)[i] /= sum;
        }
    }

    void derivative(vector<T> *input, vector<T> *output) {
        vector<T> evaluated;
        evaluated.reserve(output->size());
        evaluated.assign(output->size(), 0);
        this->evaluate(input, &evaluated);
        for (int i = 0; i < input->size(); ++i) {
            (*output)[i] = 1;
        }
    }
};



template <typename T>
class AFLossFunction {
public:
    virtual T evaluate(vector<T> *actualVals, vector<T> *expectedVals) {
    }

    virtual void derivative(vector<T> *actualVals, vector<T> *expectedVals, vector<T> *output) {
    }
};


template <typename T>
class AFSquareLossFunction : public AFLossFunction<T> {
public:
    T evaluate(vector<T> *actualVals, vector<T> *expectedVals) {
        T ans = 0;
        for (int i = 0; i < actualVals->size(); ++i) {
            double diff = (*actualVals)[i] - (*expectedVals)[i];
            ans += diff*diff*0.5;
        }
    }

    /**
     * Finds derivative of squared loss L(inputVal, actualVal, expectedVal) w.r.t. actualVals.
     * @param actualVals
     * @param expectedVals
     * @param output
     */
    void derivative(vector<T> *actualVals, vector<T> *expectedVals, vector<T> *output) {
        for (int i = 0; i < output->size(); ++i) {
            double diff =  (*actualVals)[i] - (*expectedVals)[i] ;
            (*output)[i] = diff;
        }
    }
};



template <typename T>
class CrossEntropyLoss : public AFLossFunction<T> {
public:
    T evaluate(vector<T> *actualVals, vector<T> *expectedVals) {
        T ans = 0;
        for (int i = 0; i < actualVals->size(); ++i) {
            ans -= (*expectedVals)[i] * log( (*actualVals)[i] );
        }
    }

    /**
     * Finds derivative of squared loss L(inputVal, actualVal, expectedVal) w.r.t. actualVals.
     * @param actualVals
     * @param expectedVals
     * @param output
     */
    void derivative(vector<T> *actualVals, vector<T> *expectedVals, vector<T> *output) {
        for (int i = 0; i < output->size(); ++i) {
            double val =  (*actualVals)[i] / (*expectedVals)[i] ;
            (*output)[i] = val;
        }
    }
};



template <typename T>
AFActivationFunction<T> *createActivationFunction(int activationFnCode) {
    // TODO: Make `activationFnCode` an enum, not int
    if (activationFnCode == 0) {
        return new ReLU<T>();
    }
};


template <typename T>
AFLossFunction<T> *createLossFunction(int lossFnCode) {
    // TODO: Make `activationFnCode` an enum, not int
    if (lossFnCode == 0) {
        return new AFSquareLossFunction<T>();
    }
};


#endif //CNET_AFACTIVATIONFUNCTION_H
