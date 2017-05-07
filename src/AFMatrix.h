//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_AFMATRIX_H
#define CNET_AFMATRIX_H

#define ACCEPTABLE_DOUBLE_DIFF 0.000000001
#include <array>
using namespace std;

template <class T, size_t ROWS, size_t COLS>
class AFMatrix {
public:
    int numRows;
    int numCols;
    /**
     * This is the values of the matrix stored in one long array regardless of the matrix's actual shape.
     * The values are stored in row-by-row
     *
     * @todo Is vals dynamically allocated or what??
     * | a  b |\n
     * | c  d | = [a b c d e f]\n
     * | e  f |\n
     *
     */
    array<T, ROWS*COLS> *vals;


    AFMatrix() {
        this->numRows = ROWS;
        this->numCols = COLS;
        this->vals = new array<T, ROWS*COLS>();
    }


    /**
     * Creates a new copy and copies data.
     * @todo Make this effecient and non-copying
     * @param copyFrom
     */
    AFMatrix(AFMatrix<T, ROWS, COLS> *copyFrom) {
        this->numRows = ROWS;
        this->numCols = COLS;
        this->vals = new array<T, ROWS*COLS>();
        this->copyValues(this, copyFrom); // Copies the values from `copyFrom` to itself
    }

    /**
     * Creates a new copy and copies data from `copyFrom`.
     * @todo Make this effecient and non-copying
     * @param copyFrom
     */
    AFMatrix(array<T, ROWS*COLS> *copyFromArray) {
        this->numRows = ROWS;
        this->numCols = COLS;
        this->vals = new array<T, ROWS*COLS>();
        this->copyValues(this->vals, copyFromArray); // Copies the values from `copyFrom` to itself
    }

    ~AFMatrix() {
        delete this->vals;
    }

    /**
     * @param row
     * @param col
     * @return the index `i` such that `this->vals[i] = (row, col)`.
     */
    inline int getIndex(int row, int col) {
        return row * this->numCols + col;
    }

    /**
     * @param row
     * @param col
     * @return the index `i` such that `this->vals[i] = (row, col)`.
     */
    inline T getValue(int row, int col) {
        return (*this->vals)[this->getIndex(row, col)];
    }

    /**
     * @param row
     * @param col
     * @param newValue The new value to put in this row/col
     */
    void setValue(int row, int col, T newValue) {
        (*this->vals)[this->getIndex(row, col)] = newValue;
    }

    /**
     *
     * @param other The other matrix to multiply this against
     * @param out The matrix to write the output values
     */
    template <size_t OTHER_COLS>
    void innerProduct(AFMatrix<T, COLS, OTHER_COLS> *other, AFMatrix<T, ROWS, OTHER_COLS> *out) {

    }

    /**
     * Multiplies a matrix on the left against a array on the right. The array on the right is treated as a column vector.
     * @pre this.numCols = other.size()
     * @param other The other array to inner product with.
     * @param out Output matrix to write values to. It has this.numRows rows and 1 column.
     */
    void innerProduct(array<T, COLS> *other,  AFMatrix<T, ROWS, 1> *out) {
    }

    /**
     * Multiplies a matrix on the left against a vector on the right.
     * @pre this.numRows = other.size()
     * @param other The other vector to inner product with.
     * @param out Output matrix to write values to. It has this.numRows rows and 1 column.
     */
    void innerProduct(array<T, COLS> *other,  array<T, ROWS> *out) {
    }

    /**
     * Transposes this matrix and writes result to out
     * @param out - Matrix that has this.numCols rows and this.numRows cols. The result will be written to out.
     */
    void transpose(AFMatrix<T, COLS, ROWS> *out) {

    }

    /**
     *
     * @return A new, dynamically allocated matrix that is the transpose of this
     * @warning Remember to delete the returned Matrix when done
     */
    AFMatrix* transpose() {

    }

    /**
     * Multiplies all entries of a matrix by `factor`
     * @param factor
     * @param out
     */
    void scale(double factor, AFMatrix* out) {
        for (int col = 0; col < COLS; ++col) {
            for (int row = 0; row < ROWS; ++row) {
                out->setValue(row, col, factor * this->getValue(row, col));
            }
        }
    }

    /**
     * Adds two matrices and writes result into `out`
     * @param other - The matrix to add to `this`.
     * @param out - The matrix to write the result to
     * @warning requires
     */
    void add(AFMatrix<T, ROWS, COLS> *other, AFMatrix<T, ROWS, COLS> *out) {
        for (int col = 0; col < COLS; ++col) {
            for (int row = 0; row < ROWS; ++row) {
                T val = this->getValue(row, col) + other->getValue(row, col);
                out->setValue(row, col, val);
            }
        }
    }

    /**
     * Subtracts two matrices and writes result into `out`
     * @param other - The matrix to subtract from `this`.
     * @param out - The matrix to write the result to
     */
    void subtract(AFMatrix<T, ROWS, COLS> *other, AFMatrix<T, ROWS, COLS> *out) {
        for (int col = 0; col < COLS; ++col) {
            for (int row = 0; row < ROWS; ++row) {
                T val = this->getValue(row, col) - other->getValue(row, col);
                out->setValue(row, col, val);
            }
        }
    }

    /**
     * Dynamically makes a new vector that is `this.numRows * this.numCols` elements long and copies `this.vals`
     * into it.
     * @return The new dynamically allocated vector (we call `reserve()` on it though).
     */
    array<T, ROWS*COLS> *toArray() {
        array<T, ROWS*COLS> *ans = new array<T, ROWS*COLS>();
        ans = this->vals;
        return ans;
    }

    /**
     * Copies values from `src` to `dst`. The two matrices will be exactly identitcal.
     * @param dst
     * @param src
     */
    void copyValues(AFMatrix<T, ROWS, COLS> *dst, AFMatrix<T, ROWS, COLS> *src) {
        copyValues(dst->vals, src->vals);
    }

    /**
     * Copies data from the `src` array to `dst->vals`.
     * @todo Clarify how things work if matrices can be row-major or column-major.
     * @param dst
     * @param src
     */
    void copyValues(AFMatrix<T, ROWS, COLS> *dst, array<T, ROWS*COLS> *src) {
        copyValues(this->vals, src);
    }

    /**
     * Copies values from `src` to `dst`. The arrays will be identical afterwards.
     * @tparam N - The size of the arrays, must be equal
     * @param dst
     * @param src
     */
    template <typename T1, size_t N>
    void copyValues(array<T1, N> *dst, array<T1, N> *src) {
        for (int i = 0; i < N; ++i) {
            (*dst)[i] = (*src)[i];
        }
    }

    bool equals(AFMatrix<T, ROWS, COLS> *otherMat) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                // TODO: This assumes doubles
                if (this->getValue(i, j) - otherMat->getValue(i, j) >= ACCEPTABLE_DOUBLE_DIFF) {
                    return false;
                }
            }
        }
        return true;
    }
};



template <typename T, size_t N>
/**
 * @tparam T The type of data in the vectors being multiplies. Probably a `double`.
 * @param vec1 - Left vector
 * @param vec2 - Right vector
 * @return The dot product (inner product) of two vectors.
 * @pre vec1 and vec2 have the same length.
 */
T vectorInnerProductBounded(array<T, N> *vec1, array<T, N> *vec2, size_t start1, size_t start2, size_t len) {
    // TODO: Add bound checking?
    T ans;
    for (int offset = 0; offset < len ; ++offset) {
        ans += (*vec1)[start1 + offset] * (*vec2)[start2 + offset];
    }
    return ans;
}

// TODO: integer matrices multiplied by double matrices though?
template <typename T, size_t N>
/**
 * @tparam T The type of data in the vectors being multiplies. Probably a `double`.
 * @param vec1 - Left vector
 * @param vec2 - Right vector
 * @return The dot product (inner product) of two vectors.
 * @pre vec1 and vec2 have the same length.
 */
T vectorInnerProduct(array<T, N> *vec1, array<T, N> *vec2) {
    return vectorInnerProductBounded(vec1, vec2, 0ul, 0ul, N);
}


template <size_t N>
bool doubleVectorEqual(array<double, N> *vec1, array<double, N> *vec2) {
    for (int i = 0; i < N; ++i) {
        if ((*vec1)[i] - (*vec2)[i] >= ACCEPTABLE_DOUBLE_DIFF) {
            return false;
        }
    }
    return true;
}
#endif //CNET_AFMATRIX_H
