#include "mymalloc.h"
#include<stdio.h>
int main(int argc, char** argv){
    int i = 0;
    char* myptr[100000];
    while(1){
        myptr[i++] = (char*)malloc(sizeof(char));
        if(myptr[i - 1] == NULL){
            break;
        }
    }
    for(int x = 0; x < i; x++){
        free(myptr[x]);
    }
    clienttrouble();
}