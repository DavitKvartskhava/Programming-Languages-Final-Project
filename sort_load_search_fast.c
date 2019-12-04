#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

const int ARRAY_SIZE = 1000000000;
// const int BUFFER_SIZE = 1024*1024;
const int CHUNK_SIZE = 9;

int int_cmp(const void *a, const void *b){
	const int *int_a = (const int *)a;
	const int *int_b = (const int *)b;
	
	return *int_a - *int_b;
}

char rem[20];
int rem_SIZE;
int k;

void convert(long long *arr, char *buffer, int buffer_size){
	long long next_int = 0;
	long coeff = 1;
	char curr_rem[12];
	int curr_rem_SIZE;
	int i = buffer_size;
	int l = 0;

	//getting to the point where a new int starts
	//everything else should go to remainder
	while(buffer[i] != '\n') {i--;}

	while(i > 0) {
		i--;
		if(buffer[i] == '\n'){
			arr[k++]= next_int;
			next_int = 0;
			coeff = 1;
		} else {

			if(buffer[i] == '-'){
				next_int *= -1;
			} else {
				next_int += coeff * (buffer[i]-'0');
				coeff = coeff * 10;
			}
		}
	}

	//because at the start of the file we don't have \n
	if (rem_SIZE != 0) {
		for(int m = 0; m < rem_SIZE; m++){
			if (rem[m] == '\n') {printf("This shouldn't have happened!\n");}

			if(rem[m] == '-'){
				next_int *= -1;

			} else {
				next_int += coeff * (rem[m]-'0');
				coeff = coeff * 10;
			}
		}
	}
	arr[k++] = next_int;

	// ////////////////
	// Based on remainder, fill rem array
	// ////////////////
	i = buffer_size-1;
	while(buffer[i] != '\n') {
		rem[l++] = buffer[i--];
	}
	rem_SIZE = l; //l is incremented already
}

void read_file(long long *arr, char *filename){
	FILE *fs;
	char *buffer;

	// Add newline at the end of file if needed. Needed for convert's algorithm.
	// fs = fopen(filename, "a");
	// fprintf(fs,"\n");
	// fclose(fs);

	if(!(fs = fopen(filename, "rb"))){
		fprintf(stderr, "error encounted while opening the file\n");
	}

	/* Get the number of bytes */
	fseek(fs, 0L, SEEK_END);
	long file_size = ftell(fs);
	fseek(fs, 0L, SEEK_SET);

	long buffer_size = file_size/CHUNK_SIZE;
	long remainder_size = file_size % CHUNK_SIZE; 

	buffer = malloc(buffer_size);

	for(int i = 0; i < CHUNK_SIZE; i++){
		if(fread(buffer, buffer_size, 1, fs) == 1) {
			convert(arr, buffer, buffer_size);
		} else {
			printf("Gone wrong buddy\n");
		}
	}

	//REad the rest of the file.
    free(buffer);
    buffer = malloc(remainder_size);
    if (fread(buffer, remainder_size, 1, fs) == 1){
    	// printf("BUFFER HAS RETURNED:");
    	// printf("[");
    	// for(int i = 0; i < remainder_size; i++){
    	// 	printf("%c,", buffer[i]);
    	// }
    	// printf("]\n");
    	convert(arr, buffer, remainder_size);
    }

    fclose(fs);
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
	k = 0;
	rem_SIZE = 0;
	read_file(values, values_filename);
	int line_count_values = k;

	// k=0;
	// rem_SIZE = 0;
	// read_file(keys, keys_filename);
	// int line_count_keys = k;


    for(int i = 0; i < k; i++){
    	printf("%lld\n",values[i]);
    }

	// printf("HELLO!!!%d", line_count_values);
	// printf("HELLO!!!%d", line_count_keys);

	// qsort(values, ARRAY_SIZE, sizeof(long long), int_cmp);

	// int *index;
	// for(int i = 0; i < line_count_keys; i++){
	// 	index = bsearch(&keys[i], values, ARRAY_SIZE, sizeof(long long), int_cmp); // Maybe ARRAY_SIZE -> line_count_values

	// 	if (index!=NULL){	

	// 		printf("%d\n", *index);
	// 	}
	// }
}
