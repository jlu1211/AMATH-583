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

inline double integrand(double x) {
    double fp = 1.0/x - x/4.0;
    return std::sqrt(1 + fp*fp);
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
    const double a = 1.0, b = 6.0;
    double dx = (b - a) / double(n);

    uint64_t base = n/size;
    uint64_t remainder = n % size;

    uint64_t start = rank * base + std::min<uint64_t>(rank, remainder);
    uint64_t local_n = base + (rank < remainder ? 1 : 0);
    uint64_t end = start + local_n;

    // time the local computation
    auto start_time = MPI_Wtime();
    double local_sum = 0.0;
    for (uint64_t i = start; i < end; ++i) {
        double x = a + (i + 0.5) * dx;
        local_sum += integrand(x);
    }
    local_sum *= dx;
    auto end_time = MPI_Wtime();

    //Reduce to get global sum
    double global_length = 0.0;
    MPI_Reduce(&local_sum, &global_length, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Gather timings from all processes
    double local_time = end_time - start_time;
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank ==0 ) {
        std::cout << size << "," << std::setprecision(6) << max_time << "," << std::setprecision(10) << global_length << std::endl;
    }

    MPI_Finalize();
    return 0;
}