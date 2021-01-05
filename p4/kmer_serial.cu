#include "util.h"


void computeLPSArray(char* pat, int M, int* lps) 
{ 

	int len = 0; 

	lps[0] = 0; // lps[0] is always 0 

	int i = 1; 
	while (i < M) { 
		if (pat[i] == pat[len]) { 
			len++; 
			lps[i] = len; 
			i++; 
		} 
		else // (pat[i] != pat[len]) 
		{ 
			if (len != 0) { 
				len = lps[len - 1]; 
			} 
			else // if (len == 0) 
			{ 
				lps[i] = 0; 
				i++; 
			} 
		} 
	} 
} 



int KMPSearch(char* pat, int M, char* txt, int N) 
{ 
	// char buff[M];
	int matches = 0;
	int lps[M]; 

	computeLPSArray(pat, M, lps); 

	int i = 0; // index for txt[] 
	int j = 0; // index for pat[] 
	while (i < N) { 
		if (pat[j] == txt[i]) { 
			j++; 
			i++; 
		} 

		if (j == M) {
			// printf("Found pattern at index %d\n", i - j);
			// substring(buff, pat, 0, M);
			// printf("%s", buff);
			// printf("==");
			// substring(buff, txt, i-j, i-j+M);
			// printf("%s\n", buff);
			matches++;
			j = lps[j - 1]; 
		} 

		// mismatch after j matches 
		else if (i < N && pat[j] != txt[i]) { 
			if (j != 0) 
				j = lps[j - 1]; 
			else
				i = i + 1; 
		} 
	} 
	return matches;
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
	char buff[kmer];
	// buff[kmer] = '\0';
	reference_length = strlen(reference_str); //Last character is '\n'
	
	
	int matches[queries.used];
    for (int u = 0; u < queries.used; u++)
    {
		// printf("read %d: %s\n", u, queries.array[u]);
		matches[u]=0;
		
		int k = 0;
		bool end = 0;
        while (!end)
        {
			substring(buff, queries.array[u], k, k+kmer);
			
			// printf("\n\tkmer %d: %s\n", k, buff);
			matches[u] += KMPSearch(buff, kmer, reference_str, reference_length);
			
            
            
            
            if (queries.array[u][k+kmer+1]=='\0')
            {
                end = 1;
            }else
            {
                k++;
            }
		}
		
		// printf("matches for read %d: %d\n",u, matches[u]);
    }

	


	// printf("\n");

	FILE* out_f = fopen(output_filename, "w");
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
