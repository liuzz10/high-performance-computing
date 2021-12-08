# Build instructions

% mkdir build  
% cd build  
% cmake ../  
% make

## Run program

% srun -n 1 ./convolution <dimension of matrix>
for example, `% ./convolution 10 100` will calculate a matrix of size 10*10, with 3 channels, applying 100 filter of size 3*3. The matrix and filter will be filled with random numbers to mock up an image input, so we don't need to pass a image.

## To run OpenMP version, you need to set up OpenMP env first:

export OMP_PLACES=threads
export OMP_PROC_BIND=spread
export OMP_NUM_THREADS=<number of threads>

## To check validity

The program can print out the output and check if the output channel has correct output. Uncomment line `print(out_data, channel_dimension*OUTPUT_CHANNEL, channel_dimension)` to print and check validity by passing a smaller dimension parameter.

# Grab a node on Cori

Login:
$ ssh [username]@cori.nersc.gov

Env setup:
$ module load cmake
% module load openmpi

Grab an interactive KNL node:
$ salloc --nodes 1 --qos interactive --time 01:00:00 --constraint knl --account m3930

#eof
