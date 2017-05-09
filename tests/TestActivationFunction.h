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
    ReLU<double> *reluFn = new ReLU<double>();

    vector<double> sampleInput = {0.0, 3.0, -1.0, -5.0, 2.0};
    vector<double> expectedValue = {0.0, 3.0, 0.0, 0.0, 2.0};
    vector<double> expectedDerivative = {0.0, 1.0, 0.0, 0.0, 1.0};

    vector<double> actualValue;
    vector<double> actualDerivative;
    actualDerivative.reserve(expectedDerivative.size());
    actualDerivative.assign(expectedDerivative.size(), 0);
    actualValue.reserve(expectedValue.size());
    actualValue.assign(expectedValue.size(), 0);

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
    IdentityFunction<double> *identityFn = new IdentityFunction<double>();

    vector<double> sampleInput = {0.0, 3.0, -1.0, -5.0, 2.0};
    vector<double> expectedValue = {0.0, 3.0, -1.0, -5.0, 2.0};
    vector<double> expectedDerivative = {1.0, 1.0, 1.0, 1.0, 1.0};

    vector<double> actualValue;
    vector<double> actualDerivative;
    actualDerivative.reserve(expectedDerivative.size());
    actualDerivative.assign(expectedDerivative.size(), 0);
    actualValue.reserve(expectedValue.size());
    actualValue.assign(expectedValue.size(), 0);


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
    AFSquareLossFunction<double> *squareLoss = new AFSquareLossFunction<double>();

    vector<double> actualValues = {0.0, 0.0, -1.0, 5.0, 2.0};
    vector<double> expectedValues = {0.0, 3.0, 1.0, -5.0, 2.0}; // 0 + 9 + 4 + 100 + 0 = 113
    double expectedValue = 113.0/2;
    vector<double> expectedDerivative = {0, -3, -2, 10.0, 0}; // {0, 3, 2, -10, 0}

    double actualValue = squareLoss->evaluate(&actualValues, &expectedValues);
    vector<double> actualDerivative;
    actualDerivative.reserve(expectedDerivative.size());
    actualDerivative.assign(expectedDerivative.size(), 0);

    squareLoss->derivative(&actualValues, &expectedValues, &actualDerivative);

    EXPECT_DOUBLE_EQ(expectedValue, actualValue);
    EXPECT_TRUE(doubleVectorEqual(&expectedDerivative, &actualDerivative));

    delete(squareLoss);
}

#endif //CNET_TESTACTIVATIONFUNCTION_H
