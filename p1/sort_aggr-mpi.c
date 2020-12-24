// ********************
// Ricardo Roman-Brenes
// 22001125
//
// oct-2020
// Bilkent University
// CS525
// *********************

#include "./headers.h"
#include "mpi.h"

#define DEBUG 1

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

int main(int argc, char **argv)
{

    // MPI setup //
    MPI_Init(&argc, &argv);
    int num_process, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_length;
    MPI_Get_processor_name(processor_name, &name_length);
    // MPI setup //

    int elements, range, min, max;
    char c[6] = {rank + '0', '.', 'l', 'o', 'g', '\0'};
    FILE *log = fopen(c, "w");

    if (rank == 0)
    {
        elements = get_file_lines(argv[1]);
    }

    MPI_Bcast(&elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *index;
    int *values;
    index = malloc(sizeof(int) * elements);
    values = malloc(sizeof(int) * elements);

    if (rank == 0)
    {

        FILE *input = fopen(argv[1], "r");
        DEBUG &&fprintf(log, "%s[%s] - (%d|%d) >> %d elements\n\n", get_current_time(), processor_name, rank, num_process, elements);

        for (int i = 0; i < elements; i++)
        {
            fscanf(input, "%d %d", &(index[i]), &(values[i]));
        }

        range = find_range(index, elements);
        min = INT_MAX;
        max = INT_MIN;
        get_min_max(index, elements, &min, &max);
    }

    MPI_Bcast(&min, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&range, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *index_recv;
    int *values_recv;
    int count_recv;

    if (rank == 0)
    {
        int counts_send[num_process];
        int displacements[num_process];

        int chunk_size = elements / (num_process);
        int remaining = elements % (num_process);

        counts_send[0] = chunk_size + remaining;
        displacements[0] = 0;
        for (int i = 1; i < num_process; i++)
        {
            counts_send[i] = chunk_size;
            displacements[i] = displacements[i - 1] + counts_send[i - 1];
        }

        // scatter amount of elements to process
        MPI_Scatter(counts_send,   //const void *buffer_send,
                    1,             //int count_send,
                    MPI_INT,       //MPI_Datatype datatype_send,
                    &count_recv,   //void *index_recv,
                    1,             //int count_recv,
                    MPI_INT,       //MPI_Datatype datatype_recv,
                    0,             //int root,
                    MPI_COMM_WORLD //MPI_Comm communicator
        );

        index_recv = malloc(sizeof(int) * count_recv);
        values_recv = malloc(sizeof(int) * count_recv);

        // scatter the elements to process
        MPI_Scatterv(index,         //const void* buffer_send,
                     counts_send,   //const int counts_send[],
                     displacements, //const int displacements[],
                     MPI_INT,       //MPI_Datatype datatype_send,
                     index_recv,    //void* index_recv,
                     count_recv,    //int count_recv,
                     MPI_INT,       //MPI_Datatype datatype_recv,
                     0,             //int root,
                     MPI_COMM_WORLD //MPI_Comm communicator);
        );

        MPI_Scatterv(values,        //const void* buffer_send,
                     counts_send,   //const int counts_send[],
                     displacements, //const int displacements[],
                     MPI_INT,       //MPI_Datatype datatype_send,
                     values_recv,   //void* index_recv,
                     count_recv,    //int count_recv,
                     MPI_INT,       //MPI_Datatype datatype_recv,
                     0,             //int root,
                     MPI_COMM_WORLD //MPI_Comm communicator);
        );
    }
    else
    {
        MPI_Scatter(0x0,           //const void *buffer_send,
                    0,             //int count_send,
                    0,             //MPI_Datatype datatype_send,
                    &count_recv,   //void *index_recv,
                    1,             //int count_recv,
                    MPI_INT,       //MPI_Datatype datatype_recv,
                    0,             //int root,
                    MPI_COMM_WORLD //MPI_Comm communicator
        );
        index_recv = malloc(sizeof(int) * count_recv);
        values_recv = malloc(sizeof(int) * count_recv);

        MPI_Scatterv(0x0,           //const void* buffer_send,
                     0,             //const int counts_send[],
                     0,             //const int displacements[],
                     MPI_INT,       //MPI_Datatype datatype_send,
                     index_recv,    //void* index_recv,
                     count_recv,    //int count_recv,
                     MPI_INT,       //MPI_Datatype datatype_recv,
                     0,             //int root,
                     MPI_COMM_WORLD //MPI_Comm communicator);
        );
        MPI_Scatterv(0x0,           //const void* buffer_send,
                     0,             //const int counts_send[],
                     0,             //const int displacements[],
                     MPI_INT,       //MPI_Datatype datatype_send,
                     values_recv,   //void* index_recv,
                     count_recv,    //int count_recv,
                     MPI_INT,       //MPI_Datatype datatype_recv,
                     0,             //int root,
                     MPI_COMM_WORLD //MPI_Comm communicator);
        );
    }

    // data distribution //

    DEBUG &&fprintf(log, "%s[%s] - (%d|%d) >> count_recv:%d\n\n", get_current_time(), processor_name, rank, num_process, count_recv);

    DEBUG &&fprintf(log, "%s[%s] - (%d|%d) >> index:value\n\n", get_current_time(), processor_name, rank, num_process);
    DEBUG &&fprintf(log, "%s[%s] - (%d|%d) >> ", get_current_time(), processor_name, rank, num_process);
    for (int i = 0; i < count_recv; i++)
    {
        DEBUG &&fprintf(log, "%d:%d  ", index_recv[i], values_recv[i]);
    }

    DEBUG &&fprintf(log, "\n\n");

    // data processing //

    bucket_list_item buckets[range + 1];
    for (int i = 0; i < range + 1; i++)
    {
        buckets[i].id = min + i;
        buckets[i].items = 0x0;
    }

    for (int i = 0; i < count_recv; i++)
    {
        item *val = malloc(sizeof(item));
        val->value = values_recv[i];
        val->next = buckets[index_recv[i] - min].items;
        buckets[index_recv[i] - min].items = val;
    }

    for (int i = 0; i < range + 1; i++)
    {

        item *tmp = buckets[i].items;
        while (tmp != 0x0)
        {

            tmp = tmp->next;
        }
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

    int buckets_to_send[range + 1];

    for (int i = 0; i < range + 1; i++)
    {

        item *tmp = buckets[i].items;
        while (tmp != 0x0)
        {

            fprintf(log, "%d\n", tmp->value);
            buckets_to_send[i] = tmp->value;
            tmp = tmp->next;
        }
    }

    if (rank == 0)
    {
        int final_buckets[range + 1];
        MPI_Reduce(buckets_to_send,
                   final_buckets,
                   range + 1,
                   MPI_INT,
                   MPI_SUM,
                   0,
                   MPI_COMM_WORLD);

        FILE *output = fopen(argv[2], "w");

        for (int i = 0; i < range + 1; i++)
        {
            printf("%d\n", final_buckets[i]);
            fprintf(output, "%d\n", final_buckets[i]);
        }
        fclose(output);
    }
    else
    {
        MPI_Reduce(buckets_to_send,
                   0x0,
                   range + 1,
                   MPI_INT,
                   MPI_SUM,
                   0,
                   MPI_COMM_WORLD);
    }

    // finalizing //

    MPI_Barrier(MPI_COMM_WORLD);

    free(index_recv);
    free(values_recv);
    free(values);
    free(index);
    MPI_Finalize();
    return 0;
}