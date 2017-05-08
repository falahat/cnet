//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_AFACTIVATIONFUNCTION_H
#define CNET_AFACTIVATIONFUNCTION_H

using namespace std;
#include <math.h>
#include <array>

//TODO: THis is annoyingly verbose.

template <typename T, size_t N>
class AFActivationFunction {
public:
    virtual void evaluate(array<T, N> *input, array<T, N> *output) {
    }

    virtual void derivative(array<T, N> *input, array<T, N> *output) {
    }
};

template <typename T, size_t N>
class ReLU : public AFActivationFunction<T,N> {
public:
    void evaluate(array<T, N> *input, array<T, N> *output) {
        for (int i = 0; i < N; ++i) {
            if ((*input)[i] > 0) {
                (*output)[i] = (*input)[i];
            } else {
                (*output)[i] = 0;
            }
        }
    }

    void derivative(array<T, N> *input, array<T, N> *output) {
        for (int i = 0; i < N; ++i) {
            if ((*input)[i] > 0) {
                (*output)[i] = 1;
            } else {
                (*output)[i] = 0;
            }
        }
    }
};

template <typename T, size_t N>
class IdentityFunction: public AFActivationFunction<T,N> {
public:
    void evaluate(array<T,N> *input, array<T,N> *output) {
        // TODO: How are some ways to copy two arrays?
        for (int i = 0; i < N; ++i) {
            (*output)[i] = (*input)[i];
        }
    }

    void derivative(array<T,N> *input, array<T,N> *output) {
        for (int i = 0; i < N; ++i) {
            (*output)[i] = 1;
        }
    }
};

template <typename T, size_t N>
class AFLossFunction {
public:
    void evaluate(array<T,N> *actualVals, array<T,N> *expectedVals, array<T,N> *output) {
    }

    void derivative(array<T,N> *actualVals, array<T,N> *expectedVals, array<T,N> *output) {
    }
};


template <typename T, size_t N>
class AFSquareLossFunction : AFLossFunction<T,N> {
public:
    T evaluate(array<T,N> *actualVals, array<T,N> *expectedVals) {
        T ans = 0;
        for (int i = 0; i < N; ++i) {
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
    void derivative(array<T,N> *actualVals, array<T,N> *expectedVals, array<T,N> *output) {
        for (int i = 0; i < N; ++i) {
            double diff =  (*expectedVals)[i] - (*actualVals)[i];
            (*output)[i] = diff;
        }
    }
};

#endif //CNET_AFACTIVATIONFUNCTION_H
