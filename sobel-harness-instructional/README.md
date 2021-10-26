# sobel-harness-instructional

This directory contains the code harnesses for doing three different implementation of a Sobel edge detection
filter. The three implementations are:

- CPU only in C++, with your added OpenMP parallelism
- GPU only in CUDA
- CPU n C++ with OpenMP Device Offload for running on the GPU

# Build instructions - general

% source scripts/modules_for_cgpu.sh
% mkdir build  
% cd build  
% cmake ../  
% make

## Part 1

% ./sobel_cpu

## Part 2

% salloc -C gpu -t 15 -c 10 -G 1 -q interactive -A m3930
% srun nvprof ./sobel_gpu 1 256
% srun nvprof -m sm_efficiency ./sobel_gpu 1 256
% sh ../job.in

## Part 3

% salloc -C gpu -t 15 -c 10 -G 1 -q interactive -A m3930
% srun nvprof -m sm_efficiency ./sobel_cpu_omp_offload
% srun nvprof ./sobel_cpu_omp_offload

## To check validity

% module load python
% python scripts/imshow.py data/processed-raw-int8-4x-cpu.dat 7112 5146

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
