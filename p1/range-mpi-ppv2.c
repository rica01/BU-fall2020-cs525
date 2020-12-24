// ********************
// Ricardo Roman-Brenes
// 22001125
//
// oct-2020
// Bilkent University
// CS525
// *********************


#include "./headers.h"
#include <mpi.h>

#define DEBUG 1

int main(int argc, char **argv)
{

    signal(SIGSEGV, handler); // install our handler

    // MPI setup //
    MPI_Init(&argc, &argv);
    int num_process, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_length;
    MPI_Get_processor_name(processor_name, &name_length);
    // MPI setup //

    int read_status = 1;
    int buff = 0;
    int global_max = INT_MIN;
    int global_min = INT_MAX;
    int range = 0;
    BOOL loop_end;

    // debug info //


    // char d[200] = {'\0'};
    // char *debug_string(char* d)
    // {
    //     sprintf(d, "%s[%s] - (%d|%d)", get_current_time(), processor_name, rank, num_process);
    //     return d;
    // };

    char c[6] = {rank + '0', '.', 'l', 'o', 'g', '\0'};
    FILE *output = fopen(c, "w");

    DEBUG &&fprintf(output, "%s[%s] - (%d|%d) >> %s\n", get_current_time(), processor_name, rank, num_process, "reporting in...");
    // debug info //

    int array_size = 666;
    int *local_data = (int *)0x666;

    // ======== // data distribution // ======== //
    if (rank == 0) // head node
    {

        array_size = get_file_lines(argv[1]);

        local_data = (int *)malloc(sizeof(int) * array_size);
        local_data = get_file_data(argv[1], array_size, local_data);

        for (int i = 0; i < array_size; i++)
        {
            DEBUG &&fprintf(output, "%s[%s] - (%d|%d) >> %d.\n", get_current_time(), processor_name, rank, num_process, local_data[i]);
        }

        // int chunk_size = array_size / (num_process);
        // int remaining = array_size % (num_process);
        // MPI_Request requests[num_process];

        DEBUG &&fprintf(output, "%s[%s] - (%d|%d) >> total data elements = %d. All nodes will process all data.\n", get_current_time(), processor_name, rank, num_process, array_size);
    }

    // send data size
    MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    DEBUG &&fprintf(output, "%s[%s] - (%d|%d) >> communication from master: data elements = %d.\n", get_current_time(), processor_name, rank, num_process, array_size);


    if(rank != 0) local_data = (int *)malloc(sizeof(int) * array_size);

    MPI_Bcast(local_data, array_size, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < array_size; i++)
    {
        DEBUG &&fprintf(output, "%s[%s] - (%d|%d) >> %d.\n", get_current_time(), processor_name, rank, num_process, local_data[i]);
    }
    // ======== // data distribution // ======== //


    // ======== // local data processing // ======== //

    get_min_max(local_data, array_size, &global_min, &global_max);
    range = global_max - global_min;
    DEBUG &&fprintf(output, "%s[%s] - (%d|%d) >> global_min = %d; global_max = %d; range = %d\n", get_current_time(), processor_name, rank, num_process, global_min, global_max, range);


    // ======== // local data processing // ======== //

    free(local_data);

    DEBUG &&fprintf(output, "%s[%s] - (%d|%d) >> %s\n", get_current_time(), processor_name, rank, num_process, "signing off");
    fclose(output);

    if(rank == 0 ) printf("%d\n", range);
    // debug info //
    MPI_Finalize();
    return 0;
}
