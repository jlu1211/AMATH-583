#include "file_swaps.hpp"
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <cstdio>

int main() {
    std::vector<int> sizes = {16,32,64,128,256,512,1024,2048,4096,8192};
    const int trials = 3;
    std::mt19937_64 rng(0);
    std::uniform_real_distribution<double> dist(0,1);

    std::cout << "n,swapRow_time_s,swapCol_time_s\n";
    for (int n : sizes) {
        // create random n×n matrix (column-major) and write to file
        std::vector<double> M((size_t)n*n);
        for (int j=0; j<n; ++j)
            for (int i=0; i<n; ++i)
                M[(size_t)j*n + i] = dist(rng);
        const char* fname = "swap_test.bin";
        {
            std::ofstream out(fname, std::ios::binary|std::ios::trunc);
            out.write(reinterpret_cast<const char*>(M.data()), (size_t)n*n*sizeof(double));
        }

        double t_row=0, t_col=0;
        for (int t=0; t<trials; ++t) {
            std::fstream fs(fname, std::ios::in|std::ios::out|std::ios::binary);
            int i = rng() % n;
            int j = rng() % n;
            auto s0 = std::chrono::high_resolution_clock::now();
            swapRowsInFile(fs, n, n, i, j);
            auto s1 = std::chrono::high_resolution_clock::now();
            t_row += std::chrono::duration<double>(s1-s0).count();
            fs.close();

            std::fstream fs2(fname, std::ios::in|std::ios::out|std::ios::binary);
            i = rng() % n;
            j = rng() % n;
            s0 = std::chrono::high_resolution_clock::now();
            swapColsInFile(fs2, n, n, i, j);
            s1 = std::chrono::high_resolution_clock::now();
            t_col += std::chrono::duration<double>(s1-s0).count();
            fs2.close();
        }
        t_row /= trials;
        t_col /= trials;
        std::remove(fname);
        std::cout << n << "," << t_row << "," << t_col << "\n";
    }
    return 0;
}
