#include <stdio.h>
#include "mymalloc.h"
int main(int argc, char** argv){
    for(int i = 1; i < 50; i++){
        int* ptr = malloc(sizeof(int) * i);
        printf("address of ptr: %p\n",ptr);
    }
    
    return 0;
} 
