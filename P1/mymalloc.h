#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include<stddef.h>
#include<stdbool.h>
#include<stdio.h> //remove after
#include<string.h>
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
// real one we need void *mymalloc(size_t size, char *file, int line);
void* mymalloc(size_t size, char *file, int line);
void  myfree(void *ptr, char *file, int line);

typedef struct block{
 size_t size;
 bool isFree;
 struct block* next;
 struct block* prev;
}block_t;
#endif
