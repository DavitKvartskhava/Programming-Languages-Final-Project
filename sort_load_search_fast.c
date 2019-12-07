#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include <stdbool.h>

const int ARRAY_SIZE = 1000000000;
const int HASH_SIZE = 1300000000;
const int BUFFER_SIZE = 10000;
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
    int val;
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

void insert(struct table *t,long key,int val){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    newNode->val = val;
    newNode->next = list;
    t->list[pos] = newNode;
}
int search(struct table *t, long key){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////

void convert(char *buffer, int buffer_size, bool write_to_arr){
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
			if(write_to_arr) {
				keys[k++] = next_int;
			} else {
				insert(values, next_int, 1);
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
		insert(values, next_int, 1);
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
	long buffer_size = BUFFER_SIZE;
	num_chunk = file_size / buffer_size;
	long remainder_size = file_size % buffer_size; 
	//long remainder_size = file_size % CHUNK_SIZE; 

	buffer = malloc(buffer_size);

	for(long i = 0; i < num_chunkm; i++){
		if(fread(buffer, buffer_size, 1, fs) == 1) {
			convert(buffer, buffer_size, write_to_arr);
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
	read_file(values_filename, false);

	rem_SIZE = 0;
	read_file(keys_filename, true);

	long next_key;
    for(long i = 0; i < k; i++){
    	next_key = keys[i];
    	if (search(values, next_key) != -1 ) {
			printf("%lld\n", next_key);
		}	
    }

}
