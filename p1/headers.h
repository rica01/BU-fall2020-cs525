// ********************
// Ricardo Roman-Brenes
// 22001125
//
// oct-2020
// Bilkent University
// CS525
// *********************

#define BOOL char
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

typedef struct item
{
    int value;
    struct item *next;
} item;

typedef struct bucket_list_item
{
    int id;
    struct item *items;
} bucket_list_item;


char *get_current_time();

int read_file(char *filename, int **data);
void get_min_max(int *array, int size, int *min, int *max);
int* get_file_data(char* filename, int elements, int* data);
int get_file_lines(char *filename);

void handler(int sig);
