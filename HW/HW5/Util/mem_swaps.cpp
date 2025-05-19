#include "mem_swaps.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <chrono>

static std::pair<int, int> getRandomIndices(int nRows, int nCols) {
    int i = std::rand() % nRows;
    int j = std::rand() % (nRows - 1);
    if (j >= i) {
        ++j;
    }
    return std::make_pair(i, j);
}

int main() {
    std::srand(std::time(0));
    const int ntrials = 5;
    std::cout << "n,swapRows,swapCols,manualSwapRows,manualSwapCols" << std::endl;
    for (int N = 16; N <= 4096; N *= 2) {
        std::vector<double> matrix(N*N);
        for (auto &x : matrix) {
            x = std::rand();
        }
        // Swap rows
        auto [ri, rj] = getRandomIndices(N, N);
        auto start = std::chrono::high_resolution_clock::now();
        for (int t = 0; t < ntrials; ++t) {
            swapRows(matrix, N, N, ri, rj);
        }
        auto end = std::chrono::high_resolution_clock::now();
        double time_taken = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

        // Swap columns
        auto [ci, cj] = getRandomIndices(N, N);
        auto start2 = std::chrono::high_resolution_clock::now();
        for (int t = 0; t < ntrials; ++t) {
            swapCols(matrix, N, N, ci, cj);
        }
        auto end2 = std::chrono::high_resolution_clock::now();
        double time_taken2 = std::chrono::duration_cast<std::chrono::duration<double>>(end2 - start2).count();

        // Swap rows manually
        auto start3 = std::chrono::high_resolution_clock::now();
        for (int t = 0; t < ntrials; ++t) {
            manualSwapRows(matrix, N, N, ri, rj);
        }
        auto end3 = std::chrono::high_resolution_clock::now();
        double time_taken3 = std::chrono::duration_cast<std::chrono::duration<double>>(end3 - start3).count();

        // Swap columns manually
        auto start4 = std::chrono::high_resolution_clock::now();
        for (int t = 0; t < ntrials; ++t) {
            manualSwapCols(matrix, N, N, ci, cj);
        }
        auto end4 = std::chrono::high_resolution_clock::now();
        double time_taken4 = std::chrono::duration_cast<std::chrono::duration<double>>(end4 - start4).count();

        std::cout << N << "," << time_taken << "," << time_taken2 << "," << time_taken3 << "," << time_taken4 << std::endl;
    }
    return 0;
}
