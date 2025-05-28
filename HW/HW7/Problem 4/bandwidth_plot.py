import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV file
df = pd.read_csv("copy_bandwidth.csv")

# Convert bytes to MiB for x-axis readability
df["Size_MiB"] = df["Bytes"] / (1024**2)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(df["Size_MiB"], df["H2D_GBps"], 'o-', label='Host → Device (H2D)')
plt.plot(df["Size_MiB"], df["D2H_GBps"], 's--', label='Device → Host (D2H)')

plt.xscale('log', base=2)
plt.xlabel("Buffer Size (MiB, log₂ scale)")
plt.ylabel("Bandwidth (GiB/s)")
plt.title("CUDA Memory Transfer Bandwidth: Host ↔ Device")
plt.grid(True, which='both', linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig("copy_bandwidth_plot.png", dpi=300)
plt.show()
