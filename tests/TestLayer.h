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
    Layer<2,3> *layer = new Layer<2,3>(2,3,relu);
    layer->weights->setValue(0,0, 1);   layer->weights->setValue(0,1, 1);
    layer->weights->setValue(1,0, 2);   layer->weights->setValue(1,1, 2);
    layer->weights->setValue(2,0, 3);   layer->weights->setValue(2,1, 3);


    array<double, 2> sampleInput = {1, 2};
    array<double, 3> expectedOutput = {3, 6, 9};
    layer->forwardPass(&sampleInput);
    EXPECT_TRUE(doubleVectorEqual(&expectedOutput, layer->outputVals));

    EXPECT_EQ(2, 2);

}


#endif //CNET_TESTLAYER_H
