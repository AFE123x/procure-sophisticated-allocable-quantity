#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
// real one we need void *mymalloc(size_t size, char *file, int line);
void *mymalloc(long size, char *file, int line);
void myfree(void *ptr, char *file, int line);

typedef struct Block {
    int isallocated;
    int size;
    struct Block *prev;
    struct Block *next;
} Block;

#endif