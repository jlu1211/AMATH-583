#ifndef REF_AXPYT_HPP
#define REF_AXPYT_HPP
#include<stdexcept>
#include <vector>
template <typename T>
void axpy(T a, const std::vector<T>& x, std::vector<T>& y) {
    size_t n = x.size();
    if (n != y.size()) {
        throw std::invalid_argument("daxpy: x and y must have the same size");
    }
    for (size_t i = 0; i < n; ++i) {
        y[i] = a * x[i] + y[i];
    }
}
#endif
