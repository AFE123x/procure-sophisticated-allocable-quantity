#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){
    int length = atoi(argv[1]);
    for(int i = 0; i < length; i++){
        printf("%lu\n",(rand() % atoi(argv[2])));
    }
    return 0;
}
