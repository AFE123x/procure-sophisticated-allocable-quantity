#include <stdio.h>
#include "mymalloc.h"
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#define REMREMREM 30
/*
Test 1: malloc() and immediately free() a 1-byte object, 120 times.

Test 2: Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to
deallocate the chunks.

Test 3: Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte
object and adding the pointer to the array and deallocating a previously allocated object (if
any), until you have allocated 120 times. Deallocate any remaining objects

Test 4: Two more stress tests of your design. Document these in your README.

*/

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


    test1();
    test2();
    test3();
    test4();
    test5();

    return 0;

} 

float test1() {
// Test 1: malloc() and immediately free() a 1-byte object, 120 times.
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for(int i = 0; i < 120; i++){
        char* ptr = (char*)malloc(sizeof(char));
        free(ptr);
    }
    gettimeofday(&end, NULL);
    float sum1=(end.tv_sec - start.tv_sec) * 10000000 ;
	float sum2=(end.tv_usec - start.tv_usec) * 0.000001;
    printf("Time taken for test 1: %f\n",sum1+sum2);
    return 0;

}
float test2() {
// 2. Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
    struct timeval start, end;
    gettimeofday(&start, NULL);

    char* ptrs[120];
    for(int i = 0; i < 120; i++){
        ptrs[i] = (char*)malloc(sizeof(char));
    }
    for(int i = 0; i < 120; i++){
        free(ptrs[i]);
    }
    gettimeofday(&end, NULL);
    float sum1=(end.tv_sec - start.tv_sec) * 10000000 ;
    float sum2=(end.tv_usec - start.tv_usec) * 0.000001;
    printf("Time taken for test 2: %f\n",sum1+sum2);
    return 0;

}
float test3() {
/*3. Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte
object and adding the pointer to the array and deallocating a previously allocated object (if
any), until you have allocated 120 times. Deallocate any remaining objects.*/

    struct timeval start, end;
    gettimeofday(&start, NULL);

    char* ptrs[120];
    for(int i = 0; i < 120; i++){
        ptrs[i] = NULL;
    }
    int count = 0;
    while(count < 120){
        int choice = rand() % 2;
        if(choice == 0){
            for(int i = 0; i < 120; i++){
                if(ptrs[i] == NULL){
                    ptrs[i] = (char*)malloc(sizeof(char));
                    count++;
                    break;
                }
            }
        }else{
            for(int i = 0; i < 120; i++){
                if(ptrs[i] != NULL){
                    free(ptrs[i]);
                    ptrs[i] = NULL;
                    count++;
                    break;
                }
            }
        }
    }
    for(int i = 0; i < 120; i++){
        if(ptrs[i] != NULL){
            free(ptrs[i]);
        }
    }
    gettimeofday(&end, NULL);
    float sum1=(end.tv_sec - start.tv_sec) * 10000000 ;
    float sum2=(end.tv_usec - start.tv_usec) * 0.000001;
    printf("Time taken for test 3: %f\n",sum1+sum2);
    return 0;

}
float test4() {
//
    struct timeval start, end;
    gettimeofday(&start, NULL);
////////////////////////////////

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



////////////////////////////////
    gettimeofday(&end, NULL);
    float sum1=(end.tv_sec - start.tv_sec) * 10000000 ;
    float sum2=(end.tv_usec - start.tv_usec) * 0.000001;
    printf("Time taken for test 4: %f\n",sum1+sum2);


}
float test5() {
// 
    struct timeval start, end;
    gettimeofday(&start, NULL);
////////////////////////////////

    char* ptr1 = (char*)malloc(10);
    if (ptr1 == NULL) {
        printf("Error: Failed to allocate memory\n");
        return 1;
    }

    char* ptr2 = (char*)malloc(20);
    if (ptr2 == NULL) {
        printf("Error: Failed to allocate memory\n");
        return 1;
    }

    // Use the allocated memory
    for (int i = 0; i < 10; ++i) {
        ptr1[i] = 'A' + i;
    }

    for (int i = 0; i < 20; ++i) {
        ptr2[i] = 'a' + i;
    }

    // Free the allocated memory
    free(ptr1);
    free(ptr2);

////////////////////////////////
    gettimeofday(&end, NULL);
    float sum1=(end.tv_sec - start.tv_sec) * 10000000 ;
    float sum2=(end.tv_usec - start.tv_usec) * 0.000001;
    printf("Time taken for test 5: %f\n",sum1+sum2);

}

