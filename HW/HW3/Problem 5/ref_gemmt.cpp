#include "ref_gemmt.hpp"

template void gemm<double>(double,
    const std::vector<std::vector<double>>&, 
    const std::vector<std::vector<double>>&, 
    double,
    std::vector<std::vector<double>>&);

template void gemm<int>(int,
    const std::vector<std::vector<int>>&, 
    const std::vector<std::vector<int>>&, 
    int,
    std::vector<std::vector<int>>&);