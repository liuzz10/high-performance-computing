#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>


const char* dgemv_desc = "OpenMP dgemv.";

/*
 * This routine performs a dgemv operation
 * Y :=  A * X + Y
 * where A is n-by-n matrix stored in row-major format, and X and Y are n by 1 vectors.
 * On exit, A and X maintain their input values.
 */

void my_dgemv(int n, double* A, double* x, double* y) {

   // #pragma omp parallel
   // {
   //    int nthreads = omp_get_num_threads();
   //    int thread_id = omp_get_thread_num();
   //    printf("Hello world: thread %d of %d checking in. \n", thread_id, nthreads);
   // }

   // insert your dgemv code here. you may need to create additional parallel regions,
   // and you may want to comment out the above parallel code block that prints out
   // nthreads and thread_id so as to not taint your timings
   
   // Record the time in high precision
   std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
   // Use OpenMP to parallel for calculating each row i of matrix A at the same time
   // Tried #pragma omp parallel for collapse(2) as well but the performance didn't improve that much
   #pragma omp parallel for
   for (off_t i = 0; i < n; i++) {
      for (off_t j = 0; j < n; j++) {
         y[i] += A[i*n+j] * x[j];
      }
   }
   std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = end_time - start_time;
   std::cout << " Elapsed time is : " << std::fixed << std::setprecision(9) << elapsed.count() << " " << std::endl;
}

