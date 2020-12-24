#include "./util.h"

#ifndef IMAGE_H
#define IMAGE_H

struct image
{

    int width;
    int height;
    int *pixels;
};
typedef struct image image;

int get_pixel(image *img, int w, int h);
void set_pixel(image *img, int w, int h, int pix);
image *create_image(int **data, int rows, int cols);
void delete_image(image *img);
void save_image(image *img, char *filename);
void print_image(image *img);
image *copy_image(image *img);

#endif