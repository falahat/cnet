//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_AFMATRIX_H
#define CNET_AFMATRIX_H

#define ACCEPTABLE_DOUBLE_DIFF 0.000000001
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;



// TODO: All 
template <typename T>
/**
 * @tparam T The type of data in the vectors being multiplies. Probably a `double`.
 * @param vec1 - Left vector
 * @param vec2 - Right vector
 * @return The dot product (inner product) of two vectors.
 * @pre vec1 and vec2 have the same length.
 */
T vectorInnerProductBounded(vector<T> *vec1, vector<T> *vec2, size_t start1, size_t start2, size_t len) {
    // TODO: Add bound checking?
    T ans = 0;
    for (int offset = 0; offset < len ; ++offset) {
        ans += (*vec1)[start1 + offset] * (*vec2)[start2 + offset];
    }
    return ans;
}

// TODO: integer matrices multiplied by double matrices?
template <typename T>
/**
 * @tparam T The type of data in the vectors being multiplies. Probably a `double`.
 * @param vec1 - Left vector
 * @param vec2 - Right vector
 * @return The dot product (inner product) of two vectors.
 * @pre vec1 and vec2 have the same length.
 */
T vectorInnerProduct(vector<T> *vec1, vector<T> *vec2) {
    return vectorInnerProductBounded(vec1, vec2, 0ul, 0ul, vec1->size());
}

// TODO: Improve method name: "doubleVectorEqual"
bool doubleVectorEqual(vector<double> *vec1, vector<double> *vec2) {
    for (int i = 0; i < vec1->size(); ++i) {
        if ((*vec1)[i] - (*vec2)[i] >= ACCEPTABLE_DOUBLE_DIFF) {
            return false;
        }
    }
    return true;
}


template <class T>
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
    vector<T> *vals;


    AFMatrix(int numRows, int numCols) {
		this->numRows = numRows;
        this->numCols = numCols;
        this->vals = new vector<T>();
        this->vals->reservegetSize());
        this->vals->assign(getSize(), 0);
    }

    /**
     * Creates a new copy and copies data.
     * @todo Make this effecient and non-copying
     * @param copyFrom
     */
    AFMatrix(AFMatrix<T> *copyFrom) {
        this->numRows = copyFrom->numRows;
        this->numCols = copyFrom->numCols;
        this->vals = new vector<T>();
        this->vals->reserve(getSize());
        this->vals->assign(getSize());
        this->copyValues(this, copyFrom); // Copies the values from `copyFrom` to itself
    }

    /**
     * Creates a new copy and copies data from `copyFrom`.
     * @todo Make this effecient and non-copying
     * @param copyFrom
     */
    AFMatrix(int numRows, int numCols, vector<T> *copyFromArray) {
        this->numRows = numRows;
        this->numCols = numCols;
        this->vals = new vector<T>();
        this->vals->reserve(getSize());
        this->vals->assign(getSize(), 0);
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
		// TODO: Assumes row major? Allow flexibility?
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

	inline size_t getSize() {
		return (this->numCols * this->numRows);
	}

    /**
     *
     * @param col
     * @return An `std:array` filled with the column values of this matrix
     * @warning Delete the new array to free memory
     */
    vector<T> *getCol(int col) {
        vector<T> *ans = new vector<T>();
        ans->reserve(this->numRows);
        ans->assign(this->numCols,0);
        for (int row = 0; row < this->numRows; ++row) {
            (*ans)[row] = this->getValue(row, col);
        }
        return ans;
    };

    /**
     *
     * @param row
     * @return An `std:array` filled with the row values of this matrix
     * @warning Delete the new array to free memory
     */
    vector<T> *getRow(int row) {
		// TODO: Improve this for performance. Perhaps return a view instead? 
        vector<T> *ans = new vector<T>();
        ans->reserve(this->numCols);
        ans->assign(this->numCols,0); // TODO: Why assign?
        for (int col = 0; col < this->numCols; ++col) {
            (*ans)[col] = this->getValue(row, col);
        }
        return ans;
    };

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
    void innerProduct(AFMatrix<T> *other, AFMatrix<T> *out) {
        this->innerProduct(other, out, 0, 0);
    }

    /**
     *
     * @param other The other matrix to multiply this against
     * @param out The matrix to write the output values
     */
    void innerProduct(AFMatrix<T> *other, AFMatrix<T> *out, size_t outStartRow, size_t outStartCol) {
        for (size_t outCol = outStartCol; outCol < other->numCols; ++outCol) {
            vector<T> *currRightCol = other->getCol(outCol);
            this->innerProduct(currRightCol, out, outCol);
        }
    }

    /**
     * Multiplies a matrix on the left against a array on the right. The array on the right is treated as a column vector.
     * @pre this.numCols = other.size()
     * @param other The other array to inner product with.
     * @param out Output matrix to write values to. It has this.numRows rows and 1 column.
     */
    void innerProduct(vector<T> *other,  AFMatrix<T> *out, size_t outCol) {
        for (int outRow = 0; outRow < this->numRows; ++outRow) {
            // TODO: Assumes that `this->vals` is row-wise
            vector<T> *currLeftRow = this->getRow(outRow);
            T currVal = vectorInnerProduct(currLeftRow, other);
            out->setValue(outRow, outCol, currVal);
            // FIXME: DRY!!!
        }
    }

    /**
     * Multiplies a matrix on the left against a vector on the right.
     * @pre this.numRows = other.size()
     * @param other The other vector to inner product with.
     * @param out Output matrix to write values to. It has this.numRows rows and 1 column.
     */
    void innerProduct(vector<T> *other,  vector<T> *out) {
        for (int outRow = 0; outRow < this->numRows; ++outRow) {
            // TODO: Assumes that `this->vals` is row-wise
            vector<T> *currLeftRow = this->getRow(outRow);
            T currVal = vectorInnerProduct(currLeftRow, other);
            (*out)[outRow] =  currVal;
        }
    }

    /**
     * Transposes this matrix and writes result to out
     * @param out - Matrix that has this.numCols rows and this.numRows cols. The result will be written to out.
     */
    void transpose(AFMatrix<T> *out) {
        // TODO: Implement transpose
    }

    /**
     *
     * @return A new, dynamically allocated matrix that is the transpose of this
     * @warning Remember to delete the returned Matrix when done
     */
    AFMatrix* transpose() {
        // TODO: Implement transpose
    }

    /**
     * Multiplies all entries of a matrix by `factor`
     * @param factor
     * @param out
     */
    void scale(double factor, AFMatrix* out) {
		// TODO: Slowest version but we will improve it
        for (int col = 0; col < this->numCols; ++col) {
            for (int row = 0; row < this->numRows; ++row) {
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
    void add(AFMatrix<T> *other, AFMatrix<T> *out) {
        for (int col = 0; col < this->numCols; ++col) {
            for (int row = 0; row < this->numRows; ++row) {
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
    void subtract(AFMatrix<T> *other, AFMatrix<T> *out) {
        for (int col = 0; col < this->numCols; ++col) {
            for (int row = 0; row < this->numRows; ++row) {
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
    vector<T> *toArray() {
        vector<T> *ans = new vector<T>();
        // TODO: Does this copy or overwrite the pointer
        ans = this->vals;
        return ans;
    }

    /**
     * Copies values from `src` to `dst`. The two matrices will be exactly identitcal.
     * @param dst
     * @param src
     */
    void copyValues(AFMatrix<T> *dst, AFMatrix<T> *src) {
        copyValues(dst, src, 0, 0);
    }

    /**
     * Copies values from `src` to `dst`. The two matrices will be exactly identitcal.
     * @param dst
     * @param src
     */
    void copyValues(AFMatrix<T> *dst, AFMatrix<T> *src, size_t srcRowStart, size_t srcColStart) {
        // TODO: Check that srcRowStart
        for (int dstRow = 0; dstRow < dst->numRows; ++dstRow) {
            int srcRow = dstRow + srcRowStart;
            for (int dstCol = 0; dstCol < dst->numCols; ++dstCol) {
                int srcCol = dstCol + srcColStart;
                dst->setValue(dstRow, dstCol, src->getValue(srcRow, srcCol));
            }
        }
    }

    /**
     * Copies data from the `src` array to `dst->vals`.
     * @todo Clarify how things work if matrices can be row-major or column-major.
     * @param dst
     * @param src
     */
    void copyValues(AFMatrix<T> *dst, vector<T> *src) {
        copyValues(this->vals, src);
    }

    /**
     * Copies values from `src` to `dst`. The arrays will be identical afterwards.
     * @tparam N - The size of the arrays, must be equal
     * @param dst
     * @param src
     */
    void copyValues(vector<T> *dst, vector<T> *src) {
        for (int i = 0; i < dst->size(); ++i) {
            (*dst)[i] = (*src)[i];
        }
    }

    bool equals(AFMatrix<T> *otherMat) {
        // TODO: Check dimensions!

        for (int i = 0; i < this->numRows; ++i) {
            for (int j = 0; j < this->numCols; ++j) {
                // TODO: This assumes doubles
                if (this->getValue(i, j) - otherMat->getValue(i, j) >= ACCEPTABLE_DOUBLE_DIFF) {
                    return false;
                }
            }
        }
        return true;
    }


    std::ostringstream toString() {
        std::ostringstream oss;
        oss << "Matrix: " << this->numRows << " Rows, " << this->numCols << " Cols." << endl;
        for (int row = 0; row < this->numRows; ++row) {
            oss << "| ";
            for (int col = 0; col < this->numCols; ++col) {
                oss << this->getValue(row, col);
                if (col != this->numCols - 1) {
                    oss << ", ";
                }
            }
            oss << "| " << endl;
        }
        return oss;
    }
};

template <typename T>
std::ostringstream vectorToString(vector<T> vec) {
    std::ostringstream oss;
    oss << "[";
    if (!vec.empty())
    {
    // Convert all but the last element to avoid a trailing ","
        std::copy(vec.begin(), vec.end()-1,
                  std::ostream_iterator<int>(oss, ","));

    // Now add the last element with no delimiter
        oss << vec.back();
    }
    oss << "] ";
    return oss;
}

#endif //CNET_AFMATRIX_H
