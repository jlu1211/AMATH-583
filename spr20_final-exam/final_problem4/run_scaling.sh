#!/bin/bash

# Load required modules
module load gcc intel/oneAPI/2023.2.1

# Compile the code
source cmpl.krazeb

# Create output file for timing results
echo "Processes,Wall_Time" > scaling_results.csv

# Run scaling tests
echo "Running 1 process..."
mpirun -np 1 ./xkrazeb 1 1 48 48 4096 | grep "wall_t:" | awk '{print "1," $NF}' >> scaling_results.csv

echo "Running 2 processes..."
mpirun -np 2 ./xkrazeb 1 2 48 48 4096 | grep "wall_t:" | awk '{print "2," $NF}' >> scaling_results.csv

echo "Running 4 processes..."
mpirun -np 4 ./xkrazeb 2 2 48 48 4096 | grep "wall_t:" | awk '{print "4," $NF}' >> scaling_results.csv

echo "Running 8 processes..."
mpirun -np 8 ./xkrazeb 2 4 48 48 4096 | grep "wall_t:" | awk '{print "8," $NF}' >> scaling_results.csv

echo "Running 16 processes..."
mpirun -np 16 ./xkrazeb 4 4 48 48 4096 | grep "wall_t:" | awk '{print "16," $NF}' >> scaling_results.csv

echo "Scaling study complete. Results saved in scaling_results.csv" 