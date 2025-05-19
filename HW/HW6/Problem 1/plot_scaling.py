#!/usr/bin/env python3
import subprocess
import csv
import matplotlib.pyplot as plt

# --- PARAMETERS ---
threads  = [1, 2, 4, 8, 16]
n_points = 10**8           # or whatever n you choose
binary   = "./length_threads"

# --- COLLECT DATA ---
times   = []
lengths = []
with open('scaling.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['threads','time_s','approx_length'])
    for p in threads:
        # call: length_threads <p> <n>  → prints "time,approx_length"
        out = subprocess.check_output([binary, str(p), str(n_points)])
        t_str, L_str = out.decode().strip().split(',')
        t = float(t_str); L = float(L_str)
        writer.writerow([p, t, L])
        times.append(t)
        lengths.append(L)
        print(f"p={p:2d}  T={t:.4f}s  L≈{L:.8f}")

print("Wrote scaling.csv")

# --- PLOT: Time vs Threads ---
plt.figure()
plt.plot(threads, times, 'o-')
plt.xscale('log', base=2)
plt.yscale('log')
plt.xlabel('Number of threads (p)')
plt.ylabel('Execution time $T_p$ (s)')
plt.title(f'Strong Scaling (n={n_points:,})')
plt.grid(True, which='both', ls='--', alpha=0.5)
plt.xticks(threads, threads)
plt.savefig("scaling_time_vs_threads.png")

# --- PLOT: Efficiency E_p = T1/(p * Tp) ---
T1 = times[0]
eff = [T1/(p * Tp) for p, Tp in zip(threads, times)]

plt.figure()
plt.plot(threads, eff, 's-')
plt.xscale('log', base=2)
plt.xlabel('Number of threads (p)')
plt.ylabel(r'Efficiency $E_p = T_1/(p\,T_p)$')
plt.title('Strong‐Scaling Efficiency')
plt.ylim(0, 1.05)
plt.grid(True, which='both', ls='--', alpha=0.5)
plt.xticks(threads, threads)
plt.savefig("scaling_efficiency.png")

plt.show()
