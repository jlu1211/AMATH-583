#ifndef REF_DAXPY_HPP
#define REF_DAXPY_HPP
#include<vector>
#include<stdexcept>
void daxpy(double a,
           const std::vector<double> &x,
           std::vector<double> &y);
#endif