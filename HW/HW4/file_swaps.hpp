#ifndef FILE_SWAPS_HPP
#define FILE_SWAPS_HPP

#include<fstream>
#include<vector>
#include<stdexcept>

void swapRowsInFile(std::fstream &file,
                    int nRows,
                    int nCols,
                    int i,
                    int j) {
    if (i < 0 || j < 0 || i >= nRows || j >= nRows) {
        throw std::out_of_range("Row index out of range");
    }
    double vi, vj;
    for (int c = 0; c < nCols; ++c) {
        std::streamoff off_i = (static_cast<std::streamoff>(c)*nRows + i)
                               * sizeof(double);
        std::streamoff off_j = (static_cast<std::streamoff>(c)*nRows + j)
                               * sizeof(double);

        file.seekg(off_i, std::ios::beg);
        file.read(reinterpret_cast<char*>(&vi), sizeof(double));
        file.seekg(off_j, std::ios::beg);
        file.read(reinterpret_cast<char*>(&vj), sizeof(double));

        file.seekp(off_i, std::ios::beg);
        file.write(reinterpret_cast<const char*>(&vj), sizeof(double));
        file.seekp(off_j, std::ios::beg);
        file.write(reinterpret_cast<const char*>(&vi), sizeof(double));
    }
    file.flush();
}

void swapColsInFile(std::fstream &file,
                    int nRows,
                    int nCols,
                    int i,
                    int j) {
    if (i < 0 || j < 0 || i >= nCols || j >= nCols) {
        throw std::out_of_range("Columns index out of range");
    }
    std::vector<double> colI(nRows), colJ(nRows);
    std::streamoff off_i0 = static_cast<std::streamoff>(i)*nRows*sizeof(double);
    std::streamoff off_j0 = static_cast<std::streamoff>(j)*nRows*sizeof(double);

    file.seekg(off_i0, std::ios::beg);
    file.read(reinterpret_cast<char*>(colI.data()), nRows*sizeof(double));
    file.seekg(off_j0, std::ios::beg);
    file.read(reinterpret_cast<char*>(colJ.data()), nRows*sizeof(double));

    file.seekp(off_i0, std::ios::beg);
    file.write(reinterpret_cast<const char*>(colJ.data()), nRows*sizeof(double));
    file.seekp(off_j0, std::ios::beg);
    file.write(reinterpret_cast<const char*>(colI.data()), nRows*sizeof(double));

    file.flush();
}
#endif