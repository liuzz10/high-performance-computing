#include <iostream>
#include <omp.h>
#include "likwid-stuff.h"

const char* dgemm_desc = "Basic implementation, OpenMP-enabled, three-loop dgemm.";

/*
 * This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in column-major format.
 * On exit, A and B maintain their input values.
 */
void square_dgemm(int n, double* A, double* B, double* C) 
{
   // insert your code here: implementation of basic matrix multiply with OpenMP parallelism enabled

   // be sure to include LIKWID_MARKER_START(MY_MARKER_REGION_NAME) inside the block of parallel code,
   // but before your matrix multiply code, and then include LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME)
   // after the matrix multiply code but before the end of the parallel code block.

// std::cout << "Insert your basic matrix multiply, openmp-parallel edition here " << std::endl;

   #pragma omp parallel
   {
   LIKWID_MARKER_START(MY_MARKER_REGION_NAME);
   #pragma omp for
   for (off_t j = 0; j < n; j++) {
      for (off_t i = 0; i < n; i++) {
         double Ccopy;
         Ccopy = C[i+j*n];
         for (off_t k = 0; k < n; k++) {
            Ccopy += + A[i+k*n] * B[k+j*n];
         }
         C[i+j*n] = Ccopy;
      }
   }
   LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME);
   }
}
