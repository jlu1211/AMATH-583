# README.txt

# 1. Build shared library librefBLAS.so from Problems 1–4
mkdir -p "Problem 5"
g++ -std=c++17 -c -fPIC "Problem 1/ref_daxpy.cpp"   -o "Problem 5/ref_daxpy.o"
g++ -std=c++17 -c -fPIC "Problem 2/ref_dgemv.cpp"   -o "Problem 5/ref_dgemv.o"
g++ -std=c++17 -c -fPIC "Problem 3/ref_dgemm.cpp"   -o "Problem 5/ref_dgemm.o"
g++ -std=c++17 -c -fPIC "Problem 4/ref_axpyt.cpp"   -o "Problem 5/ref_axpyt.o"
g++ -std=c++17 -c -fPIC "Problem 4/ref_gemvt.cpp"   -o "Problem 5/ref_gemvt.o"
g++ -std=c++17 -c -fPIC "Problem 4/ref_gemmt.cpp"   -o "Problem 5/ref_gemmt.o"

g++ -shared -o "Problem 5/librefBLAS.so" \
    "Problem 5/ref_daxpy.o" "Problem 5/ref_dgemv.o" "Problem 5/ref_dgemm.o" \
    "Problem 5/ref_axpyt.o" "Problem 5/ref_gemvt.o" "Problem 5/ref_gemmt.o"

# 2. Compile performance drivers
#    Problem 1: DAXPY
g++ -std=c++17 -O2 \
    -I. -I"Problem 1" \
    -L"Problem 5" -lrefBLAS \
    "Problem 1/test_daxpy.cpp" -o "Problem 1/test_daxpy"

#    Problem 2: DGEMV
g++ -std=c++17 -O2 \
    -I. -I"Problem 2" \
    -L"Problem 5" -lrefBLAS \
    "Problem 2/test_dgemv.cpp" -o "Problem 2/test_dgemv"

#    Problem 3: DGEMM
g++ -std=c++17 -O2 \
    -I. -I"Problem 3" \
    -L"Problem 5" -lrefBLAS \
    "Problem 3/test_dgemm.cpp" -o "Problem 3/test_dgemm"

# 3. Ensure the loader can find librefBLAS.so at runtime
export LD_LIBRARY_PATH="$(pwd)/Problem 5"

# 4. Run performance tests and save CSVs
./Problem\ 1/test_daxpy  > "Problem 1/q1.csv"
./Problem\ 2/test_dgemv  > "Problem 2/q2.csv"
./Problem\ 3/test_dgemm  > "Problem 3/q3.csv"

# 5. Compile correctness tester (test_driver.cpp) in Problem 5
g++ -std=c++17 -O2 \
    -I. \
    -I"Problem 1" -I"Problem 2" -I"Problem 3" -I"Problem 4" \
    -L"Problem 5" -lrefBLAS \
    "Problem 5/test_driver.cpp" \
    -o "Problem 5/test_driver"

# 6. Run correctness tests and capture output
./Problem\ 5/test_driver > "Problem 5/accuracy_test.txt"
