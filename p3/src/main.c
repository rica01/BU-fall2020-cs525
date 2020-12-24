#include <stdio.h>
#include <time.h>
#include "util.h"
#include "image.h"
#include "convolution.h"
int main(int argc, char *argv[])
{

    printf("w0la\n");

    int num_rows, num_cols;
    int **_i = read_pgm_file(argv[1], &num_rows, &num_cols);
    image *img = create_image(_i, num_rows, num_cols);
    dealloc_2d_matrix(_i, num_rows, num_cols);

    int k_row, k_col;
    int **_k = read_pgm_file(argv[2], &k_row, &k_col);
    image *k = create_image(_k, k_row, k_col);
    dealloc_2d_matrix(_k, k_row, k_col);

    clock_t s, e;
    s = clock();
    
    img = convolve_image(k, img);

    e = clock();
    double time_taken = ((double)(e-s)) / CLOCKS_PER_SEC; // in seconds
    printf("%f\n", time_taken);

    save_image(img, argv[3]);

    delete_image(img);
    delete_image(k);

    printf("bai!\n");
    return 0;
}
