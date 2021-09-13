#include <iostream>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <iomanip>

const char* dgemm_desc = "Basic implementation, three-loop dgemm.";

/*
 * This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in column-major format.
 * On exit, A and B maintain their input values.
 */
void square_dgemm(int n, double* A, double* B, double* C) 
{
   // insert your code here: implementation of basic matrix multiple
   std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
   for (off_t j = 0; j < n; j++) {
      for (off_t i = 0; i < n; i++) {
         for (off_t k = 0; k < n; k++) {
            C[i+j*n] = C[i+j*n] + A[i+k*n] * B[k+j*n];
         }
      }
   }
   std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = end_time - start_time;
   std::cout << " Problem size is : " << n << " " << std::endl;
   std::cout << " Elapsed time is : " << std::fixed << std::setprecision(9) << elapsed.count() << " " << std::endl;
}
