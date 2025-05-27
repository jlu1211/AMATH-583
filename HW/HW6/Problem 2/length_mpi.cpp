#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <cstdio>

inline long double integrand(long double x) {
    long double fp = 1.0L/x - x/4.0L;
    return std::sqrt(1.0L + fp*fp);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    uint64_t n = std::stoull(argv[1]);
    const long double a = 1.0L, b = 6.0L;
    long double dx = (b - a) / static_cast<long double>(n);

    uint64_t base = n/size;
    uint64_t remainder = n % size;

    uint64_t start = rank * base + std::min<uint64_t>(rank, remainder);
    uint64_t local_n = base + (rank < remainder ? 1 : 0);
    uint64_t end = start + local_n;

    // time the local computation
    auto start_time = MPI_Wtime();
    long double local_sum = 0.0L;
    for (uint64_t i = start; i < end; ++i) {
        long double x = a + (i + 0.5L) * dx;
        local_sum += integrand(x);
    }
    local_sum *= dx;
    auto end_time = MPI_Wtime();

    //Reduce to get global sum
    long double global_length = 0.0L;
    MPI_Reduce(&local_sum, &global_length, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Gather timings from all processes
    double local_time = end_time - start_time;
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << size << "," << std::setprecision(6) << max_time << "," << std::setprecision(20) << global_length << std::endl;
    }

    MPI_Finalize();
    return 0;
}