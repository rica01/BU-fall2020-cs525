#include <time.h>
/*
*		reads a a graph file in CSR format, each integer number separated by line,
*       first line numRows, second line numColumns, next numrows lines define offsets
*       array, and very next numColumns lines define edges array.
*
*       @params:
*		file_name: name of the file
*		numRows: number of rows(vertices) in the file
*		numColumns: number of columns (edges) in the file
*       offsets: offsets(vertices) array
*       edges: edge(columns) array
*
*       Returns 0 if there is a file format or a reading problem, 1 if successful
*
*       Usage Example:
*       int numVertices,numEdges,*offsets,*edges;
*       int success = read_file(argv[1],&numVertices,&numEdges,&offsets,&edges);
*       if(success){
*           //Do the computation
*       }
**/
int read_file(char *file_name, int *numRows, int *numColumns, int **offsets, int **edges);

struct pos
{
    int data;
    struct pos *next;
};
typedef struct pos pos;

struct queue
{
    int count;
    pos *front;
    pos *rear;
};
typedef struct queue queue;

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void init_q(queue *q);

int is_empty(queue *q);

void enqueue(queue *q, int value);

int dequeue(queue *q);

void destroy_q(queue *q);

char *get_current_time();

int pop(queue *q);
void push(queue *q, int d);
int exists_in_queue(queue *lst, int d);