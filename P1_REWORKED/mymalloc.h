#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include<stddef.h>
#define malloc(s)   mymalloc(s, __FILE__, __LINE__)
#define free(p)     myfree(p, __FILE__, __LINE__)

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void clienttrouble();
typedef struct metadata{
    size_t size;
    char buf[0];
}block_t;
#endif
