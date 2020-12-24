#include "parallel_bfs.h"

void init_csr(csr *g, int n_rows, int n_cols, int *vertex, int *edges)
{
    g->edges = edges;
    g->n_cols = n_cols;
    g->n_rows = n_rows;
    g->vertex = vertex;
    print_csr(g);
}

void print_array(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        fprintf(logfile, "[%d] %d\t", i, arr[i]);
    }
    fprintf(logfile, "\n");
}

void print_csr(csr *g)
{

    print_array(g->vertex, g->n_rows);
    print_array(g->edges, g->n_cols);
}

void get_neighbors(int vertex, int first_vertex,
                   int *my_edgesList, int my_numEdges,
                   int *my_vertexList, int my_numVertex,
                   queue *neighbors_q)
{
    int alpha = my_vertexList[vertex];
    int omega = my_vertexList[vertex + 1];

    if (alpha == omega)
    {
        neighbors_q = 0x0;
    }
    else
    {
    }
}

int get_num_neighbors(csr *g, int src)
{
    int n = 0;

    int alpha = g->vertex[src];
    int omega = g->vertex[src + 1];
    return omega - alpha;
}

int get_neighbors_seq(csr *g, int src, int **neighbors)
{
    int n = 0;

    int alpha = g->vertex[src];
    int omega = g->vertex[src + 1];

    if (alpha == omega) // no neighbors
    {
        *neighbors = NULL;
        n = 0;
    }
    else
    {
        n = omega - alpha;
        *neighbors = (int *)malloc(sizeof(int) * n);

        for (int nli = 0, eli = g->vertex[src]; nli < n; nli++, eli++)
        {
            (*neighbors)[nli] = g->edges[eli];
        }
    }
    //return neighbors;
    return n;
}

void generate_frontier(int *level, int l, int first_vertex, int num_of_vertices_per_proc, queue *F)
{
    int i; // for iteration
    for (i = 0; i < num_of_vertices_per_proc; i++)
    {
        if (*(level + i) == l)
            push(F, first_vertex + i);
    }
    return;
}

////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{

    // declarations
    int i, j, k;                   // for iterations
    int myrank;                    // rank of the processor
    int world_size;                //
    int source_vertex;             // source vertex for BFS
    int first_vertex;              // index of first vertex that belong to the processor
    unsigned char *sub_adj_matrix; // store sub-adjacency matrix local to processor
    int *level;                    // store the length of path from start_vertex
    int lvl;                       // to keep track of level information for BFS
    struct list *F;                // to store the Frontier in a list
    queue frontier_q;
    struct list *N; // to store the Neighbors in a list
    queue neighbors_q;
    int frontier_has_vertices, continue_search; // for terminating the BFS
    int vertex;                                 // to store the popped vertex from Frontier
    int neighbor, owner;                        // owner represents the owner of the vertex neighbor
    struct list **map;                          // stores neighbors that need to be sent to each proc
    int *send_counts;                           // number of neighbors that will be sent to each proc
    int *send_buf;                              // contains the data that need to be sent to each proc
    int *send_disps;                            // start index of the data that has to be sent to each proc
    int *recv_counts;                           // number of neighbors to be received from each proc
    int *recv_buf;                              // to store the data to be received from each proc
    int *recv_disps;                            // start index of storage for data received from each proc
    int send_buf_size = 0;                      // size of the send_buf
    int recv_buf_size = 0;                      // size of the recv_buf
    struct list *lst;                           // temporary storeage
    int rel_vertex;                             // to store the relative number of proc vertex in a proc
    MPI_Status status;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    unsigned char *adjMatrix;
    int p;

    char logfilename[] = {myrank + 48, '.', 'l', 'o', 'g', '\0'};
    logfile = fopen(logfilename, "w");
    // logfile = stdout;
    fprintf(logfile, "w0la %s\n", get_current_time());
    source_vertex = atoi(argv[2]);

    int my_numVertex;
    int my_numEdges;
    int *my_edgesList;  // list of column indexes of non-zero vaules per row
    int *my_vertexList; // list of edges array's index of first non-zero entry for each row
    int *my_numOutEdges;

    if (myrank == 0)
    {
        int numVertex_io;
        int numEdges_io;
        int *edges_io;  // list of column indexes of non-zero vaules per row
        int *vertex_io; // list of edges array's index of first non-zero entry for each row
        read_file(argv[1], &numVertex_io, &numEdges_io, &vertex_io, &edges_io);
        csr g;
        init_csr(&g, numVertex_io, numEdges_io, vertex_io, edges_io);
        int vertex_per_proc = numVertex_io / (world_size);
        int remaining = numVertex_io % (world_size);

        for (p = 1; p < world_size; p++)
        {
            // printf("\t\t\tenviando desde %d hasta %d a proc %d\n\n",
            //        *((vertex_io + remaining) + p * vertex_per_proc),
            //        *((vertex_io + remaining) + p * vertex_per_proc + vertex_per_proc),
            //        p);
            MPI_Send(&vertex_per_proc, 1, MPI_INT, p, 420, MPI_COMM_WORLD);
            MPI_Send((vertex_io + remaining) + p * vertex_per_proc, vertex_per_proc, MPI_INT, p, 420, MPI_COMM_WORLD);

            int edges_per_proc = 0;
            int ini_rango, fin_rango;
            for (int vpp = p * vertex_per_proc; vpp < p * vertex_per_proc + vertex_per_proc; vpp++)
            {
                // por cada proc
                // el inicio del rango de vertices
                ini_rango = vertex_io[vpp + remaining];
                // el fin del rango de vertices
                fin_rango = vertex_io[vpp + remaining + vertex_per_proc];
                // haga la resta, -> edges_per_proc
                edges_per_proc = fin_rango - ini_rango;
                // empiece a copiar desde inicio de rango
                // printf("para proc %d: ini: %d, fin: %d, epp: %d\n", p, ini_rango, fin_rango, edges_per_proc);
                MPI_Send(&edges_per_proc, 1, MPI_INT, p, 420, MPI_COMM_WORLD);
                MPI_Send(
                    &edges_io[ini_rango],
                    edges_per_proc,
                    MPI_INT, p, 420, MPI_COMM_WORLD);
            }
            int first_v_of_p = (vertex_per_proc + remaining) + p * vertex_per_proc;
            MPI_Send(&first_v_of_p, 1, MPI_INT, p, 420, MPI_COMM_WORLD);
        }

        my_numVertex = vertex_per_proc + remaining;
        my_vertexList = (int *)malloc(sizeof(int) * my_numVertex + 1);
        my_numEdges = 0;
        for (int x = 0; x < vertex_per_proc + remaining; x++)
        {
            my_vertexList[x] = vertex_io[x];
            my_numEdges += get_neighbors_seq(&g, x, &my_edgesList);
        }
        my_vertexList[my_numVertex] = my_numEdges;

        my_edgesList = (int *)malloc(sizeof(int) * my_numEdges);
        for (int x = 0; x < my_numEdges; x++)
        {
            my_edgesList[x] = edges_io[x];
        }

        first_vertex = 0;
    }
    else // all of the others
    {
        MPI_Recv(&my_numVertex, 1, MPI_INT, 0, 420, MPI_COMM_WORLD, &status);
        my_vertexList = (int *)malloc(sizeof(int) * my_numVertex + 1);
        my_vertexList[my_numVertex] = my_numEdges;

        MPI_Recv(my_vertexList, my_numVertex, MPI_INT, 0, 420, MPI_COMM_WORLD, &status);

        MPI_Recv(&my_numEdges, 1, MPI_INT, 0, 420, MPI_COMM_WORLD, &status);
        my_edgesList = (int *)malloc(sizeof(int) * my_numEdges);

        MPI_Recv(my_edgesList, my_numEdges, MPI_INT, 0, 420, MPI_COMM_WORLD, &status);

        MPI_Recv(&first_vertex, 1, MPI_INT, 0, 420, MPI_COMM_WORLD, &status);
    }
    fprintf(logfile, "\n");

    fprintf(logfile, "[%d|%d]:>", myrank, world_size - 1);
    fprintf(logfile, "\tnumVertex > %d\n", my_numVertex);
    print_array(my_vertexList, my_numVertex);

    fprintf(logfile, "\n");
    fprintf(logfile, "[%d|%d]:>", myrank, world_size - 1);
    fprintf(logfile, "\tnumEdges > %d\n", my_numEdges);
    print_array(my_edgesList, my_numEdges);
    fprintf(logfile, "\n");

    printf("P%d: local_number_of_vertices=%d\n", myrank, my_numVertex);
    for (int x = 0; x < my_numVertex; x++)
    {
        printf("P%d:\tvertices[%d]=%d\n", myrank, x, my_vertexList[x]);
    }
    printf("P%d: local_number_of_edges=%d\n", myrank, my_numEdges);
    for (int x = 0; x < my_numEdges; x++)
    {
        printf("P%d:\tedges[%d]=%d\n", myrank, x, my_edgesList[x]);
    }
    // ///////////////////////////////////////////////////////
    // MPI_Barrier(MPI_COMM_WORLD);
    // ///////////////////////////////////////////////////////

    // level = (int *)malloc(sizeof(int) * my_numVertex);
    // for (i = 0; i < my_numVertex; i++)
    // {
    //     if (first_vertex + i == source_vertex)
    //     {
    //         level[i] = 0;
    //     }
    //     else
    //         level[i] = -1;
    // }
    // fprintf(logfile, "[%d|%d]:>", myrank, world_size - 1);
    // fprintf(logfile, "level >\n");
    // print_array(level, my_numVertex);
    // fprintf(logfile, "\n");

    // init_q(&frontier_q);
    // init_q(&neighbors_q);

    // send_counts = (int *)malloc(sizeof(int) * world_size);
    // send_disps = (int *)malloc(sizeof(int) * world_size);

    // recv_counts = (int *)malloc(sizeof(int) * world_size);
    // recv_disps = (int *)malloc(sizeof(int) * world_size);

    // printf("[%d|%d]\t\t%d my_numVertex\n", myrank, world_size - 1, my_numVertex);
    // continue_search = TRUE;

    for (lvl = 0; lvl < my_numVertex - 1; lvl++)
    {
        //     printf("\t\t\t[%d|%d]>%d\n", myrank, world_size - 1, lvl);
        //     MPI_Barrier(MPI_COMM_WORLD);

        //     // frontier
        //     generate_frontier(level, lvl, first_vertex, my_numVertex, &frontier_q);
        //     if (frontier_q.count > 0)
        //         frontier_has_vertices = 1;
        //     else
        //         frontier_has_vertices = 0;

        //     MPI_Barrier(MPI_COMM_WORLD);

        //     printf("[%d|%d]\t\tdoing allreduce\n", myrank, world_size - 1);
        // MPI_Allreduce(&frontier_has_vertices, &continue_search, 1, MPI_INT, MPI_LOR, MPI_COMM_WORLD);

    } // end for

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}