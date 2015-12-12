/* Hash table by Grzegorz Wyszynski
 * Stand-alone C-only tool version
 * email: wyszynskig@gmail.com           */



#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#define ERROR_HANDLE(command) printf("%i - %i\n",command,__LINE__);
/* -------------------------------------------------- type definitions */
struct list_s{ /* one-directional list */
	char* index; /* "index" of the entry */
	char* value; /* value */
	struct list_s* next; /* next value */
};

typedef struct list_s list;

struct hashtable_s{ /* hashtable struct */
	int size; /* number of "baskets" (may be smaller than number of elements!) */
	list** basket; /* content of "basket" */
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
	printf("%i\n",output % sizeOfHashtable);
	return output % sizeOfHashtable;
}
/* Initialize Hashtable */
hashtable * InitializeHashtable(int size)
{
	int nBasket;
	if( size < 1 ) return NULL;
	hashtable * output= NULL;
	
	/* Allocate the hashtable */
	if( ( output = malloc( sizeof( hashtable ) ) ) == NULL ) { /* error-proofing */
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if( ( output->basket = malloc( sizeof( list * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( nBasket = 0; nBasket < size; nBasket++ ) {
		output->basket[nBasket] = NULL;
	}

	output->size = size;

	return output;	
}

/* Destroy hashtable */
void DestroyHashtable(hashtable * input)
{
	int nBasket;
	list* currEntry;
	list * tempEntry;
	/* destroy all entries */
	for (nBasket=0;nBasket<input->size;nBasket++)
	{
		currEntry=input->basket[nBasket];
		while ((tempEntry = currEntry) != NULL) { /* adding temporary entry variable to store current element */
			currEntry = currEntry->next;         /* while the loop variable proceeds */
			free(tempEntry);                     /* we free the previous value */
		}
		
	}
	free(input);
	return;
}

int AddValueToEntry(char * index, char * value, list * basket)
{
	while(basket!=NULL)
		if (!strcmp(basket->index,index)) /* If the index does not much current entry */
		{
			basket=basket->next;      /* Proceed through the list until we find the last entry */
		}
		else						       /* Otherwise update the value */
		{
			free(basket->value);
			basket->value = strdup(value);
			return 1;
		}
	if ((basket = malloc(sizeof(list))) == NULL) /* Allocate the */
		return 0;
	basket->index = strdup(index);  /* Adding index */
	basket->value = strdup(value);  /* And value */
	basket->next = NULL;	
	return 1;	
}

int AddValueToHashtable(char * index, char * value, hashtable * table)
{
	int hashvalue = GetStringHash(index, table->size); /* Hash for "value" string */
	list * current;
	if (table->basket[hashvalue]==NULL)
	{
		if ((table->basket[hashvalue]=malloc(sizeof(list)))==NULL) return 0;
		table->basket[hashvalue]->index = strdup(index);
		table->basket[hashvalue]->value = strdup(value);
		table->basket[hashvalue]->next  = NULL;
	}
	else
	{
		current = table->basket[hashvalue];
		while (current->next!=NULL)
		{
			current=current->next;
		}
		if ((current->next=malloc(sizeof(list)))==NULL) return 0;
		current=current->next;
		current->index = strdup(index);
		current->value = strdup(value);
		current->next  = NULL;
	}
	return AddValueToEntry(index,value,table->basket[hashvalue]);
}

char * GetValueFromEntry(char * index, list * basket)
{
	list * currEntry = basket;
	while (currEntry!=NULL)
	{
		if(strcmp(index,currEntry->index)) /* If indexes are equal */
			return currEntry->value;
		currEntry=currEntry->next;
	}
	return NULL;
}

char * GetValueFromHashtable(char * index, hashtable * table)
{
	int hashvalue = GetStringHash(index, table->size); /* Hash for "value" string */
	return GetValueFromEntry(index,table->basket[hashvalue]);
}

void PrintAll(hashtable * table)
{
	int nBasket;
	list * currEntry;
	for(nBasket=0;nBasket < table->size; nBasket++)
	{
		printf("nbasket: %i\n",nBasket);
		currEntry=table->basket[nBasket]; /* Now we work with nBasket-th basket */
		while (currEntry!=NULL)
		{
			printf("%s\t%s\n",currEntry->index,currEntry->value);
			currEntry=currEntry->next;
		}
	}
}

int main()
{
	hashtable * table=InitializeHashtable(2);
	ERROR_HANDLE(AddValueToHashtable("a","bbb",table))
	PrintAll(table);
	ERROR_HANDLE(AddValueToHashtable("c","eee",table))
	PrintAll(table);
	ERROR_HANDLE(AddValueToHashtable("d","ddd",table))
	PrintAll(table);
	printf("%s\n",table->basket[0]->value);
	DestroyHashtable(table);
	return 0;
}