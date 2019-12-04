#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

const int ARRAY_SIZE = 1000005;

int int_cmp(const void *a, const void *b){
	const int *int_a = (const int *)a;
	const int *int_b = (const int *)b;
	
	return *int_a - *int_b;
}

int read_file(long long *arr, char *filename){
	FILE *fptr;
    int line_count = 0, j = 0;
    long long next_value;

	if(!(fptr = fopen(filename, "r"))){
		fprintf(stderr, "error encounted while opening the file\n");
	}

	while(fscanf(fptr, "%lld", &next_value) != EOF){
		arr[j++] = next_value;
		line_count++;
	}
	fclose(fptr);
	return line_count;
}
long long *values, *keys;

int main(int argc, char *argv[]){
	char values_filename[128], keys_filename[128];
	values = malloc(ARRAY_SIZE * sizeof(long long));
	keys = malloc(ARRAY_SIZE * sizeof(long long));

	if(argc != 3){
		fprintf(stderr, "enter both filenames, please\n");
		return -1;
	} else {
		strcpy(values_filename, argv[1]);
		strcpy(keys_filename, argv[2]);
	}

	int line_count_values = read_file(values, values_filename);
	int line_count_keys = read_file(keys, keys_filename);
	// // printf("HELLO!!!%d", line_count_values);
	// // printf("HELLO!!!%d", line_count_keys);

	qsort(values, ARRAY_SIZE, sizeof(long long), int_cmp);

	int *index;
	for(int i = 0; i < line_count_keys; i++){
		index = bsearch(&keys[i], values, ARRAY_SIZE, sizeof(long long), int_cmp);

		if (index!=NULL){	

			printf("%d\n", *index);
		}
	}
}
