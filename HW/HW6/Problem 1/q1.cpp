#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>

inline double integrand(double x) {
    double fp = 1.0/x - x/4.0;
    return std::sqrt(1.0 + fp*fp);
}

void worker(double a, double b, uint64_t start_i, uint64_t end_i,
            uint64_t n, double &partial_sum) {
    double dx = (b-a) / double(n);
    double sum = 0.0;
    // Midpoint Riemann
    for (uint64_t i = start_i; i < end_i; ++i) {
        double x = a + (i + 0.5) * dx;
        sum += integrand(x);
    }
    partial_sum = sum * dx;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <number of threads> <number of intervals> <print time (0/1)>" << std::endl;
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    uint64_t n = std::stoull(argv[2]);

    const double a = 1.0, b = 6.0;
    std::vector<std::thread> threads;
    std::vector<double> partials(num_threads, 0.0);
    std::vector<uint64_t> starts(num_threads), ends(num_threads);

    uint64_t base = n / num_threads;
    uint64_t remainder = n % num_threads;
    uint64_t idx = 0;
    for (int t = 0; t < num_threads; ++t) {
        uint64_t chunk = base + (t < remainder ? 1 : 0);
        starts[t] = idx;
        ends[t] = idx + chunk;
        idx += chunk;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back(worker, a, b, starts[t], ends[t], n, std::ref(partials[t]));
    }

    for (auto &th : threads) {
        th.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    double total_sum = 0.0;
    for (double p : partials) {
        total_sum += p;
    }

    double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
    std::cout << seconds << "," << std::setprecision(10) << total_sum << std::endl;
    return 0;
}