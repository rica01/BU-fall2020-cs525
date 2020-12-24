// ********************
// Ricardo Roman-Brenes
// 22001125
//
// oct-2020
// Bilkent University
// CS525
// *********************

#include "./headers.h"

int find_range(int *data, int elements)
{
    int min, max;
    max = INT_MIN;
    min = INT_MAX;
    
    for (int i = 0; i < elements; i++)
    {
        if (data[i] < min)
            min = data[i];
        if (data[i] > max)
            max = data[i];
    }

    printf("%d\n", max - min);

    return (max - min);
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        int elements = get_file_lines(argv[1]);
        int *data = (int *)malloc(sizeof(int) * elements);
        data = get_file_data(argv[1], elements, data);
        find_range(data, elements);
        return 0;
    }
    else
    {
        printf("Missing file argument.\nUSAGE: range-serial [FILE]");
        return 1;
    }
}
