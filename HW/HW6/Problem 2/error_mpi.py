#!/usr/bin/env python3
import subprocess
import math
import csv

# Exact analytic length
L_exact = 35.0/8.0 + math.log(6.0)

# Values of n to test
ns = [10**k for k in range(1, 7)]

# Open CSV for writing
with open('mpi_error.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['n', 'approx_length', 'error'])
    for n in ns:
        # Run single MPI rank
        out = subprocess.check_output(['mpirun', '-np', '1', './length_mpi', str(n)])
        # Parse output: "<p>,<time>,<approx_length>"
        _, _, L_str = out.decode().strip().split(',')
        L_approx = float(L_str)
        error = abs(L_approx - L_exact)
        writer.writerow([n, L_approx, error])
        print(f"n={n}: approx={L_approx:.10f}, error={error:.2e}")

print("→ Wrote mpi_error.csv")
