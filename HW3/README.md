# Build instructions

After downloading, cd into the main source directly, then:
% mkdir build  
% cd build  
% cmake ../

# Set up env

% export OMP_SCHEDULE="static,[number of concurrency]"

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
