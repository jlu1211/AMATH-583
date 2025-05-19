// AMATH 483-583 row major Matrix class template starter
// write the methods for:
// transpose
// infinityNorm
// operator*
// operator+
#ifndef MATRIX_CLASS_HPP
#define MATRIX_CLASS_HPP
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

template <typename T>
class Matrix
{
public:
    Matrix(int numRows, int numCols)
        : num_rows(numRows), num_cols(numCols), data(numRows * numCols) {}

    T &operator()(int i, int j)
    {
        return data[i * num_cols + j];
    }

    const T &operator()(int i, int j) const
    {
        return data[i * num_cols + j];
    }

    Matrix<T> operator*(const Matrix<T> &other) const
    {
        // Check for dimension mismatch
        if (num_cols != other.num_rows) {
            throw std::invalid_argument("Matrix multiplication dimension mismatch: first matrix columns (" + 
                std::to_string(num_cols) + ") must match second matrix rows (" + 
                std::to_string(other.num_rows) + ")");
        }
        Matrix<T> result(num_rows, other.num_cols);
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < other.num_cols; ++j) {
                T sum = T();
                for (int k = 0; k < num_cols; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    Matrix<T> operator+(const Matrix<T> &other) const;

    Matrix<T> transpose() const
    {
        Matrix<T> result(num_cols, num_rows);
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    int numRows() const
    {
        return num_rows;
    }

    int numCols() const
    {
        return num_cols;
    }

    T infinityNorm() const
    {
        T norm = 0;
        for (int i = 0; i < num_rows; ++i) {
            T rowSum = T();
            for (int j = 0; j < num_cols; ++j) {
                rowSum += std::abs((*this)(i, j));
            }
            if (rowSum > norm) {
                norm = rowSum;
            }
        }
        return norm;
    }

private:
    int num_rows;
    int num_cols;
    std::vector<T> data;
};

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const
{
    // check for errors in Matrix dimensions
    if (num_rows != other.num_rows || num_cols != other.num_cols) {
        throw std::invalid_argument("Number of rows or number of cols unmatched");
    }
    Matrix<T> result(num_rows, num_cols);
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            result(i, j) = (*this)(i, j) + (other)(i, j);
        }
    }
    return result;
}
#endif 