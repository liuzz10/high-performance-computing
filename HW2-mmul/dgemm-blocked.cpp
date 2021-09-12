#include <iostream>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>

const char* dgemm_desc = "Blocked dgemm.";

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
   off_t num_blocks = n / block_size;
   // Create copy of A, B, C
   std::vector<double> buf(3 * block_size * block_size);
   double* Acopy = buf.data() + 0;
   double* Bcopy = Acopy + block_size * block_size;
   double* Ccopy = Bcopy + block_size * block_size;

   std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
   for (off_t i = 0; i < num_blocks; i++) {
      for (off_t j = 0; j < num_blocks; j++) {
         // copy block C[i, j] into cache
         copy_matrix_to_local(Ccopy, C, block_size, n, i, j);
         for (off_t k = 0; k < num_blocks; k++) {
            // copy block A[i, k] and block B[k, j] into cache
            copy_matrix_to_local(Acopy, A, block_size, n, i, k);
            copy_matrix_to_local(Bcopy, B, block_size, n, k, j);
            // mmul on blocks
            for (off_t x = 0; x < block_size; x++) {
               for (off_t y = 0; y < block_size; y++) {
                  for (off_t z = 0; z < block_size; z++) {
                     Ccopy[x+y*block_size] += Acopy[x+z*block_size] * Bcopy[z+y*block_size];
                  }
               }
            }
         }
         copy_matrix_to_memory(C, Ccopy, block_size, n, i, j);
      }
   }
   std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = end_time - start_time;
   std::cout << " Problem size is : " << n << " " << std::endl;
   std::cout << " Block size is : " << block_size << " " << std::endl;
   std::cout << " Elapsed time is : " << elapsed.count() << " " << std::endl;
}
