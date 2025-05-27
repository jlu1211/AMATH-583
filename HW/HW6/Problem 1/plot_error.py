#!/usr/bin/env python3
import subprocess, math, csv
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import LogLocator
from decimal import Decimal, getcontext

getcontext().prec = 28

L_exact = Decimal('35')/Decimal('8') + Decimal(str(math.log(6)))
ns = [10**k for k in range(1, 7)]
errors = []

with open('error.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['n','approx_length','error'])
    for n in ns:
        # single‐thread run: prints "time,approx_length"
        out = subprocess.check_output(['./length_threads', '1', str(n)])
        _, L_str = out.decode().strip().split(',')
        L_approx = Decimal(L_str)
        err = abs(L_approx - L_exact)
        writer.writerow([n, float(L_approx), float(err)])
        errors.append(float(err))
        print(f"n={n:7d}  L≈{L_approx:.20f}  error={err:.2e}")

print("Wrote error.csv")

plt.figure()
plt.loglog(ns, errors, 'o-', base=10)
plt.xlabel('Number of intervals $n$')
plt.ylabel(r'Error $E(n)=|L_n-L_{\mathrm{exact}}|$')
plt.title('Convergence of Midpoint Rule')
plt.grid(True, which='both', ls='--', alpha=0.5)
plt.tight_layout()
plt.savefig("error.png")

p = np.polyfit(np.log(ns), np.log(errors), 1)
print(f"Observed convergence order ≈ {p[0]:.2f}")

plt.show()
