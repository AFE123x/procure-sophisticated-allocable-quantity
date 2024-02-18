#include <stdio.h>
#include "mymalloc.h"
#define REMREMREM 30
int main(int argc, char** argv){
    int* ptr = (int*)malloc(sizeof(int));
    *ptr = 0x01020304;
    printf("at address %lu, we have %d\n",ptr,*ptr);
    int* ptr2 = (int*)malloc(sizeof(int) * REMREMREM);
    for(int i = 0; i < REMREMREM; i++){
        ptr2[i] = i;
    }
    printf("at address %lu, we have %d\n",ptr2,*ptr2);
    free(ptr);
    free(ptr2);
    char* newptr = (char*)malloc(sizeof(char) * REMREMREM);
    for(int i = 0; i < REMREMREM; i++){
        newptr[i] = i;
    }
    printf("at address %lu, we have %c\n",newptr + 15 ,newptr[15]);
    return 0;
} 
