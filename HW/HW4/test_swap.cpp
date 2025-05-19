// test_swap_print.cpp
#include <cassert>
#include <fstream>
#include <vector>
#include <iostream>
#include "file_swaps.hpp"

// Read back an nRows×nCols column‐major matrix from disk
std::vector<double> readMatrix(const std::string &fname, int nRows, int nCols) {
    std::ifstream in(fname, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open for read");
    std::vector<double> M((size_t)nRows * nCols);
    in.read(reinterpret_cast<char*>(M.data()), M.size() * sizeof(double));
    return M;
}

// Print an nRows×nCols column‐major matrix in human‐readable form
void printMatrix(const std::vector<double>& M, int nRows, int nCols) {
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            // element (i,j) is at linear index j*nRows + i
            std::cout << M[j*nRows + i] << "\t";
        }
        std::cout << "\n";
    }
}

int main(){
    const int nRows = 3, nCols = 4;
    const std::string fname = "test_matrix.bin";

    // 1) Build a small test matrix M where M(i,j)=10*i + j
    std::vector<double> M((size_t)nRows * nCols);
    for(int j = 0; j < nCols; ++j)
      for(int i = 0; i < nRows; ++i)
        M[j*nRows + i] = 10*i + j;

    // 2) Write M to file in column-major form
    {
      std::ofstream out(fname, std::ios::binary|std::ios::trunc);
      out.write(reinterpret_cast<const char*>(M.data()),
                M.size()*sizeof(double));
    }

    // 3) Read & print original
    auto M0 = readMatrix(fname, nRows, nCols);
    std::cout << "Original matrix:\n";
    printMatrix(M0, nRows, nCols);
    std::cout << "-------------------\n";

    // 4) Swap rows 0 and 2
    // {
    //   std::fstream fs(fname, std::ios::in|std::ios::out|std::ios::binary);
    //   swapRowsInFile(fs, nRows, nCols, /*i=*/0, /*j=*/2);
    // }
    // auto M1 = readMatrix(fname, nRows, nCols);
    // std::cout << "After swapRowsInFile(0,2):\n";
    // printMatrix(M1, nRows, nCols);
    // std::cout << "-------------------\n";

    // 5) Swap columns 1 and 3
    {
      std::fstream fs(fname, std::ios::in|std::ios::out|std::ios::binary);
      swapColsInFile(fs, nRows, nCols, /*i=*/1, /*j=*/3);
    }
    auto M2 = readMatrix(fname, nRows, nCols);
    std::cout << "After swapColsInFile(1,3):\n";
    printMatrix(M2, nRows, nCols);
    std::cout << "-------------------\n";

    // Cleanup
    std::remove(fname.c_str());
    return 0;
}
