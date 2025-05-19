#ifndef REF_DGEMM_HPP
#define REF_DGEMM_HPP
#include<vector>
#include<chrono>

void dgemm(double a, const std::vector<std::vector<double>> &A,
           const std::vector<std::vector<double>> &B, double b,
           std::vector<std::vector<double>> &C);
#endif
