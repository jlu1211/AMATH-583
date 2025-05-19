#ifndef IO_BANDWIDTH_HPP
#define IO_BANDWIDTH_HPP

#include<string>
#include<vector>
#include<fstream>
#include<stdexcept>

inline void writeMatrixBinary(const std::string &filename,
                              const std::vector<double> &data,
                              int n) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Error opening file for writing: " + filename);
    }
    outFile.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(n)*n*sizeof(double));
    outFile.close();
}

inline std::vector<double> readMatrixBinary(const std::string &filename,
                                            int n) {
    std::ifstream inFile(filename, std::ios::binary);
    std::vector<double> data(n * n);
    inFile.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(n) * n * sizeof(double));
    if (!inFile) {
        throw std::runtime_error("Error reading data from file: " + filename);
    }
    inFile.close();
    return data;
}
#endif