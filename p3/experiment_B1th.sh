#!/bin/bash

OMP_NUM_THREADS=1 ./convolution_omp ./images/1000x1000 ./kernels/k0.k ./out/1000x1000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/2000x2000 ./kernels/k0.k ./out/2000x2000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/3000x3000 ./kernels/k0.k ./out/3000x3000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/4000x4000 ./kernels/k0.k ./out/4000x4000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/5000x5000 ./kernels/k0.k ./out/5000x5000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/6000x6000 ./kernels/k0.k ./out/6000x6000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/7000x7000 ./kernels/k0.k ./out/7000x7000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/8000x8000 ./kernels/k0.k ./out/8000x8000.txt
OMP_NUM_THREADS=1 ./convolution_omp ./images/9000x9000 ./kernels/k0.k ./out/9000x9000.txt
