#pragma once
#include <mpi.h>

// A simple point-to-point implementation of broadcast
// T must be trivially copyable; we send raw bytes
template <typename T>
void my_broadcast(T* data, int count, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int bytes = count * sizeof(T);
    if (rank == root) {
        // root sends to all other ranks
        for (int r = 0; r < size; ++r) {
            if (r == root) continue;
            MPI_Send(reinterpret_cast<char*>(data), bytes,
                     MPI_BYTE, r, 0, comm);
        }
    } else {
        // non-root ranks receive from root
        MPI_Recv(reinterpret_cast<char*>(data), bytes,
                 MPI_BYTE, root, 0, comm, MPI_STATUS_IGNORE);
    }
}