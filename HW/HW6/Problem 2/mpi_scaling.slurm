#!/bin/bash
#SBATCH --job-name=mpi_scaling
#SBATCH --account=amath
#SBATCH --partition=cpu-g2
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32      # one task running the Python driver
#SBATCH --mem=20G
#SBATCH --time=02:00:00

#SBATCH --chdir=/Users/chris/Library/CloudStorage/OneDrive-UW/4. Seinor/AMATH 583/HW/HW6/Problem 2  # ← change to your path
#SBATCH --output=mpi_scaling.%j.out
#SBATCH --error=mpi_scaling.%j.err

# Load modules
module load gcc/13.2.0
module load ompi/4.1.6-2
module load coenv/python/3.11.9

# Make sure the driver is executable
chmod +x collect_and_plot_mpi_scaling.py

# Run the Python driver (it will internally launch mpirun for p=1,2,4,8,16,32)
./collect_and_plot_mpi_scaling.py
