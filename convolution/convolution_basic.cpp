#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define TOTAL_CHANNEL 3
#define FILTER_DIMENSION 3

void fill_random(float *array, int size) {
    for (int i=0; i < size; i++) {
        array[i] = float(rand()) / float((RAND_MAX));
    }
}

float convolve_pixel(float *s, int i, int j , int channel_dimension, float *filter, int filter_start)
{
    if (i == 0 || i >= channel_dimension - 1 || j == 0 || j >= channel_dimension - 1) {
        return 0.0;
    }
   float res = 0.0;
   for (int x_offset = -1; x_offset < 2; x_offset++) {
      for (int y_offset = -1; y_offset < 2; y_offset++) {
        res += s[(i+x_offset)*channel_dimension+(j+y_offset)] * filter[filter_start];
        filter_start++;
      }
   }
   return res;
}

void do_convolution(
    float *in_data, 
    float *out_data, 
    float *filter, 
    int channel_dimension,
    off_t channel_nvalues) {
        for (int channel_count = 0; channel_count < TOTAL_CHANNEL; channel_count++) {
            for (int i = 0; i < channel_dimension; i++) {
                for (int j = 0; j < channel_dimension; j++) {
                    out_data[channel_dimension * i + j] += convolve_pixel(
                            in_data, i, j, 
                            channel_dimension, 
                            filter, 
                            channel_count*FILTER_DIMENSION*FILTER_DIMENSION 
                        );
                }
            }
        }

}

// Input: number of channels, dimension of the filter squre
int
main (int ac, char *av[])
{
    int channel_dimension = (int)av[1];
    int filter_dimension = (int)av[2];
    off_t channel_nvalues = channel_dimension * channel_dimension;
    off_t filter_nvalues = FILTER_DIMENSION * FILTER_DIMENSION;
    float *in_data = (float *)malloc(sizeof(float) * channel_nvalues * TOTAL_CHANNEL);
    float *out_data = (float *)malloc(sizeof(float) * channel_nvalues * TOTAL_CHANNEL);
    float *filter = (float *)malloc(sizeof(float) * filter_nvalues * TOTAL_CHANNEL);
    fill_random(in_data, channel_nvalues * TOTAL_CHANNEL);
    fill_random(filter, filter_nvalues * TOTAL_CHANNEL);

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

    do_convolution(in_data, out_data, filter, channel_dimension, channel_nvalues);

    std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << " Elapsed time is : " << elapsed.count() << " " << std::endl;

}

