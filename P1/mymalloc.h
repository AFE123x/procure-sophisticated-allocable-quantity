#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include<stddef.h>
#include<stdio.h> //remove after
#include<string.h>
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
// real one we need void *mymalloc(size_t size, char *file, int line);
void* mymalloc(size_t size, char *file, int line);
void  myfree(void *ptr, char *file, int line);

/**
 * Our struct data.
 * size contains an 8 byte size.
 * isFree char to indicate whether memory is free.
 * next is the reference to next node.
 * reference to previous node. 
 */
typedef struct block{
 size_t size;
 char isFree;
 struct block* next;
 struct block* prev;
}block_t;
#endif
