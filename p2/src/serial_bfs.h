#define BOOL char
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

#include "util.h"

FILE *logfile = 0x0;
typedef struct csr
{
    int n_rows; // vertex numbers
    int n_cols;
    int *vertex;
    int *edges;
} csr;

void print_array(int *arr, int n);
int *bfs(csr *g, int src);
void init_csr(csr *g, int, int, int *, int *);
void print_csr(csr *g);
int get_neighbors(csr *g, int src, int **neighborgs);
void save_to_output(int *data, int n, char *filename);
