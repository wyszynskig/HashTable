/* Hash table by Grzegorz Wyszynski
 * Python interface
 * email: wyszynskig@gmail.com           */
/* more info on python extension: https://docs.python.org/3.4/extending/newtypes.html */
#include <Python.h>
#include <structmember.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <Python.h>
#define ERROR_HANDLE(command) printf("%i - %i\n",command,__LINE__);


/* -------------------------------------------------- type definitions */
struct list_s{ /* one-directional list */
	char* index; /* "index" of the entry */
	char* value; /* value */
	struct list_s* next; /* next value */
};

typedef struct list_s list;

typedef struct{ /* hashtable struct */
	int size; /* number of "baskets" (may be smaller than number of elements!) */
	list** basket; /* content of "basket" */
}hashtable_HashTableObject;

typedef struct hashtable_s hashtable;
/* Members of HashTable object */
static PyMemberDef HashTable_members[] = {
    {"size", T_OBJECT_EX, offsetof(hashtable, size), 0,
     "number of baskets"},
    {NULL}  /* Sentinel */
};

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
static PyObject *
HashTable_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	int nBasket;
	hashtable * output= NULL;
	if( size < 1 ) return NULL;
	
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

	return (PyObject *)output;	
}


/* Destroy hashtable */
static void DestroyHashtable(hashtable * input)
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

int AddValueToHashtable(hashtable * self, PyObject *args)
{
	int hashvalue = GetStringHash(index, table->size); /* Hash for "value" string */
	list * current;
	static char *kwlist[] = {"index", "value", NULL};
	char * index;
	char * value;
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "ss", kwlist,
                                      &index, &value))
		return -1;
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
	return 1;
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

PyObject * GetValue(hashtable * self, PyObject *args)
{
	char * index;
	static char *kwlist[] = {"index", NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist,&index))
		return -1;
	int hashvalue = GetStringHash(index, table->size); /* Hash for "value" string */
	return GetValueFromEntry(index,table->basket[hashvalue]);
}

PyObject * ReturnAll(hashtable * self)
{
	int nBasket;
	list * currEntry;
	PyObject * output = PyListNew(0);
	for(nBasket=0;nBasket < table->size; nBasket++)
	{
		currEntry=table->basket[nBasket]; /* Now we work with nBasket-th basket */
		while (currEntry!=NULL)
		{
			PyList_Append(output,PyBuildValue((ss),currEntry->index,currEntry->value));
			currEntry=currEntry->next;
		}
	}
	return output;
}

static PyMethodDef hashtable_methods[] = {
    {"returnall", (PyCFunction)ReturnAll,  METH_VARARGS,
     "Return the name, combining the first and last name"
    },
    {"append", (PyCFunction)AddValueToHashtable, METH_VARARGS, 
    "Add the value with index to Hash Table"
    },
    {"get", (PyCFunction)GetValue, METH_VARARGS, 
    "Get the value corresponding to index from Hash Table"
    },
    {"getAll", (PyCFunction)ReturnAll, METH_NOARGS, 
    "Get all the value with corresponding indices from Hash Table"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject NoddyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "hashtable.HashTable",             /* tp_name */
    sizeof(hashtable),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)DestroyHashtable, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT         /* tp_flags */
    "Noddy objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    hashtable_methods,             /* tp_methods */
    HashTable_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,      /* tp_init */
    0,                         /* tp_alloc */
    HashTable_new,             /* tp_new */
};

static PyModuleDef noddy2module = {
    PyModuleDef_HEAD_INIT,
    "noddy2",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};


PyMODINIT_FUNC
PyInit_noddy2(void)
{
    PyObject* m;

    if (PyType_Ready(&NoddyType) < 0)
        return NULL;

    m = PyModule_Create(&noddy2module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&NoddyType);
    PyModule_AddObject(m, "Noddy", (PyObject *)&NoddyType);
    return m;
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