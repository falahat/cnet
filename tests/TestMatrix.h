//
// Created by Aryan on 5/6/2017.
//

#ifndef CNET_TESTMATRIX_H
#define CNET_TESTMATRIX_H

#include "../src/AFMatrix.h"
#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../lib/googletest/googletest/include/gtest/internal/gtest-internal.h"


TEST (TestMatrix, CreateMatrix) {
    /**
     * Create matrix and make sure it's the right dimensions
     * Try to set/get some values
     */
    AFMatrix<double, 2, 3> *mat = new AFMatrix<double, 2, 3>();
    mat->setValue(0,0, 1.0);
    mat->setValue(1, 2, 2.0);

    EXPECT_EQ(mat->numRows, 2);
    EXPECT_EQ(mat->numCols, 3);
    EXPECT_DOUBLE_EQ(mat->getValue(0,0), 1.0);
    EXPECT_DOUBLE_EQ(mat->getValue(1,2), 2.0);

    delete(mat);
}


TEST (TestMatrix, CreateMatrixFromValues) {
    /**
     * Create matrix and make sure it's the right dimensions
     * Change/Delete the original value and try to access matrix values (should be copied data)
     * Try to get values
     */
    array<double, 2*3> *ogVals = new array<double, 2*3>();
    (*ogVals)[0] = 1.0;
    (*ogVals)[1] = 2.0;
    (*ogVals)[2] = 3.0;
    (*ogVals)[3] = 4.0;
    (*ogVals)[4] = 5.0;
    (*ogVals)[5] = 6.0;
    (*ogVals)[6] = 7.0;
    AFMatrix<double, 2, 3> *mat = new AFMatrix<double, 2, 3>(ogVals);

    // Basic Val Checks
    EXPECT_EQ(mat->numRows, 2);
    EXPECT_EQ(mat->numCols, 3);
    EXPECT_DOUBLE_EQ(mat->getValue(0,0), 1.0);
    EXPECT_DOUBLE_EQ(mat->getValue(1,2), 7.0);

    // Mutate/Delete and check we copied ogValues, not just passed a reference
    (*ogVals)[0] = -1;
    EXPECT_DOUBLE_EQ(mat->getValue(0,0), 1.0);
    delete(ogVals);
    EXPECT_DOUBLE_EQ(mat->getValue(0,0), 1.0);


    delete(mat);
}


TEST (TestMatrix, CreateMatrixFromMatrix) {
    AFMatrix<double, 2, 3> *matOg = new AFMatrix<double, 2, 3>();
    matOg->setValue(0,0, 1.0);
    matOg->setValue(1, 2, 2.0);
    EXPECT_EQ(matOg->numRows, 2);
    EXPECT_EQ(matOg->numCols, 3);
    EXPECT_DOUBLE_EQ(matOg->getValue(0,0), 1.0);
    EXPECT_DOUBLE_EQ(matOg->getValue(1,2), 2.0);
    AFMatrix<double, 2, 3> *matNew = new AFMatrix<double, 2, 3>(matOg);

    EXPECT_EQ(matNew->numRows, 2);
    EXPECT_EQ(matNew->numCols, 3);
    EXPECT_DOUBLE_EQ(matNew->getValue(0,0), 1.0);
    EXPECT_DOUBLE_EQ(matNew->getValue(1,2), 2.0);

    matOg->setValue(0,0, -2.0);
    EXPECT_DOUBLE_EQ(matNew->getValue(0,0), 1.0);
    delete(matOg);
    EXPECT_DOUBLE_EQ(matNew->getValue(1,2), 2.0);

    delete(matNew);

}


TEST (TestMatrix, MatrixEquals) {
    AFMatrix<double, 1, 2> *mat1 = new AFMatrix<double, 1, 2>();
    mat1->setValue(0,0, 1.0);
    mat1->setValue(0,1, 2.0);

    AFMatrix<double, 1, 2> *mat2 = new AFMatrix<double, 1, 2>();
    mat2->setValue(0,0, 1.0);
    mat2->setValue(0,1, 2.0);
    EXPECT_TRUE(mat1->equals(mat2));

    mat2->setValue(0,1, 0.0);
    EXPECT_FALSE(mat1->equals(mat2));
}


TEST (TestMatrix, VectorInnerProduct) {
    array<double, 3> vec1 = {1.0, 1.0, 3.0};
    array<double, 3> vec2 = {1.0, 20.0, 10.0};
    double outVal = vectorInnerProduct(&vec1, &vec2);
    EXPECT_DOUBLE_EQ(outVal, 51.0);
}


TEST (TestMatrix, VectorInnerProductBounded) {
    array<double, 3> vec1 = {1.0, 1.0, 3.0};
    array<double, 3> vec2 = {1.0, 20.0, 10.0};
    double outVal1 = vectorInnerProductBounded(&vec1, &vec2, 1, 2, 1, 2);
    EXPECT_DOUBLE_EQ(outVal1, 20.0);
    double outVal2 = vectorInnerProductBounded(&vec1, &vec2, 1, 3, 1, 3);
    EXPECT_DOUBLE_EQ(outVal2, 50.0);
}


TEST (TestMatrix, MatrixVectorInnerProduct) {
    // TODO: Make sure instantiated row by row
    array<double, 2*3> vals1 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    AFMatrix<double, 2, 3> *mat1 = new AFMatrix<double, 2, 3>(&vals1);

    array<double, 3> vals2 = {2.0,
                              2.0,
                              2.0};

    array<double, 2> expected = {12.0, 30.0};
    array<double, 2> *actual = new array<double, 2>();
    mat1->innerProduct(&vals2, actual);
    EXPECT_TRUE(doubleVectorEqual(&expected, actual));
}

TEST (TestMatrix, MatrixInnerProduct) {
    array<double, 2*3> vals1 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    AFMatrix<double, 2, 3> *mat1 = new AFMatrix<double, 2, 3>(&vals1);

    array<double, 3*3> vals2 = {1.0, 2.0, 3.0,
                                1.0, 2.0, 3.0,
                                1.0, 2.0, 3.0};
    AFMatrix<double, 3, 3> *mat2 = new AFMatrix<double, 3, 3>(&vals2);

    array<double, 3*2> expectedVals = {6.0, 12.0, 18.0,
                                       15.0, 30.0, 45.0};


    AFMatrix<double, 2, 3> *expected = new AFMatrix<double, 2, 3>(&expectedVals);
    AFMatrix<double, 2, 3> *actual = new AFMatrix<double, 2, 3>();
    mat1->innerProduct(mat2, actual);
    EXPECT_TRUE(expected->equals(actual));
}


TEST (TestMatrix, MatrixAdd) {
    EXPECT_EQ(0, 0);
}



TEST (TestMatrix, MatrixSubtract) {
    EXPECT_EQ(0, 0);
}


TEST (TestMatrix, MatrixScale) {
    EXPECT_EQ(0, 0);
}


#endif //CNET_TESTMATRIX_H
