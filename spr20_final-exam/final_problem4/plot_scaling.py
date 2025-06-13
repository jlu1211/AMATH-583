import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the scaling results
df = pd.read_csv('scaling_results.csv')

# Calculate ideal scaling (time_1_process / number_of_processes)
time_1_process = df[df['Processes'] == 1]['Wall_Time'].values[0]
ideal_times = [time_1_process / p for p in df['Processes']]

# Create the plot
plt.figure(figsize=(10, 6))

# Plot actual scaling
plt.plot(df['Processes'], df['Wall_Time'], 'bo-', label='Actual Scaling', linewidth=2, markersize=8)

# Plot ideal scaling
plt.plot(df['Processes'], ideal_times, 'r--', label='Ideal Scaling', linewidth=2)

# Add labels and title
plt.xlabel('Number of Processes', fontsize=12)
plt.ylabel('Wall Clock Time (seconds)', fontsize=12)
plt.title('Strong Scaling Study (N=4096)', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=10)

# Set x-axis to show exact process counts
plt.xticks(df['Processes'])

# Add speedup annotations
for i, (p, t) in enumerate(zip(df['Processes'], df['Wall_Time'])):
    speedup = time_1_process / t
    plt.annotate(f'Speedup: {speedup:.2f}x', 
                xy=(p, t),
                xytext=(0, 10),
                textcoords='offset points',
                ha='center',
                fontsize=8)

# Save the plot
plt.tight_layout()
plt.savefig('scaling_plot.png', dpi=300, bbox_inches='tight')
plt.close()

print("Plot has been saved as 'scaling_plot.png'") 