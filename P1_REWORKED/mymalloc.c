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
/**
 * You want to print an error to stderr.
 * ZERO_SIZE: if client tries to allocate 0 bytes
*/
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

/**
 * Get's the size of the header metadata.
 * 
 * The most significant 61 bits represent the size of our data.
 * 
 * @param ptr void* Pointer to the memory block.
 * @return size_t Size of the memory block.
 */
static size_t Get_size(void* ptr){
    size_t getchanges = 7;
    getchanges = ~getchanges;
    block_t* dataptr = (block_t*)ptr;
    return dataptr->size & getchanges;
}

/**
 * Finds the address of the next header.
 * 
 * @param ptr void* Pointer to the current memory block.
 * @return void* Address of the next memory block header.
 */
static void* Get_next(void* ptr){
    size_t size = Get_size(ptr);
    return ((char*)ptr) + (sizeof(size_t) + size);
}

/**
 * Checks if the bit in header metadata indicates the block is free.
 * 
 * @param ptr void* Pointer to the memory block.
 * @return char Returns '1' if the memory block is free, '0' otherwise.
 */
static char Isfree(void* ptr){
    return (((block_t*)ptr)->size & 7) == 0;
}

/**
 * Sets the isfree bit to 1 or 0 in the header metadata.
 * 
 * @param ptr void* Pointer to the memory block.
 * @param freebit size_t '1' to mark the block as free, '0' otherwise.
 */
static void Setfreebit(void* ptr, size_t freebit){
    block_t* dataptr = (block_t*)ptr;
    dataptr->size &= -8;
    dataptr->size |= freebit;
}

/**
 * Implements first fit approach to finding available memory space.
 * 
 * @param size size_t Size of the memory block to be allocated.
 * @return void* Address of the first available header that fits the given size.
 * @return NULL If first fit fails to find an available space.
 */
static void* Firstfit(size_t size){
    for(block_t* ptr = (block_t*)&memory[0]; ptr < (block_t*)&memory[MEMLENGTH]; ptr = Get_next(ptr)){
        if(Isfree(ptr) && Get_size(ptr) >= size){
            return ptr;
        }
    }
    return NULL;
}

/**
 * Checks if the given pointer has a valid header.
 * 
 * @param ptr void* Pointer to the memory block.
 * @returns char '1' if ptr has a valid header, '0' otherwise.
 */
static char isValidheader(void* ptr){
    void* i = (void*)&memory[0];
    while(i < ptr && i < (void*)&memory[MEMLENGTH]){
        i = Get_next(i);
    }
    return i == ptr;
}

/**
 * Iterates through the memory blocks and prints their metadata.
 * 
 * Prints the size, isFree, location, and location of the buffer for each block.
 */
static void iteratememory(){
    for(block_t* ptr = (block_t*)&memory[0]; ptr < (block_t*)&memory[MEMLENGTH]; ptr = Get_next(ptr)){
        printf("size: %ld\tisFree:%c\tlocation_in_mem:%p\taddressofbuffer:%p\n", Get_size(ptr), Isfree(ptr) ? 'Y' : 'N', ptr, ptr->buf);
    }
    printf("address of border: %p\n", &memory[MEMLENGTH]);
}

/**
 * Allows the client to visualize memory by calling iteratememory().
 */
void clienttrouble(){
    iteratememory();
}

/**
 * Coalesces adjacent free memory blocks.
 * 
 * Merges adjacent free memory blocks by updating their headers.
 * 
 * @param ptr void* Pointer to the memory block.
 */
static void coalesce(void* ptr) {
    block_t* HeaderA = ptr;
    block_t* HeaderB = Get_next(ptr);

    while ((void*)HeaderB < (void*)&memory[MEMLENGTH]) {
        if (Isfree(HeaderA) && Isfree(HeaderB)) {
            HeaderA->size += sizeof(block_t) + HeaderB->size;
            HeaderB = Get_next(HeaderB);
        } else {
            HeaderA = HeaderB;
            HeaderB = Get_next(HeaderB);
        }
    }
}

/**
 * Implementation of malloc, which allocates memory from the array.
 * 
 * Allocates memory from the memory array and returns the address of the allocated space.
 * 
 * @param size size_t Size of the memory block to be allocated.
 * @param file char* Name of the source file.
 * @param line int Line number in the source file.
 * @return void* Address of the newly allocated space if malloc finds it, NULL otherwise.
 */
void* mymalloc(size_t size, char* file, int line){
    size = (size + 7) & -8; // Align size to 8-byte boundary
    if(size == 0){
        PRINTERR(ZERO_SIZE,file,line);
        return NULL;
    }

    // Check if heap is not initialized
    if(memory[0] == 0){
        block_t header;
        header.size = sizeof(memory) - sizeof(header);
        Setfreebit(&header, 0); // Mark the block as free
        memcpy(memory, &header, sizeof(block_t));
    }
    
    void* destination = Firstfit(size); // Find the first fit block
    if(destination == NULL){
        PRINTERR(NOSPACE,file,line);
        return NULL;
    }

    // Variables from old header
    block_t* oldheader = (block_t*)destination;
    size_t oldsize = oldheader->size;
    oldheader->size = size | 1; // Mark the block as used

    // Create a new header for the remaining free space
    block_t newheader;
    newheader.size = oldsize - sizeof(block_t) - size;
    void* newaddress = Get_next(oldheader); // Get the address for the new header

    if(newaddress < (void*)&memory[MEMLENGTH]){
        void* final_address = memcpy(newaddress, &newheader, sizeof(block_t)); // Copy the new header to memory
    }
    return oldheader->buf; // Return the address of the buffer
}

/**
 * Frees up memory in the heap.
 * 
 * @param ptr void* Pointer to the memory block to be freed.
 * @param file char* Name of the source file.
 * @param line int Line number in the source file.
 */
void myfree(void* ptr, char* file, int line){
    if(ptr == NULL){
        PRINTERR(NULL_POINTER, file, line);
        return;
    }
    // Check if memory is out of bounds
    if(ptr < (void*)&memory[0] || ptr >= (void*)&memory[MEMLENGTH]){
        PRINTERR(OUTOFRANGE, file, line);
        return;
    }
    // Check if the header is valid
    if(!isValidheader(ptr - sizeof(size_t))){
        PRINTERR(INVALID_HEADER, file, line);
        return;
    }
    block_t* address = (block_t*)(((char*)ptr) - sizeof(size_t)); // Get the address of the header
    if(Isfree(address)){
        PRINTERR(ISFREE, file, line);
        return;
    }
    Setfreebit(address, 0); // Mark the block as free
    coalesce(&memory[0]); // Coalesce adjacent free blocks
    return;
}

// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠴⣶⣿⣬⣭⣧⠴⣠⠤⠤⣤⣤⡄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠔⡪⣻⣶⣿⣿⣿⠯⠕⠤⢹⡶⣽⡱⡰⡺⣽⡿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠞⡱⣻⢟⢿⣿⣿⢿⣷⢜⠴⣕⣤⡳⣮⣻⣮⢪⣿⠿⣮⡻⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣯⢭⡾⣿⣾⣿⣿⠟⠛⠻⣿⣻⡈⣻⡻⣿⣿⣌⣷⣽⣷⣼⣽⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣏⣏⡷⣻⢯⣿⡿⠃⠀⠀⠀⠘⢿⣷⣾⣛⢿⣿⣿⣿⣿⣿⣿⣿⣾⣯⣳⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⢻⣿⣝⣿⡾⡑⢀⠀⠀⠀⠀⠀⠀⠛⣟⣗⡻⣿⣿⣿⣿⣿⣿⣿⣿⣽⣧⢳⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡟⢿⣻⡟⠀⠀⠀⣠⣴⠖⣛⡉⠁⠂⠘⡟⣷⣟⠿⡿⣿⡿⢻⣿⣿⣿⣿⣧⢇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⢙⣿⡿⣲⣦⠀⠈⠋⠐⣿⣿⢓⡷⠦⠀⠸⠹⡟⣆⢹⡟⡝⠀⢿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⢀⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣜⣿⣿⣿⠟⢷⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠟⣿⣿⡿⠵⢙⢸⢹⣿⣿⣟⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠉⢂⠀⠀⠀⢀⠌⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣻⣿⠖⠁⠀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡿⢙⡈⢈⣡⣿⡿⣿⣿⣿⣟⡇⠀⠀⠀⠀⠀⠀⠀⠀
// ⠠⠀⠆⠀⢠⠃⢰⠀⠀⢀⣀⡀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡿⣹⠀⠀⠄⢀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡕⡞⠖⣿⣿⣿⣿⣿⣿⡟⣿⣳⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⡃⠘⡀⠆⠀⡆⢀⠔⠁⢀⠤⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣧⣷⢧⠀⠁⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⡟⠀⠰⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀
// ⠄⡃⢀⣷⡤⣤⡷⡥⣤⡔⢁⠠⠄⠠⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡟⣿⣧⠰⠆⡟⢓⠖⠋⠀⠀⠀⠀⠀⠀⠀⠃⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣳⠀⠀⠀⠀⠀⠀⠀
// ⠀⠸⣿⡟⣷⣿⢷⢀⣿⡿⢗⣶⡔⠒⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣼⡕⣻⣷⡐⠚⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⡄⠀⠀⠀⠀⠀⠀
// ⠈⠺⣿⡣⢁⣿⠎⣸⡫⣪⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⣿⣕⣿⣷⡄⠀⠀⠀⠀⠠⠄⠀⠀⠀⠀⠀⠀⠀⠀⣻⣿⣿⣿⣿⣿⣿⣿⣿⣷⣧⠀⠀⠀⠀⠀⠀
// ⠀⠀⠘⣯⠰⣗⢻⣿⢱⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣤⣿⣿⣮⠿⣿⣿⣿⣽⣿⠀⠀⠀⠀⠀⠀⠀⠀⠄⠒⡙⣿⣿⣿⣿⣿⣿⣿⣿⣿⡾⡆⠀⠀⠀⠀⠀
// ⠀⠀⠀⠘⡦⣟⣄⣽⡌⣘⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⡛⡿⢻⣿⣯⣹⣿⣿⠿⢛⠀⠀⠀⠀⠀⠀⠀⠀⢀⡤⠚⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠈⢎⠚⠲⡿⢽⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄⠈⢡⣾⡝⡿⣿⡟⣿⣿⡏⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⣠⣿⣿⣿⣿⣿⣿⣿⣿⣯⣇⠀⠀⠀⠀
// ⠀⠀⠀⢀⠔⠸⣦⣥⣠⣸⠃⠼⣿⣡⠀⠀⠀⠀⠀⠀⠀⠀⠂⠀⢠⣻⣻⣇⣿⣿⣿⣿⠁⢰⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⢔⣾⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⡄⠀⠀⠀
// ⠀⠀⠠⠎⣠⣼⡿⣿⣷⣾⡆⢀⡹⡿⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⡌⣰⡿⣽⣿⢿⣿⣏⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣞⣯⢍⣥⣶⣿⣿⠯⠿⠿⢽⣿⣿⣯⣯⣷⠀⠀⠀
// ⠀⠀⡴⣿⡌⢻⢿⣟⣿⣿⣿⡷⣿⣁⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡶⠋⣰⣿⣯⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⠖⣱⣯⠻⣿⣿⣿⠿⠋⠀⠀⠀⠀⠀⠉⠻⣿⣿⣼⣇⠀⠀
// ⠀⠀⣿⡹⣿⣔⣘⣿⣿⣿⣿⣟⠿⡟⡀⠀⠀⠀⠀⠀⠀⡀⠜⠉⢀⣴⣿⣿⣿⣿⣿⣿⡄⠀⠀⣀⡠⠴⡒⢝⠫⠊⢰⢿⠷⣆⣿⡿⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣷⣿⡀⠀
// ⠀⢸⣉⣣⣿⠿⣏⠉⣹⣿⣿⣿⡅⣳⣢⠀⠀⠀⠀⣠⠊⠀⢀⣠⣞⢟⠿⠙⣿⣿⣿⣿⠟⣋⠩⠔⠊⠁⠈⠀⠀⠀⢸⣆⠀⣿⢻⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣻⣿⣿⣇⠀
// ⠀⠸⣿⠿⠢⡀⣼⣿⢯⡝⣿⣿⣿⣿⣿⠁⡐⠊⠉⠀⣠⠶⠋⠈⠀⠁⠀⠀⠈⠙⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣦⣭⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⠀
// ⠀⠀⠹⡗⢤⢏⡿⢵⠂⠀⢌⢿⢿⣿⣿⠎⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣟⣟⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⡇
// ⠀⠀⠀⠀⠙⢇⡆⠀⠀⢀⢐⣯⣿⣿⣫⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⠀
// ⠀⠀⠀⠀⠀⠀⠑⢤⢔⣵⡿⣿⣾⡿⡻⣿⣓⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⡀
// ⠀⠀⠀⠀⠀⠀⠀⢘⢯⣾⣾⣿⢇⣬⠟⢀⠝⠳⣼⡷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⠀
// ⠀⠀⠀⠀⠀⠀⠀⢀⢎⣙⣿⣿⣛⢷⣤⠀⠑⠀⡨⡙⢾⣕⡢⣠⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣯⣿⣟⣿⣵⣝⠻⣢⣀⠈⠈⠢⠈⠚⠔⡵⡪⢂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠗⣠⣀⣲⣤⣄⣀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⠀
// ⠀⠀⠀⠀⠀⠀⠀⢀⠸⣿⣿⣿⣿⣿⣾⣯⡪⣻⣧⡄⠀⠀⠤⠀⠀⠙⢳⢍⠦⣀⠀⡀⠀⠀⠀⠀⠀⠀⢀⣼⣓⣒⡻⡿⣿⣿⣿⣿⣷⣦⡀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀
// ⠀⠀⠀⠀⠀⠀⠀⠸⠘⣿⣿⣿⣿⣿⣿⣿⣿⣮⣮⡪⢧⡀⠀⠈⠳⣀⣀⡭⡣⣾⣏⣚⢡⡄⡀⠀⠀⠀⡰⠃⠀⠀⠉⠉⠓⢭⡛⢿⣿⣿⣷⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⡇⢿⣿⣿⣿⣿⣿⣿⣿⣿⣮⣯⣶⣫⣦⣤⣄⣜⠫⣾⠞⠁⠄⠈⣹⡍⣼⡶⡄⡔⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⢝⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠹⣈⣿⣿⣿⣿⣿⣿⣿⣿⠟⣿⣿⣝⣄⢟⣗⡕⢭⠃⡤⠨⣰⡼⠋⣰⣫⣾⣿⣶⠦⣄⡀⠀⠀⠀⠀⢀⠀⠀⠈⢫⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠿⠟⠛⠋⠀⠀⣿⣿⣿⣿⣿⡿⣝⢟⣀⠐⣼⢟⣥⣾⡟⠛⣿⣿⣿⣷⣿⣿⣦⣄⠀⠀⠀⠀⠀⣀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⣿⠛⠙⠿⣯⢻⡜⣹⡗⢿⢃⢾⣾⡿⠿⠾⠿⣿⡿⢟⢿⣿⣿⣯⣙⣄⡀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣞⠀⡿⠀⠀⠀⠀⠘⠳⡏⠒⣞⣞⣾⠋⠔⢒⢀⢊⢿⣵⣤⣿⣿⣿⣿⣿⣿⣿⣷⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀