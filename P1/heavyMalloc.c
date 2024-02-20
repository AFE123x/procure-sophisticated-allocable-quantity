#include<stdio.h>
#include "mymalloc.h"

//make a main
int main(int argc, char** argv){
//malloc() and immediately free() a 1-byte object, 120 times
    for(int i = 0; i < 120; i++){
        int* tommy = malloc(sizeof(int));
        free(tommy);
    }


return 69;
}