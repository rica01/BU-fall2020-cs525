#include "util.h"
#define TPB 1024
#define NB 2147483647


__global__ void testgpu(char *text, char *pattern, int *match, int pattern_size, int text_size)
{
    printf("%d> %s, %s, %d, %d, %d\n", threadIdx.x + blockIdx.x*blockDim.x, text, pattern, match[threadIdx.x + blockIdx.x*blockDim.x], pattern_size, text_size);
}

__global__ void match(char *text, char *pattern, int *match, int pattern_size, int text_size){

    int pid = threadIdx.x + blockIdx.x*blockDim.x;

    if (pid <= text_size - pattern_size){
    
        int flag = 1; 
        for (int i = 0; i < pattern_size; i++){
            if (text[pid+i] != pattern[i]){
                    flag = 0;
            }
        }
        match[pid] = flag;
    }
    // printf("\t\tpid=%d, %s in %s: %d\n", pid, pattern, text, match[pid]);
    return;
}


void GPUinfo()
{
    int num_gpus;
    size_t free, total;
    cudaGetDeviceCount( &num_gpus );
    for ( int gpu_id = 0; gpu_id < num_gpus; gpu_id++ ) {
        cudaSetDevice( gpu_id );
        int id;
        cudaGetDevice( &id );
        cudaMemGetInfo( &free, &total );
        // cout << "GPU " << id << " memory: free=" << free << ", total=" << total << endl;
        printf("GPU %d free=%ld | total=%ld\n", id, free, total);
    }
    return;

}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("Wrong argments usage: ./kmer [REFERENCE_FILE] [READ_FILE] [k] [OUTPUT_FILE]\n");
    }

    FILE *fp;
    int kmer;

    char *reference_str = (char *)malloc(MAX_REF_LENGTH * sizeof(char));
    char *read_str = (char *)malloc(MAX_READ_LENGTH * sizeof(char));

	char *reference_filename;
	char *read_filename;
	char *output_filename;
    int reference_length;

    reference_filename = argv[1];
    read_filename = argv[2];
    kmer = atoi(argv[3]);
    output_filename = argv[4];


    fp = fopen(reference_filename, "r");

    if (fgets(reference_str, MAX_REF_LENGTH, fp) == NULL)
    { //A single line only
        printf("Problem in file format!\n");
        return 1;
    }

    substring(reference_str, 0, strlen(reference_str) - 1);
    // printf("Reference str is = %s\n", reference_str);
    fclose(fp);

    //Read queries
    StringList queries;
	initStringList(&queries, 3); // initially 3 reads
	

    int success = read_file(read_filename, &queries);
	// char buff[kmer];
    // buff[kmer] = '\0';
    // int matches[queries.used];
	reference_length = strlen(reference_str); //Last character is '\n'
	

    int h_matches[reference_length] = {0};
    int* d_matches = NULL;
    char* d_reference = NULL;
    char* d_kmer;
    cudaMalloc((void **)&d_reference, reference_length*sizeof(char));
    cudaMalloc((void **)&d_matches, reference_length*sizeof(int));
    cudaMalloc((void **)&d_kmer, kmer*sizeof(char));
    cudaMemcpy(d_reference, reference_str, reference_length*sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_matches, h_matches, reference_length*sizeof(int), cudaMemcpyHostToDevice);


    char buff[kmer];
    // buff[kmer] = '\0';
    int matches[queries.used] = {0};
    int sum =0;


	FILE* out_f = fopen(output_filename, "w");


    
        int num_blocks = reference_length / TPB +1;
        
        for (int u = 0; u < queries.used; u++)
        {
            // printf("read %d: %s\n", u, queries.array[u]);
            
            int k = 0;
            bool end = 0;
            while (!end)
            {
                substring(buff, queries.array[u], k, k+kmer);
                cudaMemcpy(d_kmer, buff, kmer*sizeof(char), cudaMemcpyHostToDevice);
                
                // matches[u] += KMPSearch(buff, kmer, reference_str, reference_length);

                
                // printf("\n\tkmer %d: %s\n", k, buff);
                match<<<num_blocks, TPB>>>(d_reference, d_kmer, d_matches, kmer, reference_length);
                

                cudaError_t cudaerr = cudaDeviceSynchronize();
                if (cudaerr != cudaSuccess)
                    printf("kernel launch failed with error \"%s\".\n", cudaGetErrorString(cudaerr));
                // else
                    // printf("no errors in kernel launch\n");

                cudaMemcpy(h_matches, d_matches, reference_length*sizeof(int), cudaMemcpyDeviceToHost);


                sum = 0;
                #pragma omp parallel for shared(sum, h_matches) reduction(+: sum)
                for (int i = 0; i < reference_length; i++)
                {
                    // printf("h_matches[%d]=%d\n", i, h_matches[i]);
                    sum += h_matches[i];
                }
                matches[u] += sum;

                if (queries.array[u][k+kmer+1]=='\0')
                    end = 1;
                else
                    k++;
            }
            // printf("matches for read %d: %d\n",u, matches[u]);
        }


	// printf("\n");

	// fprintf(out_f, "bai\n");
	for(int r = 0; r < queries.used; r++)
	{
		fprintf(out_f, "%d\n", matches[r]);
    }
    

	fclose(out_f);
    //Free up
    freeStringList(&queries);

    free(reference_str);
    free(read_str);
}
