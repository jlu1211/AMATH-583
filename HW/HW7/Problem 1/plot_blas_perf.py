import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('blas_perf.csv')

# Create the plot
plt.figure(figsize=(10, 6))

# Plot each level with different colors and markers
for level in sorted(df['level'].unique()):
    level_data = df[df['level'] == level]
    plt.plot(level_data['N'], level_data['MLOPS'], 
             marker='o', 
             label=f'Level {level}',
             linestyle='-',
             markersize=6)

# Set log scale for both axes since the data spans multiple orders of magnitude
plt.xscale('log', base=2)
# plt.yscale('log')

# Add labels and title
plt.xlabel('Matrix Size (N)')
plt.ylabel('Performance (MLOPS)')
plt.title('BLAS Performance by Level')
plt.grid(True, which="both", ls="-", alpha=0.2)

# Add legend
plt.legend()

# Adjust layout to prevent label cutoff
plt.tight_layout()

# Save the plot
plt.savefig('blas_performance_plot.png', dpi=300, bbox_inches='tight')
plt.close() 