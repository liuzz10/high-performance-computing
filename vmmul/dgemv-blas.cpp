#include <cblas.h>
#include <iostream>
#include <chrono>
#include <iomanip>

const char* dgemv_desc = "Reference dgemv.";

/*
 * This routine performs a dgemv operation
 * Y :=  A * X + Y
 * where A is lda-by-lda matrix stored in row-major format, and X and Y are lda by 1 vectors.
 * On exit, A and X maintain their input values.
 * This function wraps a call to the BLAS-2 routine DGEMV
 */
void my_dgemv(int n, double* A, double* x, double* y) {
   // Set up args
   double alpha=1.0, beta=1.0;
   int lda=n, incx=1, incy=1;
   
   // Performs dgemv and record the time in high precision
   std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
   cblas_dgemv(CblasRowMajor, CblasNoTrans, n, n, alpha, A, lda, x, incx, beta, y, incy);
   std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsed = end_time - start_time;
   std::cout << " Elapsed time is : " << std::fixed << std::setprecision(9) << elapsed.count() << " " << std::endl;
}
