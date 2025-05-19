#include "ref_axpyt.hpp"

template void axpy<double>(double,
                  const std::vector<double>&,
                  std::vector<double>&);

template void axpy<int>(int,
               const std::vector<int>&,
               std::vector<int>&);