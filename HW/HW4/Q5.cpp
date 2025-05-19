#include "io_bandwidth.hpp"
#include<chrono>
#include<iostream>
#include<vector>
#include<random>
#include<cstdio>

int main() {
    std::vector<int> sizes = {32,64,128,256,512,1024,2048,4096,8192,16384};
    const int trials = 3;
    std::mt19937_64 rng(0);
    std::uniform_real_distribution<double> dist(0,1);

    std::cout << "n,write_BW,read_BW\n";
    for (int n : sizes) {
        // prepare column-major data
        std::vector<double> M((size_t)n*n);
        for (int j = 0; j < n; ++j)
            for (int i = 0; i < n; ++i)
                M[(size_t)j*n + i] = dist(rng);

        const std::string tmp = "tmp.bin";
        std::remove(tmp.c_str());

        double write_time = 0, read_time = 0;
        for (int t = 0; t < trials; ++t) {
            auto t0 = std::chrono::high_resolution_clock::now();
            writeMatrixBinary(tmp, M, n);
            auto t1 = std::chrono::high_resolution_clock::now();
            write_time += std::chrono::duration<double>(t1-t0).count();

            t0 = std::chrono::high_resolution_clock::now();
            auto R = readMatrixBinary(tmp, n);
            t1 = std::chrono::high_resolution_clock::now();
            read_time += std::chrono::duration<double>(t1-t0).count();
        }
        std::remove(tmp.c_str());

        write_time /= trials;
        read_time  /= trials;
        double bytes = double(n)*n*sizeof(double);
        double bw_write = bytes / write_time;
        double bw_read  = bytes / read_time;

        std::cout << n << "," << bw_write << "," << bw_read << "\n";
    }
    return 0;
}