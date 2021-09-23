# Build instructions

After downloading, cd into the main source directly, then:
% mkdir build  
% cd build  
% cmake ../

# Set up OpenMP env

export OMP_PLACES=threads
export OMP_PROC_BIND=spread

For static:
% export OMP_SCHEDULE=static
% export OMP_NUM_THREADS=[1,2,4,8,32,64]

For dynamicL
% export OMP_SCHEDULE=dynamic
% export OMP_NUM_THREADS=[1,2,4,8,32,64]

# Run code

To run basic version:
% ./benchmark-basic

To run OpenMP version:
% ./benchmark-openmp

To run CBLAS version:
% ./benchmark-blas

# Grab a node on Cori

Login:
$ ssh liuzz10@cori.nersc.gov

Change cmake version:
$ module load cmake

Grab an interactive KNL node:
$ salloc --nodes 1 --qos interactive --time 01:00:00 --constraint knl --account m3930

#eof
