#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <algorithm>

void swapRows(std::vector<double> &matrix,int nRows, int nCols, int i, int j) {
    for (int c = 0; c < nCols; ++c) {
        std::swap(matrix[c*nRows + i], matrix[c*nRows + j]);
    }
}

void swapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {
    int off_i = i * nCols;
    int off_j = j * nCols;
    for (int r = 0; r < nRows; ++r) {
        std::swap(matrix[off_i + r], matrix[off_j + r]);
    }
}

void manualSwapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {
    for (int c = 0; c < nCols; ++c) {
        double temp = matrix[c*nRows + i];
        matrix[c*nRows + i] = matrix[c*nRows + j];
        matrix[c*nRows + j] = temp;
    }
}

void manualSwapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {
    int off_i = i * nCols;
    int off_j = j * nCols;
    for (int r = 0; r < nRows; ++r) {
        double temp = matrix[off_i + r];
        matrix[off_i + r] = matrix[off_j + r];
        matrix[off_j + r] = temp;
    }
}