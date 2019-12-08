/*By Davit Kvartskhava*/
////////////////////////
// Compile with the command gcc -fopenmp sort_load_search_parallel.c -o out -std=c99 -O3 (or -Ofast)
// 16 cores for fastest performance.(export OMP_NUM_THREADS = 16)
// Run it with time ./out /cluster/home2/charliep/courses/cs440-languages/billions/1b-ints.dat /cluster/home2/charliep/courses/cs440-languages/billions/1b-search.dat > result.dat
// Buffer_SIZE = 65536 is recommended because it yields the best results.
// Result.dat and canon need to be sorted before compared with diff.

/*Imports*/
#include <omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

const int ARRAY_SIZE = 1000000000;
const int BUFFER_SIZE = 65536;
long k;


int int_cmp(const void * a, const void * b)
{
    if( *(long long int*)a - *(long long int*)b < 0 )
        return -1;
    if( *(long long int*)a - *(long long int*)b > 0 )
        return 1;
    return 0;
}

//Helper funct for quicksort
//Moves all elems <= pivot to the left, and all all the elems > pivot to the right.
//Returns the index of the current pivot.
long partition(long *arr, long low, long high){ 
	long wall = low - 1;
	long pivot = arr[high];

	for(int i = low; i <= high; i++){
	 	if (arr[i] <= pivot){

	 		wall++;

	 		long temp = arr[wall];
	 		arr[wall] = arr[i];
	 		arr[i] = temp;
	 	 }
	 }
	 return wall;
}

//Actual quicksort algorithm.
//Using recursion.
void quicksort(long *arr, long low, long high){
	if (low < high) {
		long wall = partition(arr, low, high);
		
		#pragma omp task default(none) firstprivate(arr, low, wall)
		quicksort(arr, low, wall - 1); // omit index wall, because the number belongs there.

		#pragma omp task default(none) firstprivate(arr, high, wall)
		quicksort(arr, wall + 1, high);
	}
	return;
}

/////////////////////////////////////////////////////////////////

char rem[20];
int rem_SIZE;

void convert(long *arr, char *buffer, int buffer_size){
	long next_int = 0;
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

void read_file(long *arr, char *filename){
	FILE *fs;
	char *buffer;

	if(!(fs = fopen(filename, "rb"))){
		fprintf(stderr, "error encounted while opening the file\n");
	}

	/* Get the number of bytes */
	fseek(fs, 0L, SEEK_END);
	long file_size = ftell(fs);
	fseek(fs, 0L, SEEK_SET);

	int num_chunk = file_size / BUFFER_SIZE;
	long remainder_size = file_size % BUFFER_SIZE; 
	//long remainder_size = file_size % CHUNK_SIZE; 

	buffer = malloc(BUFFER_SIZE);

	for(int i = 0; i < num_chunk; i++){
		if(fread(buffer, BUFFER_SIZE, 1, fs) == 1) {
			convert(arr, buffer, BUFFER_SIZE);
		} else {
			printf("Gone wrong buddy\n");
		}
	}

	//REad the rest of the file.
    free(buffer);
    buffer = malloc(remainder_size);
    if (fread(buffer, remainder_size, 1, fs) == 1){
    	convert(arr, buffer, remainder_size);
    }

    fclose(fs);
}

int main(int argc, char *argv[]){
	char values_filename[128], keys_filename[128];

	long *values = (long *)malloc(ARRAY_SIZE * sizeof(long));
	long *keys = (long *)malloc(ARRAY_SIZE * sizeof(long));


	if(argc != 3){
		fprintf(stderr, "enter both filenames, please\n");
		return -1;
	} else {
		strcpy(values_filename, argv[1]);
		strcpy(keys_filename, argv[2]);
	}

	///////////////////
	/*Reading both files*/
	k = 0;
	rem_SIZE = 0;
	read_file(values, values_filename);
	long values_size = k;


	k = 0;
	rem_SIZE = 0;
	read_file(keys, keys_filename);
	long keys_size = k;


	/*Parallel quicksort*/
	#pragma omp parallel default(none) shared(values, values_size)
	{
		#pragma omp single nowait
		{quicksort(values,0, values_size-1);}
	}

	long *index;

	/*Going through keys array and checking if next key is in values array.*/
	#pragma omp parallel for private(index) shared(values_size)
	for(long i = 0; i < keys_size; i++){
		index = bsearch(&keys[i], values, values_size, sizeof(long), int_cmp);
		if(index!=NULL){
			printf("%lld\n", *index);
		}
	}
	#pragma omp barrier

	free(values);
	free(keys);

}
