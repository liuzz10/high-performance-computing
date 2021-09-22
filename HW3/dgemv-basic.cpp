#include <iostream>
#include <chrono>
#include <iomanip>

const char* dgemv_desc = "Basic implementation of matrix-vector multiply.";

/*
 * This routine performs a dgemv operation
 * Y :=  A * X + Y
 * where A is n-by-n matrix stored in row-major format, and X and Y are n by 1 vectors.
 * On exit, A and X maintain their input values.
 */
void my_dgemv(int n, double* A, double* x, double* y) {
   std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
   for (off_t i = 0; i < n; i++) {
      for (off_t j = 0; j < n; j++) {
         y[i] += A[i*n+j] * x[j];
      }
   }
   std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = end_time - start_time;
   std::cout << " Elapsed time is : " << std::fixed << std::setprecision(9) << elapsed.count() << " " << std::endl;
}
