# mpi-2dmesh-harness-instructional

This project contains the code harness for doing an MPI-based scatter-process-gather
motif.

What the code provides:

- Loads input files, parses command line arguments, writes output
- Performs domain decomposition using one of three different strategies: row-slab, column-slab, or tiled, sets up tiles on every rank that contain metadata like tile size, extents, etc. , assigns tiles to ranks
- Executes a scattering step by iterating over all tiles and invoking a method to send
  data from rank 0 to other ranks, and from non-zero ranks to receive data from rank 0.
- Executes a processing step by iterating over all tiles and sets the stage for user code to be executed on a tile
- Executes a tile gather step, where data from all tiles not owned by rank 0 is sent back to rank 0
- Writes output to a disk file

# Build instructions, general

Please use the data directory from the HW6 code harness to obtain the input image first.

This distribution uses cmake and MPI.

After downloading, cd into the mpi-2dmesh-harness directory, then:

% mkdir build  
% cd build  
% cmake
% make

# Setting up your NERSC environment

% module swap PrgEnv-intel PrgEnv-gnu  
% module load openmpi  
% module load cmake

## Build

% make
% salloc --nodes 5 --qos interactive --time 00:30:00 --constraint knl --account m3930
% bash ../scripts/run_script.sh ./mpi_2dmesh

## To check validity

Please use the data directory from the HW6 code harness to obtain the input image first.

% module load python

Correct result:
$ python scripts/imshow.py data/correct_results_4x.dat 7112 5146

Your result:
% python scripts/imshow.py data/processed-raw-int8-4x-cpu.dat 7112 5146

## To get performance figures and data in table

% python plot.py

#eof
