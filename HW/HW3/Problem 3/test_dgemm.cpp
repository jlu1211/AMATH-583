#include "ref_dgemm.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>

int main() {
    const int ntrial = 10;
    std::mt19937 gen(0);
    std::uniform_real_distribution<double> dist(0, 1.0);

    std::cout << "n,flops_count,avg_time(s),flops_per_sec\n";
    for (int n = 2; n <= 512; ++n) {
        double flops_count = 2.L * n * n * n + 2.L * n * n;  // 2n^3 + 2n^2 flops
        long double total_time = 0.L;
        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<std::vector<double>> B(n, std::vector<double>(n));
        std::vector<std::vector<double>> C(n, std::vector<double>(n)), C_copy(n, std::vector<double>(n));

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                A[i][j] = B[i][j] = C[i][j] = dist(gen);

        for (int t = 0; t < ntrial; ++t) {
            C_copy = C;
            auto t0 = std::chrono::high_resolution_clock::now();
            dgemm(2.5, A, B, 3.5, C_copy);
            auto t1 = std::chrono::high_resolution_clock::now();
            auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0);
            total_time += dur.count() * 1.e-9L;
        }

        long double avg_time = total_time / static_cast<long double> (ntrial);
        long double flops_per_sec = flops_count / avg_time;

        std::cout
            << n << ','
            << flops_count << ','
            << std::fixed << std::setprecision(15) << avg_time << ','
            << std::fixed << std::setprecision(0) << flops_per_sec
            << '\n';
    }
}