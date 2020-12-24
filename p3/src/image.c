#include "./image.h"

// int array[width * height];

// int SetElement(int row, int col, int value)
// {
// array[width * row + col] = value;
// }

int get_pixel(image *img,  int r,  int c)
{
    return img->pixels[r * img->width + c];
}

void set_pixel(image *img,  int r,  int c,  int pix)
{
    img->pixels[img->width * r + c] = pix;
    return;
}

image *copy_image(image *img)
{
    image *cpy = (image *)malloc(sizeof(image));
    cpy->width = img->width;
    cpy->height = img->height;
    if (img->pixels)
    {
        cpy->pixels = (int *)malloc(sizeof(int) * cpy->width * cpy->height);

        for (int r = 0; r < cpy->height; r++)
            for (int c = 0; c < cpy->width; c++)
                set_pixel(
                    cpy, r, c,
                    get_pixel(img, r, c));
    }
    else
    {
        cpy->pixels = 0x0;
    }
    return cpy;
}

image *create_image(int **data, int rows, int cols)
{
    image *img = (image *)malloc(sizeof(image));
    img->width = cols;
    img->height = rows;
    int size = rows * cols * sizeof(int);
    // printf(">>> %p :: %d\n", img, size);
    (img->pixels) = (int *)malloc(size);

    if (data)
    {

        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                set_pixel(img, r, c, data[r][c]);
    }
    else
    {
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                set_pixel(img, r, c, 1);
    }
    return img;
}

void delete_image(image *img)
{
    free(img->pixels);
    free(img);
    return;
}

void print_image(image *img)
{
    for (int r = 0; r < img->height; r++)
    {
        for (int c = 0; c < img->width; c++)
        {
            printf("%d\t", get_pixel(img, r, c));
        }
        printf("\n\n");
    }
}

void save_image(image *img, char *filename)
{
    FILE *f = fopen(filename, "w");

    fprintf(f, "%d\n%d\n", img->height, img->width);
    for (int r = 0; r < img->height; r++)
    {
        for (int c = 0; c < img->width; c++)
        {
            fprintf(f, "%d ", get_pixel(img, r, c));
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return;
}