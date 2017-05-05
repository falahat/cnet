//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_AFMATRIX_H
#define CNET_AFMATRIX_H

#include <vector>
using namespace std;


#define USE_NATIVE_MATRIX

template <class T>
class AFMatrix {
public:
    int numRows;
    int numCols;
    /**
     * This is the values of the matrix stored in one long vector regardless of the matrix's actual shape.
     * The values are stored in row-by-row
     *
     * | a  b |
     * | c  d | = [a b c d e f]
     * | e  f |
     *
     */
    T* vals;

    AFMatrix(int numRows, int numCols) {
        this->numRows = numRows;
        this->numCols = numCols;
        this->vals = new T[numRows * numCols];
    }

    ~AFMatrix() {
        delete [] this->vals;
    }

    AFMatrix* innerProduct(const AFMatrix &other) {
        return 0;
    }

    AFMatrix* innerProduct(const vector<T> &other) {
        return 0;
    }

};


#endif //CNET_AFMATRIX_H
