#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>

inline long double integrand(long double x) {
    long double fp = 1.0L/x - x/4.0L;
    return std::sqrt(1.0L + fp*fp);
}

void worker(long double a, long double b, uint64_t start_i, uint64_t end_i,
            uint64_t n, long double &partial_sum) {
    long double dx = (b-a) / static_cast<long double>(n);
    long double sum = 0.0L;
    // Midpoint Riemann
    for (uint64_t i = start_i; i < end_i; ++i) {
        long double x = a + (i + 0.5L) * dx;
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

    const long double a = 1.0L, b = 6.0L;
    std::vector<std::thread> threads;
    std::vector<long double> partials(num_threads, 0.0L);
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

    long double total_sum = 0.0L;
    for (long double p : partials) {
        total_sum += p;
    }

    double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
    std::cout << seconds << "," << std::setprecision(20) << total_sum << std::endl;
    return 0;
}