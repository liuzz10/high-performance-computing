# Build instructions

After downloading, cd into the main source directly, then:
% mkdir build  
% cd build  
% cmake ../

# Run code

## To run basic version:

% ./benchmark-basic

## To run OpenMP version, you need to set up OpenMP env first:

export OMP_PLACES=threads
export OMP_PROC_BIND=spread

For static:
% export OMP_SCHEDULE=static

For dynamicL
% export OMP_SCHEDULE=dynamic

Run code:
% sh ../job.in

## To run CBLAS version:

% ./benchmark-blas

# Grab a node on Cori

Login:
$ ssh [username]@cori.nersc.gov

Change cmake version:
$ module load cmake

Grab an interactive KNL node:
$ salloc --nodes 1 --qos interactive --time 01:00:00 --constraint knl --account m3930

#eof
