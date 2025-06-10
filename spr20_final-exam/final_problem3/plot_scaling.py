import matplotlib.pyplot as plt
import csv

threads = []
speedups = []

# Read CSV data
with open('scaling_data.csv', 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        threads.append(int(row['Threads']))
        speedups.append(float(row['Speedup']))

plt.plot(threads, speedups, marker='o', label='Measured')
plt.plot(threads, threads, linestyle='--', label='Ideal (y=nt)')

plt.xlabel('Number of Threads')
plt.ylabel('Speedup (T₁ / Tₙ)')
plt.title('Strong Scaling of Grayscale Filter')
plt.xticks(threads)
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
