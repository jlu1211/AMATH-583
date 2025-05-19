1: Question 2
g++ -std=c++17 -march=native -O0 main.cpp -o bench_O0
./bench_O0 > data_O0.csv

g++ -std=c++17 -march=native -O3 main.cpp -o bench_O3
./bench_O3 > data_O3.csv
