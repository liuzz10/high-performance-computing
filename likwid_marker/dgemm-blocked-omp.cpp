#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "likwid-stuff.h"
#include <vector>

const char* dgemm_desc = "Blocked dgemm, OpenMP-enabled";


/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in column-major format.
 * On exit, A and B maintain their input values. */

void copy_matrix_to_local(double* local_matrix, double* memory_matrix, int block_size, int n, off_t i, off_t j) {
   memory_matrix += i * block_size + j * block_size * n;
   for (off_t offset = 0; offset < block_size; offset++) {
      memcpy(local_matrix + offset * block_size, memory_matrix + offset * n, sizeof(double)*block_size);
   }
}

void copy_matrix_to_memory(double* memory_matrix, double* local_matrix, int block_size, int n, off_t i, off_t j) {
   memory_matrix += i * block_size + j * block_size * n;
   for (off_t offset = 0; offset < block_size; offset++) {
      memcpy(memory_matrix + offset * n, local_matrix + offset * block_size, sizeof(double)*block_size);
   }
}

void square_dgemm_blocked(int n, int block_size, double* A, double* B, double* C) 
{
   // insert your code here: implementation of blocked matrix multiply with copy optimization and OpenMP parallelism enabled

   // be sure to include LIKWID_MARKER_START(MY_MARKER_REGION_NAME) inside the block of parallel code,
   // but before your matrix multiply code, and then include LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME)
   // after the matrix multiply code but before the end of the parallel code block.

   off_t num_blocks = n / block_size;

#pragma omp parallel
   {
   LIKWID_MARKER_START(MY_MARKER_REGION_NAME);
   #pragma omp for collapse(2)
   for (off_t j = 0; j < num_blocks; j++) {
      for (off_t i = 0; i < num_blocks; i++) {
         // create local copy of A, B, C 
	 std::vector<double> buf(3 * block_size * block_size);
	 double* Acopy = buf.data() + 0;
	 double* Bcopy = Acopy + block_size * block_size;
	 double* Ccopy = Bcopy + block_size * block_size;
	 // copy block C[i, j] into cache
         copy_matrix_to_local(Ccopy, C, block_size, n, i, j);
         for (off_t k = 0; k < num_blocks; k++) {
            // copy block A[i, k] and block B[k, j] into cache
            copy_matrix_to_local(Acopy, A, block_size, n, i, k);
            copy_matrix_to_local(Bcopy, B, block_size, n, k, j);
            // mmul on blocks
            for (off_t y = 0; y < block_size; y++) {
               for (off_t x = 0; x < block_size; x++) {
                  for (off_t z = 0; z < block_size; z++) {
                     Ccopy[x+y*block_size] += Acopy[x+z*block_size] * Bcopy[z+y*block_size];
                  }
               }
            }
         }
         copy_matrix_to_memory(C, Ccopy, block_size, n, i, j);
      }
   }
   LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME);
   }
}
