//
// Created by Aryan on 5/7/2017.
//

#ifndef CNET_TESTLAYER_H
#define CNET_TESTLAYER_H

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../src/AFMatrix.h"
#include "../src/Layer.h"

TEST (TestLayer, CreateLayer) {
    /**
     * Creates a layer and checks its dimensions. It sets its weights to known values.
     * Creates a sample input and calls forward pass. It confirms the correct output values
     * Creates a sample expected output and uses that to calculate loss. Also uses backpropogation to calculate deltas.
     */
    AFActivationFunction<double,3> *relu = (new ReLU<double,3>());
    AFLossFunction<double,3> *squaredLoss = new AFSquareLossFunction<double,3>();

    Layer<2,3> *layer = new Layer<2,3>(2,3,relu);
    layer->weights->setValue(0,0, 1);   layer->weights->setValue(0,1, 1);
    layer->weights->setValue(1,0, 2);   layer->weights->setValue(1,1, 2);
    layer->weights->setValue(2,0, 3);   layer->weights->setValue(2,1, 3);

    EXPECT_EQ(layer->lenIn, 2);
    EXPECT_EQ(layer->lenOut, 3);
    EXPECT_EQ(layer->weights->numCols, 2);
    EXPECT_EQ(layer->weights->numRows, 3);

}



TEST (TestLayer, ForwardPass) {
    /**
     * Creates a layer and checks its dimensions. It sets its weights to known values.
     * Creates a sample input and calls forward pass. It confirms the correct output values
     * Creates a sample expected output and uses that to calculate loss. Also uses backpropogation to calculate deltas.
     */
    AFActivationFunction<double,3> *relu = (new ReLU<double,3>());
    AFLossFunction<double,3> *squaredLoss = new AFSquareLossFunction<double,3>();

    Layer<2,3> *layer = new Layer<2,3>(2,3,relu);
    layer->weights->setValue(0,0, 1);   layer->weights->setValue(0,1, 1);
    layer->weights->setValue(1,0, 2);   layer->weights->setValue(1,1, 2);
    layer->weights->setValue(2,0, 3);   layer->weights->setValue(2,1, 3);


    array<double, 2> sampleInput = {1, 2};
    array<double, 3> expectedOutput = {3, 6, 9};
    layer->forwardPass(&sampleInput);
    EXPECT_TRUE(doubleVectorEqual(&expectedOutput, layer->outputVals));

}



TEST (TestLayer, BackPropogateBase) {
    /**
     * Creates a layer and checks its dimensions. It sets its weights to known values.
     * Creates a sample input and calls forward pass. It confirms the correct output values
     * Creates a sample expected output and uses that to calculate loss. Also uses backpropogation to calculate deltas.
     */
    AFActivationFunction<double,3> *relu = (new ReLU<double,3>());
    AFLossFunction<double,3> *squaredLoss = new AFSquareLossFunction<double,3>();

    Layer<2,3> *layer = new Layer<2,3>(2,3,relu);
    layer->weights->setValue(0,0, 1);   layer->weights->setValue(0,1, 1);
    layer->weights->setValue(1,0, 2);   layer->weights->setValue(1,1, 2);
    layer->weights->setValue(2,0, 3);   layer->weights->setValue(2,1, 3);


    array<double, 2> sampleInput = {1, 2};
    layer->forwardPass(&sampleInput);

    array<double,3> sampleExpectedOutput = {1, 1, 1};
    array<double,3> expectedDeltas = {-2, -5, -8};
    array<double,6> expectedGradVals = {2, 4,
                                        5, 10,
                                        8, 16};
    AFMatrix<double,3,2> *expectedGrad = new AFMatrix<double,3,2>(&expectedGradVals);
    array<double,6> expectedWeightsVals = {0.8, 0.6,
                                           1.5, 1.0,
                                           2.2, 1.4};
    AFMatrix<double,3,2> *expectedWeights = new AFMatrix<double,3,2>(&expectedWeightsVals);

    layer->backpropagateBase(layer->outputVals, &sampleExpectedOutput, squaredLoss);
    layer->updateWeights(0.1);

    EXPECT_TRUE(doubleVectorEqual(&expectedDeltas, layer->deltas));
    EXPECT_TRUE(layer->weightGradient->equals(expectedGrad));
    EXPECT_TRUE(layer->weights->equals(expectedWeights));

}


TEST (TestLayer, BackPropogate) {

}


#endif //CNET_TESTLAYER_H
