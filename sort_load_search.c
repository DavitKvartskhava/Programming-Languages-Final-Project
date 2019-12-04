#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

const int ARRAY_SIZE = 1000005;


/////////////////////////////////
////////HASHMAP//////////////////
struct node{
    long key;
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
	const int *int_a = (const int *)a;
	const int *int_b = (const int *)b;
	
	return *int_a - *int_b;
}

void read_file(struct table *t, char *filename){
	FILE *fptr;
    long next_value;

	if(!(fptr = fopen(filename, "r"))){
		fprintf(stderr, "error encounted while opening the file\n");
	}

	while(fscanf(fptr, "%ld", &next_value) != EOF){
		insert(t, next_value, 1);
	}
	fclose(fptr);
}
//long long *values, *keys;


int main(int argc, char *argv[]){
	char values_filename[128], keys_filename[128];
	struct table *values = createTable(ARRAY_SIZE);

	// keys = malloc(ARRAY_SIZE * sizeof(long long));

	if(argc != 3){
		fprintf(stderr, "enter both filenames, please\n");
		return -1;
	} else {
		strcpy(values_filename, argv[1]);
		strcpy(keys_filename, argv[2]);
	}

	read_file(values, values_filename);
	// // printf("HELLO!!!%d", line_count_values);
	// // printf("HELLO!!!%d", line_count_keys);

	// qsort(values, ARRAY_SIZE, sizeof(long long), int_cmp);


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

}
