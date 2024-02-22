#include"mymalloc.h"
#include<string.h>
#include<stdio.h>
#define MEMLENGTH 512 //was 512
static double memory[MEMLENGTH];
enum ERROR_CODES {
    ZERO_SIZE,
    NULL_POINTER,
    ISFREE,
    OUTOFRANGE,
    NOSPACE,
    INVALID_HEADER
};
void PRINTERR(enum ERROR_CODES error,char* file, int line){
    switch(error){
        case ZERO_SIZE:
            fprintf(stderr,"Cannot allocate zero bytes: file %s, line %d\n",file,line);
            break;
        case NULL_POINTER:
            fprintf(stderr,"Cannot free Null Pointer: file %s, line %d\n",file,line);
            break;
        case ISFREE:
            fprintf(stderr,"Cannot free already freed pointer: file %s, line %d\n",file,line);
            break;
        case OUTOFRANGE:
            fprintf(stderr,"memory not allocated with malloc: file %s, line %d\n",file,line);
            break;
        case NOSPACE:
            fprintf(stderr,"space not found on heap, please free memory\n");
            break;
        case INVALID_HEADER:
            fprintf(stderr,"Unable to free, invalid header data: file %s, line %d\n",file,line);
            break;
        default:
            fprintf(stderr,"unknown error: file %s, line %d\n",file,line);
            break;
    }
}

//helper functions for metadata
static size_t Get_size(void* ptr){
    size_t getchanges = 7;
    getchanges = ~getchanges;
    block_t* dataptr = (block_t*)ptr;
    return dataptr->size & getchanges;
}
static void* Get_next(void* ptr){
    size_t size = Get_size(ptr);
    return ((char*)ptr) + (sizeof(size_t) + size);
}
static char Isfree(void* ptr){
    return (((block_t*)ptr)->size & 7) == 0;
}
static void Setfreebit(void* ptr, size_t freebit){
    block_t* dataptr = (block_t*)ptr;
    dataptr->size &= -8;
    dataptr->size |= freebit;
}
static void* Firstfit(size_t size){
    for(block_t* ptr = (block_t*)&memory[0]; ptr < (block_t*)&memory[MEMLENGTH]; ptr = Get_next(ptr)){
        if(Isfree(ptr) && Get_size(ptr) >= size){
            return ptr;
        }
    }
    return NULL;
}
static char isValidheader(void* ptr){
    void* i = (void*)&memory[0];
    while(i < ptr && i < (void*)&memory[MEMLENGTH]){
        i = Get_next(i);
    }
    return i == ptr;
}
//troubleshooting to print addresses of information
static void iteratememory(){
    for(block_t* ptr = (block_t*)&memory[0]; ptr < (block_t*)&memory[MEMLENGTH]; ptr = Get_next(ptr)){
        printf("size: %ld\tisFree:%c\tlocation_in_mem:%p\taddressofbuffer:%p\n",Get_size(ptr),Isfree(ptr)?'Y':'N',ptr,ptr->buf);
    }
    printf("address of border: %p\n",&memory[MEMLENGTH]);
}
void clienttrouble(){
    iteratememory();
}
static void coalesce(void* ptr) {
    block_t* HeaderA = ptr;
    block_t* HeaderB = Get_next(ptr);

    while ((void*)HeaderB < (void*)&memory[MEMLENGTH]) {
        // printf("%p < %p \n",(void*)HeaderB,(void*)&memory[MEMLENGTH]);
        if (Isfree(HeaderA) && Isfree(HeaderB)) { // | FREE | FREE|
            HeaderA->size += sizeof(block_t) + HeaderB->size;
            HeaderB = Get_next(HeaderB);
        } else {
            HeaderA = HeaderB;
            HeaderB = Get_next(HeaderB);
        }
    }
}

//main functions
void* mymalloc(size_t size, char *file, int line){
    size = (size + 7) & -8;
    if(size == 0){
        PRINTERR(ZERO_SIZE,file,line);
        return NULL;
    }

    //check if heap is not initialized
    if(memory[0] == 0){
        block_t header;
        header.size = sizeof(memory) - sizeof(header);
        Setfreebit(&header,0);
        memcpy(memory,&header,sizeof(block_t));
    }
    
    void* destination = Firstfit(size);
    if(destination == NULL){
        PRINTERR(NOSPACE,file,line);
        return NULL;
    }
    //variables from old header
    block_t* oldheader = (block_t*)destination;
    size_t oldsize = oldheader->size;
    oldheader->size = size | 1; //kind of implied that space is being used.
    block_t newheader;
    newheader.size = oldsize - sizeof(block_t) - size;
    void* newaddress = Get_next(oldheader);
    if(newaddress < (void*)&memory[MEMLENGTH]){
        void* final_address = memcpy(newaddress,&newheader,sizeof(block_t));
    }
    // iteratememory(); for troubleshooting purposes
    return oldheader->buf;
}
void myfree(void *ptr, char *file, int line){
    if(ptr == NULL){
        PRINTERR(NULL_POINTER,file,line);
        return;
    }
    //memory not in heap
    if(ptr < (void*)&memory[0] || ptr >= (void*)&memory[MEMLENGTH]){
        PRINTERR(OUTOFRANGE,file,line);
        return;
    }
    if(!isValidheader(ptr - sizeof(size_t))){
        PRINTERR(INVALID_HEADER,file,line);
        return;
    }
    block_t* address = (block_t*)(((char*)ptr) - sizeof(size_t));
    if(Isfree(address)){
        PRINTERR(ISFREE,file,line);
        return;
    }
    Setfreebit(address,0);
    coalesce(&memory[0]);
    return;
}