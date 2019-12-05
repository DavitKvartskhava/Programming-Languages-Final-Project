#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

const int ARRAY_SIZE = 2000000;
// const int BUFFER_SIZE = 1024*1024;
const int CHUNK_SIZE = 10;

/////////////////////////////////
////////HASHMAP//////////////////
struct node{
    long long key;
    int val;
    struct node *next;
};
struct table{
    int size;
    struct node **list;
};
struct table *createTable(int size){
    struct table *t = (struct table*)malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}
int hashCode(struct table *t,int key){
    if(key<0)
        return -(key%t->size);
    return key%t->size;
}
void insert(struct table *t,int key,int val){
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
int search(struct table *t,int key){
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
/////////////////////////////////
/////////////////////////////////

int int_cmp(const void *a, const void *b){
	const long long *int_a = (const long long *)a;
	const long long *int_b = (const long long *)b;
	
	return *int_a - *int_b;
}

char rem[20];
int rem_SIZE;
// int k;

void convert(struct table *t, char *buffer, int buffer_size){
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
			insert(t, next_int, 1);
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
	insert(t, next_int, 1);

	// ////////////////
	// Based on remainder, fill rem array
	// ////////////////
	i = buffer_size-1;
	while(buffer[i] != '\n') {
		rem[l++] = buffer[i--];
	}
	rem_SIZE = l; //l is incremented already
}

void read_file(struct table *t, char *filename){
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
			convert(t, buffer, buffer_size);
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
    	convert(t, buffer, remainder_size);
    }

    fclose(fs);
}

// long long *values, *keys;

int main(int argc, char *argv[]){
	char values_filename[128], keys_filename[128];
	// values = malloc(ARRAY_SIZE * sizeof(long long));
	// keys = malloc(ARRAY_SIZE * sizeof(long long));

	struct table *values = createTable(ARRAY_SIZE);

	if(argc != 3){
		fprintf(stderr, "enter both filenames, please\n");
		return -1;
	} else {
		strcpy(values_filename, argv[1]);
		strcpy(keys_filename, argv[2]);
	}
	// k = 0;
	rem_SIZE = 0;
	read_file(values, values_filename);
	// int line_count_values = k;

	// k=0;
	// rem_SIZE = 0;
	// read_file(keys, keys_filename);
	// int line_count_keys = k;


    // for(int i = 0; i < k; i++){
    // 	printf("%lld\n",keys[i]);
    // }

	// printf("HELLO!!!%d", line_count_values);
	// printf("HELLO!!!%d", line_count_keys);

	// qsort(values, line_count_values, sizeof(long long), int_cmp);

	// int *index;
	// for(int i = 0; i < line_count_keys; i++){
	// 	// index = bsearch(&keys[i], values, line_count_values, sizeof(long long), int_cmp); // Maybe ARRAY_SIZE -> line_count_values

	FILE *fptr;
    long next_key;


	if(!(fptr = fopen(keys_filename, "r"))){
		fprintf(stderr, "error encounted while opening the file\n");
	}

	while(fscanf(fptr, "%ld", &next_key) != EOF){
		if (search(values, next_key) != -1 ) {
			printf("%ld\n", next_key);
		}
	}
	fclose(fptr);
	// 	if (index!=NULL){	

	// 		printf("%d\n", *index);
	// 	}
	// }
}
