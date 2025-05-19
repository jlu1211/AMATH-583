#include "ref_dgemv.hpp"
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
        double flops_count = 2.L * n * n + 2.L * n;  // 2n^2 + 2n flops

        long double total_time = 0.L;
        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<double> x(n), y(n), y_copy(n);

        for (int i = 0; i < n; ++i) {
            x[i] = y[i] = dist(gen);
            for (int j = 0; j < n; ++j)
                A[i][j] = dist(gen);
        }

        for (int t = 0; t < ntrial; ++t) {
            y_copy = y;
            auto t0 = std::chrono::high_resolution_clock::now();
            dgemv(2.5, A, x, 3.5, y_copy);
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
