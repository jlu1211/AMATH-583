import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV file
df = pd.read_csv("gflops_vs_n.csv")

# Extract columns
n = df["n"]
openblas = df["OpenBLAS_GFLOPS"]
cublas = df["cuBLAS_GFLOPS"]

# Plot
plt.figure(figsize=(10, 6))
plt.plot(n, openblas, 'o-', label="OpenBLAS (CPU)")
plt.plot(n, cublas, 's--', label="cuBLAS (GPU)")

# Log x-axis and y-axis
plt.xscale('log', base=2)
plt.yscale('log')
plt.xticks(n, rotation=45)
plt.xlabel("Matrix size n (log₂ scale)")
plt.ylabel("GFLOPS")
plt.title("GEMM Performance Comparison: OpenBLAS vs cuBLAS (Double Precision)")
plt.grid(True, which='both', linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()

# Save and show
plt.savefig("gemm_performance_plot.png", dpi=300)
plt.show()
