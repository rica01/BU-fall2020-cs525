// ********************
// Ricardo Roman-Brenes
// 22001125
//
// oct-2020
// Bilkent University
// CS525
// *********************

#include "serial_bfs.h"

int get_neighbors(csr *g, int src, int **neighbors)
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
        fprintf(logfile, "%d:%d\t", i, arr[i]);
    }
    fprintf(logfile, "\n");
}

void print_csr(csr *g)
{

    print_array(g->vertex, g->n_rows);
    print_array(g->edges, g->n_cols);
}

int *bfs(csr *g, int src)
{

    BOOL seen[g->n_rows];
    int *dist = (int *)malloc(sizeof(int) * g->n_rows);
    for (int i = 0; i < g->n_rows; i++)
    {
        seen[i] = FALSE;
        dist[i] = -1;
    }

    queue q;
    init_q(&q);

    enqueue(&q, src);
    seen[src] = TRUE;
    dist[src] = 0;

    fprintf(logfile, "bfs:\t");
    while (!is_empty(&q))
    {

        int i = dequeue(&q);

        fprintf(logfile, "%d\t", i);
        int *neighbors_of_i;
        int n = get_neighbors(g, i, &neighbors_of_i);

        for (int j = 0; j < n; j++)
        {
            int v = neighbors_of_i[j];
            if (!seen[v] && v != i)
            {
                enqueue(&q, v);
                seen[v] = TRUE;
                dist[v] = dist[i] + 1;
            }
        }
        free(neighbors_of_i);
    }
    fprintf(logfile, "\ndists\t");
    print_array((int *)dist, g->n_rows);
    return dist;
}

void save_to_output(int *data, int n, char *filename)
{
    FILE *out = fopen(filename, "w");
    for (int i = 0; i < n; i++)
    {
        fprintf(out, "%d\n", data[i]);
    }
    fclose(out);
    return;
}

//////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    logfile = fopen("serial_bfs.logfile", "w");
    // logfile = stdout;
    fprintf(logfile, "w0la\n");

    int numRows;
    int numColumns;
    int *edges;   // list of column indexes of non-zero vaules per row
    int *offsets; // list of edges array's index of first non-zero entry for each row

    read_file(argv[1], &numRows, &numColumns, &offsets, &edges);

    csr g;
    init_csr(&g, numRows, numColumns, offsets, edges);

    int *neighbors;

    for (int i = 0; i < g.n_rows; i++)
    {
        int num_neighbors = get_neighbors(&g, i, &neighbors);

        fprintf(logfile, "neighbors of %d:\n\t >", i);
        print_array(neighbors, num_neighbors);
        fprintf(logfile, "\n\n");
    }

    int *distances_from_source = bfs(&g, atoi(argv[2]));

    save_to_output(distances_from_source, g.n_rows, argv[3]);

    fprintf(logfile, "bai!\n");

    fclose(logfile);
    return 0;
}