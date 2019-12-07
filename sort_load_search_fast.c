/*By Davit Kvartskhava*/
////////////////////////
//Compile with the command gcc sort_load_search_fast.c -o out -std=c99 -O3 (or -Ofast)
//Run it with time ./out /cluster/home2/charliep/courses/cs440-languages/billions/1b-ints.dat /cluster/home2/charliep/courses/cs440-languages/billions/1b-search.dat > result.dat
//For 1b integers, it's best to use HASH_SIZE = 1b*1.3
//Buffer_SIZE = 65536 is recommended because it yields the best results.
//Result.dat and canon need to be sorted before compared with diff.

/*Imports*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include <stdbool.h>


const int ARRAY_SIZE = 1000000000;
const int HASH_SIZE = 1300000000;
//const int ARRAY_SIZE = 2000000;
//const int HASH_SIZE = 2600000;

const int BUFFER_SIZE = 65536;
long k;
struct table *values;
long *keys;
char rem[20];
int rem_SIZE;

///////////////////////////////////////////////////////////////////
////////HASHMAP////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
struct node{
    long key;
    struct node *next;
};
struct table{
    long size;
    struct node **list;
};
struct table *createTable(long size){
    struct table *t = (struct table*)malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}
int hashCode(struct table *t,long key){
    if(key<0)
        return -(key%t->size);
    return key%t->size;
}

void insert(struct table *t,long key){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    newNode->next = list;
    t->list[pos] = newNode;
}

//If successful return 1, else return -1
int search(struct table *t, long key){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *temp = t->list[pos];
    // struct node *temp = t->list[pos];
    while(temp){
        if(temp->key==key){
            return 1;
        }
        temp = temp->next;
    }
    return -1;
}

/* Converts the characters from buffer into longs and stores them in either hashmap or array*/
// The buffer might include the characters that don't add up to one integer, so everything
// before '\n' is stored in the remainder array.
void convert(char *buffer, int buffer_size, bool write_to_arr){
	long next_int = 0;
	long coeff = 1;
	int i = buffer_size;
	int l = 0;

	//getting to the point where a new int starts
	//everything else should go to remainder
	while(buffer[i] != '\n') {i--;}

	while(i > 0) {
		i--;
		if(buffer[i] == '\n'){
			if(write_to_arr) {
				keys[k++] = next_int;
			} else {
				insert(values, next_int);
			}	
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

	if(write_to_arr) {
		keys[k++] = next_int;
	} else {
		insert(values, next_int);
	}

	// Based on remainder, fill rem array
	// ////////////////
	i = buffer_size-1;
	while(buffer[i] != '\n') {
		rem[l++] = buffer[i--];
	}
	rem_SIZE = l; //l is incremented already
}

void read_file(char *filename, bool write_to_arr){
	FILE *fs;
	char *buffer;

	if(!(fs = fopen(filename, "rb"))){
		fprintf(stderr, "error encounted while opening the file\n");
	}

	/* Get the number of bytes */
	fseek(fs, 0L, SEEK_END);
	long file_size = ftell(fs);
	fseek(fs, 0L, SEEK_SET);

	//Buffer determined by file size. Remainder bytes also assigned below.

	// long buffer_size = file_size/CHUNK_SIZE;
	int num_chunk = file_size / BUFFER_SIZE;
	long remainder_size = file_size % BUFFER_SIZE; 
	//long remainder_size = file_size % CHUNK_SIZE; 

	buffer = malloc(BUFFER_SIZE);

	for(long i = 0; i < num_chunk; i++){
		if(fread(buffer, BUFFER_SIZE, 1, fs) == 1) {
			convert(buffer, BUFFER_SIZE, write_to_arr);
		} else {
			printf("Gone wrong buddy\n");
		}
	}

	//Read the rest of the file.
    free(buffer);
    buffer = malloc(remainder_size);
    if (fread(buffer, remainder_size, 1, fs) == 1){ 

    	convert(buffer, remainder_size, write_to_arr);
    }

    free(buffer);
    fclose(fs);
}

int main(int argc, char *argv[]){
	char values_filename[128], keys_filename[128];

	keys = malloc(ARRAY_SIZE * sizeof(long));
	values = createTable(HASH_SIZE);

	if(argc != 3){
		fprintf(stderr, "enter both filenames, please\n");
		return -1;
	} else {
		strcpy(values_filename, argv[1]);
		strcpy(keys_filename, argv[2]);
	}

	k = 0;

	rem_SIZE = 0;
	read_file(values_filename, false);//False means that we are not writing in an array.(hashmap instead)

	rem_SIZE = 0;
	read_file(keys_filename, true);

	long next_key;
    for(long i = 0; i < k; i++){
    	next_key = keys[i];
    	if (search(values, next_key) != -1 ) {
			printf("%ld\n", next_key);
		}	
    }

 	free(keys);
	free(values);
}
