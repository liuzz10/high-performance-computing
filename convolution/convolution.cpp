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

void fill_random(float *array, int size) {
    for (int i=0; i < size; i++) {
        array[i] = float(rand()) / float((RAND_MAX));
    }
}

float convolve_pixel(float *s, int i, int j , int channel_dimension, float *filter, int filter_start)
{
   float res = 0.0;
   for (int x_offset = -1; x_offset < 2; x_offset++) {
      for (int y_offset = -1; y_offset < 2; y_offset++) {
        if (i+x_offset < 0 || i+x_offset > channel_dimension-1 || j+y_offset < 0 || j+y_offset > channel_dimension-1) {
            filter_start++;
            continue;
        } else {
            res += s[(i+x_offset)*channel_dimension+(j+y_offset)] * filter[filter_start];
            filter_start++;
        }
      }
   }
   return res;
}

void basic_convolution(
    float *in_data, 
    float *out_data, 
    float *filter, 
    int channel_dimension,
    off_t channel_nvalues) {
        for (int channel_count = 0; channel_count < OUTPUT_CHANNEL; channel_count++) {
            for (int i = 0; i < channel_dimension; i++) {
                for (int j = 0; j < channel_dimension; j++) {
                    out_data[channel_dimension * i + j] += convolve_pixel(
                            in_data, channel_count*channel_dimension+i, j, 
                            channel_dimension, 
                            filter, 
                            channel_count*FILTER_DIMENSION*FILTER_DIMENSION 
                        );
                }
            }
        }

}

void im2col(
    float *in_data, 
    float *im2col_data, 
    float *filter, 
    int channel_dimension,
    off_t channel_nvalues) {
        int n_patch = channel_dimension * channel_dimension;
        int channel_size = FILTER_DIMENSION * FILTER_DIMENSION * n_patch;
        // im2col: convert input data to col data
        for (int channel_count = 0; channel_count < OUTPUT_CHANNEL; channel_count++) {
            for (int i = 0; i < channel_dimension; i++) {
                for (int j = 0; j < channel_dimension; j++) {
                    for (int x_offset = -1; x_offset < 2; x_offset++) {
                        for (int y_offset = -1; y_offset < 2; y_offset++) {
                            int new_i = i + x_offset;
                            int new_j = j + y_offset;
                            int im2col_i = (x_offset+1)*FILTER_DIMENSION+(y_offset+1);
                            int im2col_j = i*channel_dimension + j;
                            if (new_i == -1 || new_i == channel_dimension || new_j == -1 || new_j == channel_dimension) {
                                im2col_data[channel_count * channel_size + (im2col_i * n_patch) + im2col_j] 
                                = 0.0;
                            } else {
                                im2col_data[channel_count * channel_size + (im2col_i * n_patch) + im2col_j] 
                                = in_data[new_i*channel_dimension+new_j];
                            }
                        }
                    }
                }
            }
        }
}

void dgemv(int rows, int cols, float* M, float* filter, float* out) {
// This routine performs a dgemv operation
// out :=  filter * M
   for (int j = 0; j < cols; j++) {
      for (int i = 0; i < rows; i++) {
        out[j] += M[i * cols + j] * filter[i];
      }
   }
}

void im2col_convolution(
    float *in_data, 
    float *out_data, 
    float *filter, 
    int channel_dimension,
    off_t channel_nvalues) {
        int n_patch = channel_dimension * channel_dimension;
        int n_cols = FILTER_DIMENSION * FILTER_DIMENSION * INPUT_CHANNEL;
        float *im2col_data = (float *)malloc(sizeof(float) * n_cols * n_patch);
        // Convert image to column
        im2col(in_data, im2col_data, filter, channel_dimension, channel_nvalues);

        // Vector matrix multiplication
        dgemv(
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
    int channel_dimension,
    off_t channel_nvalues);

void use_convolution(conv_fn_type conv_function, float *in_data, float *out_data, float *filter, int channel_dimension, off_t channel_nvalues) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
    conv_function(in_data, out_data, filter, channel_dimension, channel_nvalues);
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Elapsed time is : " << elapsed.count() << " " << std::endl;
    std::cout << "Each input matrix is of length:" << channel_nvalues << std::endl;
    std::cout << "Output matrix is" << std::endl;
    for (int i=0; i<channel_nvalues*INPUT_CHANNEL; i++) {
      std::cout << out_data[i] << std::endl;
    }
}

// Input: number of channels, dimension of the filter squre
int
main (int ac, char *av[])
{
    int channel_dimension = atoi(av[1]);
    off_t channel_nvalues = channel_dimension * channel_dimension;
    off_t filter_nvalues = FILTER_DIMENSION * FILTER_DIMENSION;
    float *in_data = (float *)malloc(sizeof(float) * channel_nvalues * INPUT_CHANNEL);
    float *out_data1 = (float *)malloc(sizeof(float) * channel_nvalues * OUTPUT_CHANNEL);
    float *out_data2 = (float *)malloc(sizeof(float) * channel_nvalues * OUTPUT_CHANNEL);
    float *filter = (float *)malloc(sizeof(float) * filter_nvalues * INPUT_CHANNEL * OUTPUT_CHANNEL);
    
    fill_random(in_data, channel_nvalues * INPUT_CHANNEL);
    fill_random(filter, filter_nvalues * INPUT_CHANNEL * OUTPUT_CHANNEL);

    use_convolution(basic_convolution, in_data, out_data1, filter, channel_dimension, channel_nvalues);
    use_convolution(im2col_convolution, in_data, out_data2, filter, channel_dimension, channel_nvalues);
}

