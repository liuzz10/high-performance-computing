# Build instructions

After downloading, cd into the main source directly, then:
$ mkdir build  
$ cd build  
$ cmake ../

# Run code

Make:
$ make

## To run basic version:

No need to modify job-basic-omp.

$ bash job-basic-omp

## To run blocked version:

Modify job-blocked-omp: (1) uncomment lines containing the for loop over block sizes and the closing done (2) on the line containing the srun command, add "-B $B" to the end of the line, you are adding a new command-line argument to be passed into your code.

$ bash job-blocked-omp

## To run CBLAS version:

Modify job-cblas: (1) comment out the loop over concurrency, we will be running the benchmark-blas code only in serial; (2) on the containing the srun command, remove "-C N:0-$maxcore" and replace it with "-C N:0"

$ bash job-blas

# Grab a node on Cori

Login:
$ ssh [username]@cori.nersc.gov

Change cmake version:
$ module load cmake

Env setup:
$ module swap PrgEnv-intel PrgEnv-gnu

Download likwid:
$ module load likwid/5.2.0

Grab an interactive KNL node:
$ salloc --nodes 1 --qos interactive --time 01:00:00 --constraint knl --account m3930

#eof
