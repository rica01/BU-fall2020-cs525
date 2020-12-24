// ********************
// Ricardo Roman-Brenes
// 22001125
//
// oct-2020
// Bilkent University
// CS525
// *********************

#include "./headers.h"

char *get_current_time()
{
	time_t raw_time;
	struct tm *time_info;
	time(&raw_time);
	return asctime(localtime(&raw_time));
};





void handler(int sig)
{
	void *array[10];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

int get_file_lines(char *filename)
{
	FILE *input = fopen(filename, "r");
	char c = 1;
	int elements = 0;

	while (c != EOF)
	{
		c = getc(input);
		if (c == '\n')
			elements++;
	}
	fclose(input);
	return elements;
}

// data must be a valid pointer
int *get_file_data(char *filename, int elements, int *data)
{
	FILE *input = fopen(filename, "r");
	BOOL loop_end = FALSE;
	int read_status = 1;
	int buff;

	for (int i = 0; i < elements; i++)
	{
		fscanf(input, "%d", &buff);
		data[i] = buff;
	}

	fclose(input);

	return data;
}

// data must be a valid pointer
int **get_file_pairs(char *filename, int elements, int **data)
{
	FILE *input = fopen(filename, "r");
	int buff;

	for (int i = 0; i < elements; i++)
	{
		fscanf(input, "%d %d", data[i][0], data[i][1]);
	}
	fclose(input);

	return data;
}

void get_min_max(int *array, int size, int *min, int *max)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i] > *max)
			*max = array[i];
		if (array[i] < *min)
			*min = array[i];
	}
	return;
}

void swap(bucket_list_item *xp, bucket_list_item *yp)
{
	bucket_list_item temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// A function to implement bubble sort
void bubbleSort(bucket_list_item *arr, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
	{
		// Last i elements are already in place
		for (j = 0; j < n - i - 1; j++)
		{
			if (arr[j].id > arr[j + 1].id)
			{

				swap(&arr[j], &arr[j + 1]);
			}
		}
	}

	return;
}