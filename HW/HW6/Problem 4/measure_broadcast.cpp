#include "my_broadcast.hpp"
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // message sizes: 8 B, 16 B, ..., 256 MB
    std::vector<size_t> sizes;
    for (size_t b = 8; b <= 256ul*1024*1024; b *= 2) {
        sizes.push_back(b);
    }

    if (rank == 0) {
        std::cout << "procs,bytes,my_bw,mpi_bw\n";
    }

    for (auto bytes : sizes) {
        std::vector<char> buffer(bytes);

        // synchronize
        MPI_Barrier(MPI_COMM_WORLD);
        // time my_broadcast
        long double t0 = MPI_Wtime();
        my_broadcast(buffer.data(), bytes, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        long double t1 = MPI_Wtime();
        long double my_bw = bytes / (t1 - t0);

        // time MPI_Bcast
        MPI_Barrier(MPI_COMM_WORLD);
        long double t2 = MPI_Wtime();
        MPI_Bcast(buffer.data(), bytes, MPI_BYTE, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        long double t3 = MPI_Wtime();
        long double mpi_bw = bytes / (t3 - t2);

        if (rank == 0) {
            std::cout << size << ","
                      << bytes << ","
                      << std::fixed << std::setprecision(2) << my_bw << ","
                      << mpi_bw << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}