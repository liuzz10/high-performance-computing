cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CMAKE_BUILD_TYPE "Release")

add_executable(convolution_basic convolution_basic.cpp)
target_compile_options(convolution_basic PRIVATE -fopenmp -march=native)
target_link_options(convolution_basic PRIVATE -fopenmp)
