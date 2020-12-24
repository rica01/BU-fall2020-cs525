#include "convolution.h"

void f(int x)
{
    printf("%d\n%", x);
    return;
}

image *extend_edges(image *img, int extend_amount)
{

    int new_height, new_width;
    new_height = img->height + 2 * extend_amount;
    new_width = img->width + 2 * extend_amount;

    // image *img_ext = copy_image(img);

    // img_ext->height = new_height;
    // img_ext->width = new_width;

    image *img_ext = create_image(0x0, new_height, new_width);

    int px;

    // copy original
    for (int r = 0; r < img->height; r++)
    {
        for (int c = 0; c < img->width; c++)
        {
            px = img->pixels[r * img->width + c];
            // get_pixel(img, r, c);

            set_pixel(img_ext, r + extend_amount, c + extend_amount, px);
        }
    }
    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // fill top left
    // px = get_pixel(img, 0, 0);
    px = img->pixels[0 * img->width + 0];

    for (int r = 0; r < extend_amount + 1; r++)
    {
        for (int c = 0; c < extend_amount + 1; c++)
        {
            set_pixel(img_ext, r, c, px);
        }
    }
    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // fill top right
    // px = get_pixel(img, 0, img->width - 1);
    px = img->pixels[0 * img->width + img->width - 1];

    for (int r = 0; r < extend_amount + 1; r++)
    {
        for (int c = img->width + extend_amount - 1; c < new_width + 1; c++)
        {
            set_pixel(img_ext, r, c, px);
        }
    }
    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // fill bottom left
    // px = get_pixel(img, img->height - 1, 0);
    px = img->pixels[(img->height - 1) * img->width + 0];

    for (int r = img->height + extend_amount - 1; r < new_height; r++)
    {
        for (int c = 0; c < extend_amount + 1; c++)
        {
            set_pixel(img_ext, r, c, px);
        }
    }
    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // fill bottom right
    // px = get_pixel(img, img->height - 1, img->width - 1);
    px = img->pixels[(img->height - 1) * img->width + (img->width - 1)];
    for (int r = img->height + extend_amount - 1; r < new_height; r++)
    {
        for (int c = img->width + extend_amount - 1; c < new_width; c++)
        {
            set_pixel(img_ext, r, c, px);
        }
    }
    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // top
    for (int r = extend_amount - 1; r >= 0; r--)
    {
        for (int c = extend_amount + 1; c <= new_width - extend_amount - 2; c++)
        {
            set_pixel(img_ext, r, c,
                      px = img->pixels[(r + 1) * img->width + (c)]);

            // get_pixel(img_ext, r + 1, c));
        }
    }

    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // bottom
    for (int r = new_height - extend_amount; r < new_height; r++)
    {
        for (int c = extend_amount + 1; c <= new_width - extend_amount - 2; c++)
        {

            set_pixel(img_ext, r, c,
                      px = img->pixels[(r - 1) * img->width + (c)]);
            // get_pixel(img_ext, r - 1, c));
        }
    }
    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // right
    for (int r = extend_amount + 1; r <= new_height - extend_amount - 2; r++)
    {
        for (int c = new_width - extend_amount; c < new_width; c++)
        {
            set_pixel(img_ext, r, c,
                      px = img->pixels[(r)*img->width + (c - 1)]);
            // get_pixel(img_ext, r, c - 1));
        }
    }
    // print_2d_matrix(img_ext, new_height, new_width);
    // printf("============================\n\n");

    // left
    for (int r = extend_amount + 1; r <= new_height - extend_amount - 2; r++)
    {
        for (int c = extend_amount - 1; c >= 0; c--)
        {

            set_pixel(img_ext, r, c,
                      px = img->pixels[(r)*img->width + (c + 1)]);
            // get_pixel(img_ext, r, c + 1));
        }
    }

    delete_image(img);
    return img_ext;
}

// Calculates the normalize index by simply summing up all elements in the
// kernel. If sum is 0, return 1. Otherwise, return the sum.
int kernel_sum(image *k)
{
    int sum = 0;
    for (int r = 0; r < k->height; r++)
        for (int c = 0; c < k->width; c++)
        {
            sum += k->pixels[(r)*k->width + (c)];
            // sum += get_pixel(k, r, c);
        }
    if (sum == 0)
        return 1;
    else

        return sum;
}

// Divides the each pixel value by normalize_amount in 2d array **img to
// normalize pixel values. Integer division is OK.
void normalize_output(image *img, int normalize_amount)
{
    for (int r = 0; r < img->height; r++)
    {
        for (int c = 0; c < img->width; c++)
        {
            set_pixel(img, r, c,
                      img->pixels[r * img->width + c] / normalize_amount);
            //   get_pixel(img, r, c) / normalize_amount);
        }
    }
    return;
}

// Core multiply-accumulate operation at given location of the image
int pixel_operation(image *k, image *img, int row_index, int col_index)
{

    int sum = 0;
    int k_offset = k->height / 2;

    for (int r = row_index - k_offset,
             k_r = 0;
         k_r < k->height;
         r++, k_r++)
    {
        for (int c = col_index - k_offset,
                 k_c = 0;
             k_c < k->width;
             c++, k_c++)
        {
            // int img_px = get_pixel(img, r, c);
            int img_px = img->pixels[(r)*img->width + (c)];

            // int k_px = get_pixel(k, k_r, k_c);
            // int k_px = img->pixels[(k_r)*img->width + (k_c)];
            int k_px = k->pixels[(k_r)*k->width + (k_c)];

            // printf("\t\t[img@%d,%d]: %d * [k@%d,%d]: %d\n", r, c, img_px, k_r, k_c, k_px);
            sum += img_px * k_px;
        }
    }

    return sum;
}

// Convolution method that convolves the image with given kernel. This
// method expected to call all other methods above.
image *convolve_image(image *k, image *img)
{

    int kernel_norm = kernel_sum(k);
    img = extend_edges(img, k->width / 2);

    // print_image(k);
    // printf("==========================\n");
    // print_image(img);
    // printf("==========================\n");

    image *new_img = create_image(0x0, img->height, img->width);

    for (int r = k->height; r < img->height - k->height; r++)
    {
        for (int c = k->width; c < img->width - k->width; c++)
        {
            int new_px = pixel_operation(k, img, r, c);
            set_pixel(new_img, r, c, new_px);
        }
        // print_image(new_img);
        // printf("==========================\n");
    }

    normalize_output(img, kernel_norm);
    return new_img;
}