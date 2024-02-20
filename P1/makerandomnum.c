#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){
    printf("int myarr[] = {");
    int length = atoi(argv[1]);
    for(int i = 0; i < length; i++){
        printf("%lu,",(rand() % atoi(argv[2])));
    }
    printf("};");
    return 0;
}
