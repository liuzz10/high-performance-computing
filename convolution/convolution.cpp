#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

#define INPUT_CHANNEL 3
#define OUTPUT_CHANNEL 1
#define FILTER_DIMENSION 3
#define TOTAL_FILTER 10

void fill_random(float *array, int size) {
    for (int i=0; i < size; i++) {
        array[i] = float(rand()) / float((RAND_MAX));
    }
}

void print(float *data, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << data[i * width + j] << " ";
        }
        std::cout << std::endl;
    }
}

float convolve_pixel(float *s, int i, int j , int channel_dimension, float *filter, int channel_count, int filter_count)
{
   float res = 0.0;
   int filter_start = filter_count*FILTER_DIMENSION*FILTER_DIMENSION*INPUT_CHANNEL + channel_count*FILTER_DIMENSION*FILTER_DIMENSION;
   for (int x_offset = 0; x_offset < FILTER_DIMENSION; x_offset++) {
      for (int y_offset = 0; y_offset < FILTER_DIMENSION; y_offset++) {
        int new_i = i+x_offset - 1;
        int new_j = j+y_offset - 1;
        int filter_off = x_offset * FILTER_DIMENSION + y_offset;
        if ((new_i < 0) || (new_i > (channel_dimension-1)) || (new_j < 0) || (new_j > (channel_dimension-1))) {
            continue;
        } else {
            res += s[(channel_count*channel_dimension + new_i)*channel_dimension+new_j] * filter[filter_start + filter_off];
        }
      }
   }
   return res;
}

void basic_convolution(
    float *in_data, 
    float *out_data, 
    float *filter, 
    int channel_dimension) {
        for (int filter_count = 0; filter_count < TOTAL_FILTER; filter_count++) {
            for (int channel_count = 0; channel_count < INPUT_CHANNEL; channel_count++) {
                for (int i = 0; i < channel_dimension; i++) {
                    for (int j = 0; j < channel_dimension; j++) {
                        out_data[filter_count * channel_dimension * channel_dimension + (channel_dimension * i + j)] += convolve_pixel(
                                in_data, i, j, 
                                channel_dimension, 
                                filter, 
                                channel_count,
                                filter_count
                            );
                    }
                }
            }
        }
}

void im2col(
    float *in_data, 
    float *im2col_data, 
    float *filter, 
    int channel_dimension) {
        int n_patch = channel_dimension * channel_dimension;
        // im2col: convert input data to col data
        for (int channel_count = 0; channel_count < INPUT_CHANNEL; channel_count++) {
            int im2col_start_i = channel_count * FILTER_DIMENSION * FILTER_DIMENSION;
            int in_start_i = channel_count * channel_dimension;
            for (int i = 0; i < channel_dimension; i++) {
                for (int j = 0; j < channel_dimension; j++) {
                    int kernel_index = 0;
                    for (int x_offset = -1; x_offset < FILTER_DIMENSION - 1; x_offset++) {
                        for (int y_offset = -1; y_offset < FILTER_DIMENSION - 1; y_offset++) {
                            int new_i = i + x_offset;
                            int new_j = j + y_offset;
                            int im2col_i = im2col_start_i + kernel_index;
                            int im2col_j = i*channel_dimension + j;
                            if (new_i < 0 || new_i >= channel_dimension || new_j < 0 || new_j >= channel_dimension) {
                                im2col_data[im2col_i * n_patch + im2col_j] = 0.0;
                            } else {
                                im2col_data[im2col_i * n_patch + im2col_j] = in_data[(in_start_i + new_i)*channel_dimension+new_j];
                            }
                            kernel_index++;
                        }
                    }
                }
            }
        }
    }

void im2col_optimized_locality(
    float *in_data, 
    float *im2col_data, 
    float *filter, 
    int channel_dimension) {
        int cols = channel_dimension * channel_dimension;
        int rows = FILTER_DIMENSION * FILTER_DIMENSION;
        // im2col: convert input data to col data
        for (int channel_count = 0; channel_count < INPUT_CHANNEL; channel_count++) {
            int im2col_start_i = channel_count * FILTER_DIMENSION * FILTER_DIMENSION;
            int in_start_i = channel_count * channel_dimension;
            for (int i = 0; i < rows; i++) {
                int start_i = (int)i / FILTER_DIMENSION;
                int start_j = i % FILTER_DIMENSION;
                int j = 0;
                while (j < cols) {
                    int new_i = start_i + (int)j / channel_dimension - 1;
                    int new_j = start_j + j % channel_dimension - 1;
                    if (new_i < 0 || new_i >= channel_dimension || new_j < 0 || new_j >= channel_dimension) {
                        im2col_data[(im2col_start_i+i)*cols+j] = 0.0;
                    } else {
                        im2col_data[(im2col_start_i+i)*cols+j] = in_data[(in_start_i+new_i)*channel_dimension+new_j];
                    }
                    j++;
                }
            }
        }
    }

void dgemv(int rows, int cols, float* M, float* filter, float* out) {
// This routine performs a dgemv operation
// out :=  filter * M
// rows: K^2*C
// cols: H*W
// M: (K^2*C) * (H*W)
// filter: (N) * (K^2*C)
// out = N * (H*W)
   for (int i = 0; i < TOTAL_FILTER; i++) {
       for (int j = 0; j < cols; j++) {
           for (int k = 0; k < rows; k++) {
               out[i*cols+j] += 
               filter[i*FILTER_DIMENSION*FILTER_DIMENSION*INPUT_CHANNEL+k]
                * M[k*cols+j];
           }
       }
   }
}

void dgemv_omp(int rows, int cols, float* M, float* filter, float* out) {
// This routine performs a dgemv operation using OMP to parallel for calculating each row i of matrix M at the same time
// out :=  filter * M
// rows: K^2*C
// cols: H*W
// M: (K^2*C) * (H*W)
// filter: (N) * (K^2*C)
// out = N * (H*W)
   #pragma omp parallel for collapse(2)
   for (int i = 0; i < TOTAL_FILTER; i++) {
       for (int j = 0; j < cols; j++) {
           float temp_out = 0.0;
           for (int k = 0; k < rows; k++) {
               temp_out += 
               filter[i*FILTER_DIMENSION*FILTER_DIMENSION*INPUT_CHANNEL+k]
                * M[k*cols+j];
           }
           out[i*cols+j] = temp_out;
       }
   }
}

void im2col_convolution(
    float *in_data, 
    float *out_data, 
    float *filter, 
    int channel_dimension) {
        int n_patch = channel_dimension * channel_dimension;
        int n_rows = FILTER_DIMENSION * FILTER_DIMENSION * INPUT_CHANNEL;
        float *im2col_data = (float *)malloc(sizeof(float) * n_rows * n_patch);
        // std::cout << "input data" << std::endl;
        // print(in_data, channel_dimension*INPUT_CHANNEL, channel_dimension);
        // Convert image to column
        im2col(in_data, im2col_data, filter, channel_dimension);
        // std::cout << "im2col data" << std::endl;
        // print(im2col_data, n_rows, n_patch);

        // Vector matrix multiplication
        dgemv(
            INPUT_CHANNEL*FILTER_DIMENSION*FILTER_DIMENSION,
            n_patch,
            im2col_data,
            filter,
            out_data
        );
}

void im2col_convolution_optimized(
    float *in_data, 
    float *out_data, 
    float *filter, 
    int channel_dimension) {
        int n_patch = channel_dimension * channel_dimension;
        int n_rows = FILTER_DIMENSION * FILTER_DIMENSION * INPUT_CHANNEL;
        float *im2col_data = (float *)malloc(sizeof(float) * n_rows * n_patch);
        // std::cout << "input data" << std::endl;
        // print(in_data, channel_dimension*INPUT_CHANNEL, channel_dimension);
        // Convert image to column
        im2col_optimized_locality(in_data, im2col_data, filter, channel_dimension);
        // std::cout << "im2col data" << std::endl;
        // print(im2col_data, n_rows, n_patch);

        // Vector matrix multiplication
        dgemv(
            INPUT_CHANNEL*FILTER_DIMENSION*FILTER_DIMENSION,
            n_patch,
            im2col_data,
            filter,
            out_data
        );
}

void im2col_omp_convolution(
    float *in_data, 
    float *out_data, 
    float *filter, 
    int channel_dimension) {
        int n_patch = channel_dimension * channel_dimension;
        int n_rows = FILTER_DIMENSION * FILTER_DIMENSION * INPUT_CHANNEL;
        float *im2col_data = (float *)malloc(sizeof(float) * n_rows * n_patch);
        // std::cout << "input data" << std::endl;
        // print(in_data, channel_dimension*INPUT_CHANNEL, channel_dimension);
        // Convert image to column
        im2col_optimized_locality(in_data, im2col_data, filter, channel_dimension);
        // std::cout << "im2col data" << std::endl;
        // print(im2col_data, n_rows, n_patch);

        // Vector matrix multiplication
        dgemv_omp(
            INPUT_CHANNEL*FILTER_DIMENSION*FILTER_DIMENSION,
            n_patch,
            im2col_data,
            filter,
            out_data
        );
}

typedef void (*conv_fn_type)(
    float *in_data, 
    float *out_data, 
    float *filter,
    int channel_dimension);

void use_convolution(conv_fn_type conv_function, float *in_data, float *out_data, float *filter, int channel_dimension) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
    conv_function(in_data, out_data, filter, channel_dimension);
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Elapsed time is : " << elapsed.count() << " " << std::endl;
    // std::cout << "Each input matrix is of length:" << channel_dimension << std::endl;
    // std::cout << "Output matrix is" << std::endl;
    // print(out_data, TOTAL_FILTER, channel_dimension*channel_dimension*OUTPUT_CHANNEL);
}

// Input: number of channels, dimension of the filter squre
int
main (int ac, char *av[])
{
    int channel_dimension = atoi(av[1]);
    off_t channel_nvalues = channel_dimension * channel_dimension;
    off_t filter_nvalues = FILTER_DIMENSION * FILTER_DIMENSION;
    float *in_data = (float *)malloc(sizeof(float) * channel_nvalues * INPUT_CHANNEL);
    float *out_data1 = (float *)malloc(sizeof(float) * channel_nvalues * OUTPUT_CHANNEL * TOTAL_FILTER);
    float *out_data2 = (float *)malloc(sizeof(float) * channel_nvalues * OUTPUT_CHANNEL * TOTAL_FILTER);
    float *out_data3 = (float *)malloc(sizeof(float) * channel_nvalues * OUTPUT_CHANNEL * TOTAL_FILTER);
    float *out_data4 = (float *)malloc(sizeof(float) * channel_nvalues * OUTPUT_CHANNEL * TOTAL_FILTER);
    float *filter = (float *)malloc(sizeof(float) * filter_nvalues * INPUT_CHANNEL * OUTPUT_CHANNEL * TOTAL_FILTER);
    
    fill_random(in_data, channel_nvalues * INPUT_CHANNEL);
    fill_random(filter, filter_nvalues * INPUT_CHANNEL * OUTPUT_CHANNEL * TOTAL_FILTER);

    std::cout << "[Basic version]" << std::endl;
    use_convolution(basic_convolution, in_data, out_data1, filter, channel_dimension);
    std::cout << "[im2col version]" << std::endl;
    use_convolution(im2col_convolution, in_data, out_data2, filter, channel_dimension);
    std::cout << "[im2col_optimized version]" << std::endl;
    use_convolution(im2col_convolution_optimized, in_data, out_data3, filter, channel_dimension);
    std::cout << "[im2col+omp version]" << std::endl;
    use_convolution(im2col_omp_convolution, in_data, out_data4, filter, channel_dimension);
    free(in_data);
    free(out_data1);
    free(out_data2);
    free(out_data3);
    free(out_data4);
}

