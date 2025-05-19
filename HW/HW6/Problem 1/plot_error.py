#!/usr/bin/env python3
import subprocess, math, csv
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import LogLocator

# Exact analytic length
L_exact = 35/8 + math.log(6)

# Range of n: powers of ten from 10^1 to 10^6
ns = [10**k for k in range(1, 7)]
errors = []

# Run and collect
with open('error.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['n','approx_length','error'])
    for n in ns:
        # single‐thread run: prints "time,approx_length"
        out = subprocess.check_output(['./length_threads', '1', str(n)])
        _, L_str = out.decode().strip().split(',')
        L_approx = float(L_str)
        err = abs(L_approx - L_exact)
        writer.writerow([n, L_approx, err])
        errors.append(err)
        print(f"n={n:7d}  L≈{L_approx:.10f}  error={err:.2e}")

print("Wrote error.csv")

# Plot error vs n
plt.figure()
plt.loglog(ns, errors, 'o-', base=10)
plt.xlabel('Number of intervals $n$')
plt.ylabel(r'Error $E(n)=|L_n-L_{\mathrm{exact}}|$')
plt.title('Convergence of Midpoint Rule')
plt.grid(True, which='both', ls='--', alpha=0.5)
plt.tight_layout()
plt.savefig("error.png")

# Estimate observed order (slope)
p = np.polyfit(np.log(ns), np.log(errors), 1)
print(f"Observed convergence order ≈ {p[0]:.2f}")

plt.show()
