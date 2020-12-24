#include "./image.h"
#include <stdbool.h>
#include <omp.h>
#ifndef CONV_H
#define CONV_H

// void normalize_output(int **img,int normalize_amount, int num_rows, int num_cols, int **output_img);
void normalize_output(image *img, int normalize_amount);

// image* extend_edges(int **img, int row_index, int col_index, int extend_amount);
image *extend_edges(image *img, int extend_amount);

// int kernel_sum(int **kernel, int kernel_size); //If sum is 0, return 1. Otherwise, return the sum.
int kernel_sum(image *kernel);

// int pixel_operation(int **kernel, int kernel_size, int **img, int row_index, int col_index);
int pixel_operation(image *k, image *img, int row_index, int col_index);

// void convolve_image(int **kernel, int kernel_size, int **img, int num_rows, int num_cols, int **output_img);
image *convolve_image(image *k, image *img);
#endif