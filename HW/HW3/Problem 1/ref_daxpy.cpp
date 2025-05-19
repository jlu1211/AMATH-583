#include "ref_daxpy.hpp"
#include<vector>
#include<stdexcept>
#include<random>
#include<iostream>

void daxpy(double a, const std::vector<double> &x, std::vector<double> &y) {
    size_t n = x.size();
    if (n != y.size()) {
        throw std::invalid_argument("daxpy: x and y must have the same size");
    }
    for (size_t i = 0; i < n; ++i) {
        y[i] = a * x[i] + y[i];
    }
}

// int main() {
//     const int ntrial = 3;
//     std::mt19937 gen(0);
//     std::uniform_real_distribution<double> dist(-1.0, 1.0);
//     std::cout << "n,flops_per_sec\n";
//     for (int n = 2; n <= 512; ++n) {
//         long double elapsed_time = 0.L;
//         std::vector<double> x(n), y(n), y_copy(n);
//         for (int i = 0; i < n; i++) {
//             x[i] = dist(gen);
//             y[i] = dist(gen);
//         }
//         for (int t = 0; t < ntrial; ++t) {
//             y_copy = y;
//             auto start = std::chrono::high_resolution_clock::now();
//             daxpy(1.23, x, y_copy);
//             auto stop = std::chrono::high_resolution_clock::now();
//             auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
//             elapsed_time += (duration.count() * 1.e-9);
//         }
//         long double avg_time = elapsed_time / ntrial;
//         double flops_count = 2.L * n;
//         double perf = flops_count / avg_time;
//         std::cout << n << "," << perf << "\n";
//     }
//     return 0;
// }