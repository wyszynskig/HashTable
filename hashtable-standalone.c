/* Hash table by Grzegorz Wyszynski
 * Stand-alone C-only tool version
 * email: wyszynskig@gmail.com           */



#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

/* -------------------------------------------------- type definitions */
struct list_s{ /* one-directional list */
	char* index; /* "index" of the entry */
	char* value; /* value */
	struct list_s* next; /* next value */
};

typedef struct list_s list;

struct hashtable_s{ /* hashtable struct */
	int size; /* number of "buckets" (may be smaller than number of elements!) */
	list** entries; /* content of "bucket" */
};

typedef struct hashtable_s hashtable;
/* -------------------------------------------------- end of type definitions */

/* Get value of */
int GetStringHash(char* value, int sizeOfHashtable)
{
	int output=0; /* hash of string "value" */
	int i;
	for(i=0;i<strlen(value);i++)
	{
		printf("%c\n",value[i]);
		output+=value[i];
	}
	return output % sizeOfHashtable;
}
/* Initialize Hashtable */
hashtable * InitializeHashtable(int size)
{
	int nBucket;
	if( size < 1 ) return NULL;
	hashtable * output= NULL;
	
	/* Allocate the hashtable */
	if( ( output = malloc( sizeof( hashtable ) ) ) == NULL ) { /* error-proofing */
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if( ( output->entries = malloc( sizeof( list * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( nBucket = 0; nBucket < size; nBucket++ ) {
		output->entries[nBucket] = NULL;
	}

	output->size = size;

	return output;	
}

/* Destroy hashtable */
int DestroyHashtable(hashtable * input)
{
	int nBucket;
	/* destroy all entries */
	for (nBucket=0;nBucket<size;nBucket++)
	{
		currEntry=entries[nBucket]
		while ((tempEntry = currEntry) != NULL) { /* adding temporary entry variable to store current element */
			currEntry = currEntry->next;         /* while the loop variable proceeds */
			free (tempEntry);                    /* we free the previous value */
		}
		
	}
	
	
	
	
	
}


int main()
{
	char entry[100];
	return 0;
}