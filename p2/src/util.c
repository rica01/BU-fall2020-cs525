#include <stdio.h>
#include <stdlib.h>
#include "util.h"

/**
Copyright 2020 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

int read_file(char *file_name, int *numVertices, int *numEdges, int **offsets, int **edges)
{
    FILE *inputFile = fopen(file_name, "r");
    if (inputFile)
    {
        int success;
        success = fscanf(inputFile, "%d", &*numVertices);
        if (!success)
        {
            printf("Bad File format!\n");
            return 0;
        }
        success = fscanf(inputFile, "%d", &*numEdges);
        if (!success)
        {
            printf("Bad File format!\n");
            return 0;
        }
        *offsets = (int *)malloc(((*numVertices) + 1) * sizeof(int));
        *edges = (int *)malloc((*numEdges) * sizeof(int));

        for (int i = 0; i < *numVertices; i++)
        {
            success = fscanf(inputFile, "%d", &((*offsets)[i]));
            if (!success)
            {
                printf("Bad File format!\n");
                return 0;
            }
        }
        (*offsets)[*numVertices] = *numEdges;
        for (int i = 0; i < *numEdges; i++)
        {
            success = fscanf(inputFile, "%d", &((*edges)[i]));
            if (!success)
            {
                printf("Bad File format!\n");
                return 0;
            }
        }
        fclose(inputFile);
        return 1;
    }
    return 0;
}

void init_q(queue *q)
{
    q->count = 0;
    q->front = 0x0;
    q->rear = 0x0;
}

int is_empty(queue *q)
{
    return (q->count == 0);
}

void enqueue(queue *q, int value)
{
    pos *tmp;
    tmp = malloc(sizeof(pos));
    tmp->data = value;
    tmp->next = NULL;
    if (!is_empty(q))
    {
        q->rear->next = tmp;
        q->rear = tmp;
    }
    else
    {
        q->front = q->rear = tmp;
    }
    q->count++;
}

int dequeue(queue *q)
{
    int n = q->front->data;
    pos *tmp = q->front;

    q->front = q->front->next;
    q->count--;
    free(tmp);
    return (n);
}

void destroy_q(queue *q)
{
    if (q)
    {
        if (q->count > 0)
        {
            int n = q->count;
            for (int i = 0; i < n; i++)
            {
                dequeue(q);
            }
        }
        free(q);
    }
    return;
}

int pop(queue *q)
{
    if (q->count > 0)
    {
        int r = q->front->data;
        pos *p = q->front;
        q->front = q->front->next;
        free(p);
        q->count--;
        return r;
    }
    else
    {
        return -1;
    }
}
void push(queue *q, int d)
{
    q->count++;
    pos *p = malloc(sizeof(pos));
    p->next = q->front;
    p->data = d;
    q->front = p;
    return;
}

char *get_current_time()
{
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    return asctime(localtime(&raw_time));
};


int exists_in_queue(queue *lst, int d){
    struct pos *cur = lst->front;
    while(cur != NULL){
        if(cur->data == d)
            return 1;
        cur = cur->next;
    }
    return 0;
}
