#include <stdlib.h>
#include "mymalloc.h"
#include<stdio.h>

int main () { 
// int x = 69;
// char* ZERO = malloc(0);
// char* N = malloc(NULL);
// char* allo = malloc(1);
// free(allo);
// free(allo);
// free(allo+1);
// free(x);
// char* nospace = malloc(-1);


char* ptrscollection[256];

for (int i = 0; i < 256; i++) {
    ptrscollection[i] = malloc(1);
}
free(ptrscollection[200]);
free(ptrscollection[201]);
free(ptrscollection[203]);
free(ptrscollection[204]);
free(ptrscollection[205]);
char* LETMEBACKIN = malloc(32);
// char* test2 = malloc(1);
// char* test3 = malloc(1);
// char* LETMEBACKIN4 = malloc(1);
// char* LETMEBACKIN5 = malloc(1); 
printf("LETS GO BOYS");






return 0;
}