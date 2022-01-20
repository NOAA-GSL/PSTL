#!/bin/bash              

# Partition Requested
#--> #SBATCH -p dgxa100_40g_1tb
#SBATCH -p epyc_a100x4

# GPU Cards Requested
#SBATCH -G 1

# CPU Tasks Requested
#SBATCH -n 1

# Exclusive Node Access
#SBATCH --exclusive

################################################
# This script should be run from within your 
# build directory which is only 1 deep from the 
# main PSTL repo directory.
#
# > pwd
# /path/to/PSTL
#
# mkdir build; cd build
# cp ../scripts/sae_submit.sh .
# sbatch sae_submit.sh
################################################

# ----------------------------------------------
# Load up the Modules                                                                                 
# ----------------------------------------------
module purge
module load nvidia-hpc-sdk/nvhpc/21.7

# ----------------------------------------------
# Display System Info for Debugging Issues
# ----------------------------------------------

# Run nvidia-smi to show we have GPUs
nvidia-smi -L

# Display CPU Info 
lscpu

# ----------------------------------------------
# Build the application on the Node
# ----------------------------------------------
cmake ..
make
make install

# ----------------------------------------------
# Run the tests and get detailed output
# ----------------------------------------------
cd test
bash runall.sh
