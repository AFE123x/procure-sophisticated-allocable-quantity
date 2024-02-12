#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include<stddef.h>
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
// real one we need void *mymalloc(size_t size, char *file, int line);
void* bestfit();
void* mymalloc(size_t size, char *file, int line);
void  myfree(void *ptr, char *file, int line);

typedef struct Block {
    size_t metadata;
    unsigned char payload[0];
} Block;

#endif
