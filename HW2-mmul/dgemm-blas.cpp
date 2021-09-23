#include <cblas.h>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <iomanip>

const char* dgemm_desc = "Reference dgemm.";

/*
 * This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values.
 * This function wraps a call to the BLAS-3 routine DGEMM,
 * via the standard FORTRAN interface - hence the reference semantics.
 */
void square_dgemm(int n, double* A, double* B, double* C) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1., A, n, B, n, 1., C, n);
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << " Problem size is : " << n << " " << std::endl;
    std::cout << " Elapsed time is : " << std::fixed << std::setprecision(9) << elapsed.count() << " " << std::endl;
}
