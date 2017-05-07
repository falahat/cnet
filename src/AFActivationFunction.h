//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_AFACTIVATIONFUNCTION_H
#define CNET_AFACTIVATIONFUNCTION_H

using namespace std;
#include <math.h>
#include <array>

//TODO: THis is annoyingly verbose.

template <typename T, size_t N, size_t M>
class AFActivationFunction {
public:
    void evaluate(array<T, N> *input, array<T, N> *output) {
    }

    void derivative(array<T, M> *input, array<T, M> *output) {
    }
};

template <typename T, size_t N, size_t M>
class ReLU : AFActivationFunction<T,N,M> {
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

    void derivative(array<T, M> *input, array<T, M> *output) {
        for (int i = 0; i < M; ++i) {
            if ((*input)[i] > 0) {
                (*output)[i] = 1;
            } else {
                (*output)[i] = 0;
            }
        }
    }
};


template <typename T, size_t N, size_t M>
class IdentityFunction: AFActivationFunction<T,N,M> {
public:
    void evaluate(array<T,N> *input, array<T,N> *output) {
        // TODO: How are some ways to copy two arrays?
        for (int i = 0; i < N; ++i) {
            (*output)[i] = (*input)[i];
        }
    }

    void derivative(array<T,M> *input, array<T,M> *output) {
        for (int i = 0; i < M; ++i) {
            (*output)[i] = 1;
        }
    }
};

#endif //CNET_AFACTIVATIONFUNCTION_H
