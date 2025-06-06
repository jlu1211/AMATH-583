import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

cuda_df = pd.read_csv('cufft_only_results.csv')
cpu_df = pd.read_csv('fftw_only_results.csv')

fig, ax1 = plt.subplots(1, 1, figsize=(15, 6))

ax1.plot(cuda_df['n'], cuda_df['gflops_cufft'], 'b-o', label='CUFFT')
ax1.plot(cpu_df['n'], cpu_df['gflops_fftw'], 'r-o', label='FFTW')
ax1.set_xlabel('Cubic Lattice Dimension (n)')
ax1.set_ylabel('GFLOPs')
ax1.set_title('3D Gradient FFT Performance')
ax1.grid(True)
ax1.legend()

plt.tight_layout()
plt.savefig('fft_comparison.png', dpi=300, bbox_inches='tight')
plt.show() 