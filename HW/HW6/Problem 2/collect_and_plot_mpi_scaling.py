#!/usr/bin/env python3
# collect_and_plot_mpi_scaling.py

import subprocess
import csv
import matplotlib.pyplot as plt

# --- PARAMETERS ---
procs    = [1, 2, 4, 8, 16, 32]
n_points = 10**8
binary   = "./length_mpi"    # MPI executable from Problem 2(a)

# --- STORAGE ---
times   = []
lengths = []

# --- COLLECT DATA ---
with open('mpi_scaling.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['procs', 'time_s', 'approx_length'])
    for p in procs:
        # Launch p MPI ranks
        cmd = ["mpirun", "-np", str(p), binary, str(n_points)]
        out = subprocess.check_output(cmd).decode().strip()
        # Expected output: "<p>,<time_s>,<approx_length>"
        _, t_str, L_str = out.split(',')
        t = float(t_str)
        L = float(L_str)
        times.append(t)
        lengths.append(L)
        writer.writerow([p, t, L])
        print(f"P={p:2d}  Time={t:.6f}s  Length={L:.10f}")

print("→ Wrote mpi_scaling.csv")

# --- PLOT: Time vs Processes ---
# plt.figure()
# plt.plot(procs, times, 'o-', markersize=6)
# plt.xscale('log', base=2)
# plt.yscale('log')
# plt.xlabel('Number of MPI Processes (p)')
# plt.ylabel('Execution time $T_p$ (s)')
# plt.title(f'MPI Strong Scaling (n={n_points:,})')
# plt.grid(True, which='both', ls='--', alpha=0.5)
# plt.xticks(procs, procs)

# # --- PLOT: Efficiency vs Processes ---
# T1 = times[0]
# efficiency = [T1/(p * tp) for p, tp in zip(procs, times)]

# plt.figure()
# plt.plot(procs, efficiency, 's-', markersize=6)
# plt.xscale('log', base=2)
# plt.xlabel('Number of MPI Processes (p)')
# plt.ylabel(r'Efficiency $E_p = T_1/(p\,T_p)$')
# plt.title('MPI Strong‐Scaling Efficiency')
# plt.ylim(0, 1.05)
# plt.grid(True, which='both', ls='--', alpha=0.5)
# plt.xticks(procs, procs)

# plt.tight_layout()
# plt.show()
