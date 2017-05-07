//
// Created by Aryan on 5/6/2017.
//

#ifndef CNET_TESTACTIVATIONFUNCTION_H
#define CNET_TESTACTIVATIONFUNCTION_H


#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../src/AFFunctions.h"
#include "../src/AFMatrix.h"

TEST (TestActivationFunction, ReLU) {
    /**
     * Create matrix and make sure it's the right dimensions
     * Try to set/get some values
     */
    ReLU<double,5, 5> *reluFn = new ReLU<double,5, 5>();

    array<double, 5> sampleInput = {0.0, 3.0, -1.0, -5.0, 2.0};
    array<double, 5> expectedValue = {0.0, 3.0, 0.0, 0.0, 2.0};
    array<double, 5> expectedDerivative = {0.0, 1.0, 0.0, 0.0, 1.0};

    array<double, 5> actualValue;
    array<double, 5> actualDerivative;
    reluFn->evaluate(&sampleInput, &actualValue);
    reluFn->derivative(&sampleInput, &actualDerivative);

    EXPECT_TRUE(doubleVectorEqual(&expectedValue, &actualValue));
    EXPECT_TRUE(doubleVectorEqual(&expectedDerivative, &actualDerivative));

    delete(reluFn);
}


TEST (TestActivationFunction, IdentityFunction) {
    /**
     * Create matrix and make sure it's the right dimensions
     * Try to set/get some values
     */
    IdentityFunction<double,5,5> *identityFn = new IdentityFunction<double,5,5>();

    array<double, 5> sampleInput = {0.0, 3.0, -1.0, -5.0, 2.0};
    array<double, 5> expectedValue = {0.0, 3.0, -1.0, -5.0, 2.0};
    array<double, 5> expectedDerivative = {1.0, 1.0, 1.0, 1.0, 1.0};

    array<double, 5> actualValue;
    array<double, 5> actualDerivative;
    identityFn->evaluate(&sampleInput, &actualValue);
    identityFn->derivative(&sampleInput, &actualDerivative);

    EXPECT_TRUE(doubleVectorEqual(&expectedValue, &actualValue));
    EXPECT_TRUE(doubleVectorEqual(&expectedDerivative, &actualDerivative));

    delete(identityFn);
}



TEST (TestLossFunction, SquaredLoss) {
    /**
     * Create matrix and make sure it's the right dimensions
     * Try to set/get some values
     */
    AFSquareLossFunction<double,5,5> *squareLoss = new AFSquareLossFunction<double,5,5>();

    array<double, 5> actualValues = {0.0, 0.0, -1.0, 5.0, 2.0};
    array<double, 5> expectedValues = {0.0, 3.0, 1.0, -5.0, 2.0}; // 0 + 9 + 4 + 100 + 0 = 113
    double expectedValue = 113.0/2;
    array<double, 5> expectedDerivative = {0, 3, 2, -10.0, 0}; // {0, 3, 2, -10, 0}

    double actualValue = squareLoss->evaluate(&actualValues, &expectedValues);
    array<double, 5> actualDerivative;
    squareLoss->derivative(&actualValues, &expectedValues, &actualDerivative);

    EXPECT_DOUBLE_EQ(expectedValue, actualValue);
    EXPECT_TRUE(doubleVectorEqual(&expectedDerivative, &actualDerivative));

    delete(squareLoss);
}

#endif //CNET_TESTACTIVATIONFUNCTION_H
