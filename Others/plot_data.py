import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("data.txt", delimiter=',')
x = data[:, 0]
y = data[:, 1]

plt.figure(figsize=(8,4))
plt.plot(x, y, color='blue', label='cos(x)')  # Added label for the legend
plt.xlabel(r'$\theta$ (radians)')  # Uncommented xlabel
plt.ylabel(r'$\cos(\theta)$')      # Uncommented ylabel
plt.title('Cosine Function')
plt.grid(True)
plt.legend()  # This will now work because a label is provided
plt.tight_layout()

plt.savefig("cosine_plot.png")
print("[Python] Saved cosine_plot.png")