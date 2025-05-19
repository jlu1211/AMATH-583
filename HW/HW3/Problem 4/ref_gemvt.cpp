#include "ref_gemvt.hpp"

template void gemv<double>(double,
                           const std::vector<std::vector<double>>&,
                           const std::vector<double>&,
                           double,
                           std::vector<double>&
);

template void gemv<int>(int,
                        const std::vector<std::vector<int>>&,
                        const std::vector<int>&,
                        int,
                        std::vector<int>&
);
