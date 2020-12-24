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
    char *get_time()
    {
        time_t raw_time;
        struct tm *time_info;
        time(&raw_time);
        return asctime(localtime(&raw_time));
    };

    char d[200] = {'\0'};
    char *debug_string()
    {
        sprintf(d, "%s[%s] - (%d|%d)", get_time(), processor_name, rank, num_process);
        return d;
    };

    char c[6] = {rank + '0', '.', 'l', 'o', 'g', '\0'};
    FILE *output = fopen(c, "w");

    DEBUG &&fprintf(output, "%s >> %s\n", debug_string(), "reporting in...");
    // debug info //

    int array_size;
    int *local_data;

    // ======== // data distribution // ======== //
    if (rank == 0) // head node
    {

        //int elements = read_file(argv[1], &data);
        int elements = get_file_lines(argv[1]);
        int *data = malloc(sizeof(int) * elements);
        get_file_data(argv[1], elements, data);

        for (int i = 0; i < elements; i++)
        {
            DEBUG &&fprintf(output, "%s >> %d.\n", debug_string(), data[i]);
        }

        int chunk_size = elements / (num_process);
        int remaining = elements % (num_process);
        MPI_Request requests[num_process];

        DEBUG &&fprintf(output, "%s >> total data elements = %d. Head node will process chunk of %d. All others chunks of %d.\n", debug_string(), elements, chunk_size + remaining, chunk_size);

        int i;
        for (i = remaining; i < num_process; i++)
        {
            // send size
            MPI_Isend(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
            //send data
            MPI_Isend((data + remaining) + i * chunk_size, chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
        }

        array_size = chunk_size + remaining;
        local_data = (int *)malloc(sizeof(int) * array_size);
        for (int i = 0; i < array_size; i++)
        {
            local_data[i] = data[i];
        }
    }
    else // all the other nodes
    {
        int chunk_size;
        MPI_Request request;

        //receive size
        MPI_Irecv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        DEBUG &&fprintf(output, "%s >> waiting for data...\n", debug_string());

        MPI_Wait(&request, MPI_STATUS_IGNORE);
        DEBUG &&fprintf(output, "%s >> data: %d.\n", debug_string(), chunk_size);

        array_size = chunk_size;
        local_data = (int *)malloc(sizeof(int) * chunk_size);

        MPI_Irecv(local_data, chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        for (int i = 0; i < chunk_size; i++)
        {
            DEBUG &&fprintf(output, "%s >> %d.\n", debug_string(), local_data[i]);
        }
        // receive data
    }
    // ======== // data distribution // ======== //

    MPI_Barrier(MPI_COMM_WORLD);
    // ======== // local data processing // ======== //

    int local_min = INT_MAX;
    int local_max = INT_MIN;
    get_min_max(local_data, array_size, &local_min, &local_max);

    DEBUG &&fprintf(output, "%s >> local_min = %d; local_max = %d.\n", debug_string(), local_min, local_max);

    // ======== // local data processing // ======== //

    // ======== // data collection // ======== //

    if (rank != 0) // all nodes but master
    {
        MPI_Request request;
        // send size
        MPI_Isend(&local_min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        //send data
        MPI_Isend(&local_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    }
    else // master node
    {
        MPI_Request request;
        int local_mins[num_process];
        int local_maxs[num_process];

        local_mins[0] = local_min;
        local_maxs[0] = local_max;

        DEBUG &&fprintf(output, "%s >> ", debug_string(), "data from nodes (min|max): ");
        DEBUG &&fprintf(output, "(%d|%d)   ", local_mins[0], local_maxs[0]);

        for (int i = 1; i < num_process; i++)
        {
            MPI_Irecv(&(local_mins[i]), 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, MPI_STATUS_IGNORE);

            MPI_Irecv(&(local_maxs[i]), 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, MPI_STATUS_IGNORE);

            DEBUG &&fprintf(output, "(%d|%d)   ", local_mins[i], local_maxs[i]);
        }
        DEBUG &&fprintf(output, "\n");

        // ======== // data collection // ======== //

        // ======== // final data processing // ======== //

        for (int i = 0; i < num_process; i++)
        {
            if (global_max < local_maxs[i])
                global_max = local_maxs[i];
            if (global_min > local_mins[i])
                global_min = local_mins[i];
        }
        DEBUG &&fprintf(output, "%s >> min=%d, max=%d; range=%d.\n", debug_string(), global_min, global_max, global_max - global_min);
        printf("%d\n", global_max - global_min);
    }
    // ======== // final data processing // ======== //

    free(local_data);

    DEBUG &&fprintf(output, "%s >> %s\n", debug_string(), "signing off");
    fclose(output);

    // debug info //
    MPI_Finalize();
    return 0;
}
