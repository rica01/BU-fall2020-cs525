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

    // printf("%d\n", max - min);

    return (max - min);
}

void sort_aggregate(int *index, int *values, int elements, int range, int min, char *filename)
{

    bucket_list_item buckets[range + 1];
    for (int i = 0; i < range + 1; i++)
    {
        buckets[i].id = min + i;
        buckets[i].items = 0x0;
    }

    for (int i = 0; i < elements; i++)
    {
        item *val = malloc(sizeof(item));
        val->value = values[i];
        val->next = buckets[index[i] - min].items;
        buckets[index[i] - min].items = val;
    }

    // printf("elements %d; range %d\n", elements, range);
    for (int i = 0; i < range + 1; i++)
    {
        // printf("bucket id: %d => values:\n\t", buckets[i].id);
        item *tmp = buckets[i].items;
        while (tmp != 0x0)
        {
            // printf("%d  ", tmp->value);
            tmp = tmp->next;
        }
        // printf("\n\n");
    }

    for (int i = 0; i < range + 1; i++)
    {
        int acc = 0;
        item *tmp = buckets[i].items;
        item *to_delete = 0x0;
        while (tmp != 0x0)
        {
            acc += tmp->value;
            to_delete = tmp;
            tmp = tmp->next;
            free(to_delete);
        }

        buckets[i].items = malloc(sizeof(item));
        buckets[i].items->value = acc;
        buckets[i].items->next = 0x0;
    }
    FILE *output = fopen(filename, "w");

    // printf("=============================\n", elements, range);
    for (int i = 0; i < range + 1; i++)
    {
        // printf("bucket id: %d => values:\n\t", buckets[i].id);
        item *tmp = buckets[i].items;
        while (tmp != 0x0)
        {
            printf("%d\n", tmp->value);
            fprintf(output, "%d\n", tmp->value);
            tmp = tmp->next;
            
        }
        // printf("\n\n");
    }

    fclose(output);

    return;
}

int main(int argc, char **argv)
{

    int elements = get_file_lines(argv[1]);
    int index[elements];
    int values[elements];

    FILE *input = fopen(argv[1], "r");
    // printf("%d\n", elements);
    for (int i = 0; i < elements; i++)
    {
        fscanf(input, "%d %d", &index[i], &values[i]);
        // printf("%d:%d\n", index[i], values[i]);
    }

    int range = find_range(index, elements);
    int min = INT_MAX;
    int max = INT_MIN;
    get_min_max(index, elements, &min, &max);

    // printf("range: %d; generating %d buckets.\n", range, range + 1);
    sort_aggregate(index, values, elements, range, min, argv[2]);

    return 0;
}