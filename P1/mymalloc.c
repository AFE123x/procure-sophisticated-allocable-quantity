#include"mymalloc.h"
#define MEMLENGTH 512
//private static functions/variables
static double memory[MEMLENGTH];

//enum for error handling
//NO_FREE_SPACE = mallocing a size that is not valid/ there is no more spance
//FREE_UNKN_PTR = attempting to free a pointer that is not in the heap OR not directed at a header
//DOUBLE_FREE = attempting to free a space that was already unallocated
typedef enum {
   NO_FREE_SPACE,
   FREE_UNKN_PTR,
   DOUBLE_FREE,
   //there could possibly be more but I dont know what else for now
} Errors;

void errorHandling (Errors errorType,void* ptr,size_t size,char *file, int line) {
    switch(errorType){
        case NO_FREE_SPACE:
            fprintf(stderr, "ERROR: There is not enough free space for this Malloc request. Try a smaller size or Freeing\n");
            break;
        case FREE_UNKN_PTR:
            fprintf(stderr, "ERROR: Pointing to unknown Malloc address. Try another Pointer\n");
            break;
        case DOUBLE_FREE:
            fprintf(stderr, "ERROR: Ptr already Freed. Cannot be Freed again\n");
            break;
        default:
            fprintf(stderr, "ERROR: something happened\n");
            break;
    }
}

//get the size of the header/node
static size_t get_size(void* ptr){
    return ((block_t*)ptr)->size;
}

//check if the the block/header is free
static char isFree(void* ptr){
    block_t* temp = (block_t*)ptr;
    return temp->isFree == 0;
} 

//get next header node in array
static void* get_next(void* ptr){
    return ((block_t*)ptr)->next;
}

//get previous header node in array
static void* get_prev(void* ptr){
    return ((block_t*)ptr)->prev;
}

//functions create header
static block_t make_header(int size, char isfree, void* next, void* prev){
    block_t newhead;
    newhead.size = size;
    newhead.isFree = isfree;
    newhead.next = next;
    newhead.prev = prev;
    return newhead;
}

//finding best free block to use for malloc
static void* bestfit(size_t size){
    
    //pointer that returns the best fit location
    void* toreturn = NULL;
    
    //2's compliment representation of size_t (values cannot be negative therefor -1 is b i g)
    size_t min = -1;

    for(void* i = (void*)&memory[0]; i != (void*)NULL; i = get_next(i)) {
        //condition 1. this chunk needs to fit the data we need
        //condition 2. this space is straight up free
        //condition 3. arbitrary max. if its less than arb then we set the max to that one and set the minimum that satisfies.
        if ((void*)&memory[MEMLENGTH - 1] && isFree(i) && (get_size(i) >= size) && (get_size(i) < min)) {
            printf("best fit so far is: %p\n",i);
            size = get_size(i);
            toreturn = i;
        }   
    }
   //printf("best address found: %p\n",toreturn);
    return toreturn;
}

 static void printheader(block_t* input){
   printf("At address %p\tsize: %zu\tisfree? %s, struct->next: %p\n",input,input->size,input->isFree == 0 ? "is free" : "is allocated",input->next);
}
//change back to static
static void printmemory(){
    for(void* i = &memory; i != (void*)NULL; i = get_next(i)){
        printheader(i);
    }
}
static int i = 0; //solely for troubleshooting

////// kareems attempt at coalescing
static void coalesce(block_t* ptr) {
    // if (ptr->next == &memory[MEMLENGTH]) {
        
    // }

    //simply check if the next for prev are null.
    //if they are null immediately return because there is no coalescing to be done.
    if (ptr->next == NULL || ptr ->next == (block_t*)&memory[MEMLENGTH]) {
        // printf("ptr->next == NULL || ptr ->next == (block_t*)&memory[MEMLENGTH]\n");
        if (isFree(ptr) && isFree(ptr->prev)) {
            //header to the left
            block_t* leftHeader = ptr->prev;

            leftHeader->next = (block_t*)&memory[MEMLENGTH];

            //changing the size of header to the left to header on the left + original header + size of header
            leftHeader->size = (leftHeader->size) + ((ptr->size)+sizeof(block_t));
            return;
        }
        return;
    }

    //if previous node is null. it is implied it is the front of memory
    if (ptr->prev == NULL || ptr ->next == (void*)&memory[0]) {
        // printf("ptr->prev == NULL || ptr ->next == &memory[0]\n");
        //check if header to the right is free
        if (isFree(ptr) && isFree(ptr->next)) {
        
            //header on the right
            block_t* rightHeader = ptr->next;
            //header on the right of the right
            block_t* rightrightHeader = ptr->next->next;

            /////////////////////////////
            if (rightHeader->next == NULL) {
                ptr->next = NULL;
                ptr->size = ((ptr->size)+((rightHeader->size)+sizeof(block_t)));                
                return;
            }
            ////////////////////////////

            //set node after next to original node
            rightrightHeader->prev = ptr;
            //set original node to point to node after next
            ptr->next = rightrightHeader;

            //make the size of the original node current size + next header + payload.
            ptr->size = (ptr->size)+((rightHeader->size)+sizeof(block_t));

            return;
        }
        return;
    }


    if (ptr->next != NULL && ptr->prev != NULL) {
        // printf("ptr->next != NULL && ptr->prev != NULL\n");
        if ((ptr->prev)->isFree == 0) {
            block_t* leftHeader = ptr->prev;
            block_t* rightHeader = ptr->next;

            leftHeader->next = rightHeader;
            rightHeader->prev = leftHeader;
            leftHeader->size = (leftHeader->size)+((ptr->size)+sizeof(block_t));
            // printf("ptr->left is free. coalescing to the left\n");
        }
        //what if the pointer after next is NULL or the end of the memory???
        if ((ptr->next)->isFree == 0) {
            block_t* rightHeader = ptr->next;
            block_t* rightrightHeader = ptr->next->next;

            ptr->next = rightrightHeader;
            rightrightHeader->prev= ptr;
            ptr->size=(ptr->size)+((rightHeader->size)+sizeof(block_t));
            // printf("ptr->right is free. coalescing to the right\n");
        }


        return;
    }

    //we should not need a check for if both headers to the left AND right are NULL because that shouldn't ever happen.
    //but maybe I might implement it anyway

    // printf("Coalescing attempted but nothing happened\n");
    return;
}                  
//////
//static int i = 0;
//public functions available to client
void* mymalloc(size_t size, char *file, int line){
    size = (size + 7) & -8;
    if (memory[0] == 0) {

        block_t header = make_header(((MEMLENGTH*8)-sizeof(block_t)),0,NULL,NULL);
        memcpy(&memory[0], &header, sizeof(header));
    }

    void* destination = bestfit(size);

    //could we put this logic inside of best fit?
    if (destination == NULL) {
        errorHandling(NO_FREE_SPACE,destination,size,file,line);
        return NULL;
    }

    //saves old data from header for new header
    size_t old_size = get_size(destination);
    block_t* tempnextheader = get_next(destination);
    //populating header with new data.
    block_t* newheader = (block_t*)destination;
    newheader->size = size;
    newheader->isFree = 1;
    //creating header after.
    block_t mynewheader;
    mynewheader.size = old_size - sizeof(block_t) - newheader->size;
   //printf("new size of header: %zu\n",mynewheader.size);
    mynewheader.next = tempnextheader;
    mynewheader.prev = newheader;
    mynewheader.isFree = 0;
    newheader->next = destination+sizeof(block_t) + newheader->size;
    //copies our data to memory.
   //printmemory();
    printf("We're getting fault at %p vs. %p\n",(destination+sizeof(block_t) + newheader->size),&memory[MEMLENGTH - 3]);
    if((void*)(destination+sizeof(block_t) + newheader->size) < (void*)&memory[MEMLENGTH - 3]){
    void* ptr = memcpy(destination+sizeof(block_t) + newheader->size,&mynewheader,sizeof(block_t));
    }
    // newheader->next = (block_t*)ptr;  
    return (block_t*)destination + 1;
}


void myfree(void* ptr, char* file, int line){
    //this syntax doesnt work but the idea is
    //errorhandling for double free
    block_t* temp = ((block_t*)ptr);

    //error checking for going out of the bounds of the array
    // printf("%p < %p || %p >= %p\n",(((block_t*)ptr) - 1),&memory[0],(((block_t*)ptr) - 1),&memory[MEMLENGTH]);
    if((((block_t*)ptr) - 1) < (block_t*)&memory[0] || ((((block_t*)ptr) - 1) >= (block_t*)&memory[MEMLENGTH])){
        fprintf(stderr,"ARE YOU IDOT!!! YOU NO FREEFREE D-:. Just give up. stop using my program\n");    
        return;
    }

    //error checking for a non existent pointer
    if(ptr == NULL){
        fprintf(stderr,"unable to free NULL pointer\n");
        return;
    }

    //error checking for freeing an already free block
    if (isFree(temp-1)) {
        errorHandling(DOUBLE_FREE,ptr,0,file,line);
        return;
    }

    (temp - 1)->isFree = 0;

    //coalesce(temp-1);
    printmemory();
    //also need checking for if a pointer isnt a header at all.
    //pointing within the bounds of the array but not towards a header.

    // (temp - 1)->isFree = 0;
}

// B#####################&&&&&&##BPP55PGBB#&&#BGGGGGBGGGGB5GGYGGPJG###B5B#####&#GB#&G??7GBPPGGB####GB###BGGGGGP?P&##B5B##PYB#GB#BG#####BGG5PBGB#BGG5G###B
// #########&&&&&&&&&#&#####BBGGPPPGBB####&&#BGGGGGBBGGGGBPGPJGP?PB###PG########B##P??~?BGJPGB####BG####GGGGGGJ?P&##GPB##PP#BG##BGB#####GGPPBBB##BGG5B###
// ##################BBBBGGGGBBB#####BBBGB#BGG5GGGBBBGGGBBGGP?P75B###B5B####BP&##&P?J!^PBY?GG###&PGB###BGGGGBY??G&##GB##BGB#BB###GB#####BGG5B#G##BGG5P###
// ###############################BGGGGGGBBG5YPGGBBBGGGGB#GGP77JG####BB###B#5G&#&PJJ!^!B5JJGB#&&#5GB###BGGGGP???B&##G###BG##BB###GB######GG5B#G###GGG5B##
// ###############################BGGGGBBGPY5GGB#&BBGGGGB#GGG77PB####B####PG?G&&G??~..~BJ??GB##&P5BB###BGGGGJ???B##BB###BG##B5B##GB######BG5B#B###BGG5G##
// BBBBBBBBBBBBBB#################GGGBBGP55PGB##&#BBGGGGG#GGG?YGB#######&P5J7G&#YY?!!^!G???P####JYBB###BGGB5???J########G5##G?B&#GB######BGG##B####GGPP##
// B#############################BGBBGPP5PBBBBB###BBGBGGG#BGG5PG#########Y5YP##&&&&&#BPGYJJP&#&B5YBP###BGGGJ???J#######BYB#BY?B&#GB######BGB##B####GGP5B#
// ##############################BGPPPPPGGGBB#####BBG#BGG#BGGGGG#########B&@B#&#&&&&&@#B#B5J#&&B?JBPG&#BGGP????5&####&#Y5&#5!!B&#B#######BG########BGP5B#
// #############BBBBBBBBBBBBBBBBGGGBPPGBB###&#####BBB#BGGB#BGGGB#######&PB@5J&&5B@@@@BY:.Y#BP#&#J?GP5&##GB5????B####&BYJ#&GJ~?##B#&######BB########BGG5B#
// BBBBBBBBBBBBBBBBBBBBBBBBBBBGGBBBBGBB#####BB####BBB##GGB#BGGGB#######&Y5#7G@#&@@@@@G#GP#JPBG&&Y?5GJB##BBY???P###&&GJ!5&G~:~5###BB######B#########BGGGG#
// BBBBBBBBBBBBBBBBBBBBBBBBGGGGGGGGB#######BG#####BBB##BGG##BGGG####&###Y!!~##G#@@@@@BB&@#^!PBP&P!?GJ5&##BY??P&##&B5?~Y&5^..~Y&##5B################BGGGB#
// BBBBBBBBBBBBBBBBBBBBGGGGGGGBB####BB###BGB######BBB##BBGB&#BGG########J^.:BP?G@@@@BPG#@? :7Y7J#!~JP?G&##5?G&#&#PJ!^Y#5~^^^75&#5Y#################GGGGB#
// BBBBBBBBBBBBBBBBGGGGBBBB###BBBGGGG##BGGB######BB#B###BBB###BBB#######J::.7GJ75GGY!7J&5...:^7!!5^~JY?B&#GG&&#PY5PPB&&###P?!G#Y7P&################GGGGB#
// BBBBBBBBBBBBBBBBBBB#######BGGGGGGBBGGB#########G#B####BB#&##BB####&#&Y.::~G&GY??J5BB?....:::~~^!:^!7?B###B5JYB@GG&&#7:YB#GP???#################BGGGGB#
// BBBBBBBBBBBBBBBBBBBBBBBB#BGGGGGBBGGB##########BGB###&#BBB#&##B######&5...::!7J?Y5GP7~:.:::::.::.::.:7B#&G?!~Y&&##@&5BGG75@?7?5&################GGBGBB#
// BBBBBBBBBBBBBBBBBBBBBB##BGGGBBBGB#############BGG###&##BB##########5#B:...:......::^^:::::::::::::~YP5J5#?.~&B&@@@&#@B!!Y@B7J#######B#########BGBGGBB#
// BBBBBBBBBBBBBBB#&@@@@@@&BBBBBBB###############BGGB##&###B###&#####&Y5&J.::::::::::::.::::::::::::^7!^^:~!Y:PPB@@@#G#G:.J#GG?G&#####B##########GBBGBBB#
// BBBBBBBBBBBBBB&@@@@@@@&BBBBB#########BB#######BGGG###&#######&####&G~5#!.:::::::::::::::::::::::::.:::::::~BJJ#BYJYJ. ~?!^JP######B##########BB#BB#BB#
// BBBBBBBBBBBBB#@@@@&#BBBB########BBBBBB######&&BGGGB##&&############&J:JG^.:::::::::::::::::::::::::::::::.~PPYJJ5Y^ .::..JB&#####B##########BBBBGBBBB#
// BBBBBBBBBBB###&#######BBBB###BGGGGGB######&&&&BGGGG###&##############!.75~.::::::::::::::::::::::::7~:::::..:!?YP7:.::.:5&&####B###B########BBBBBBBB##
// BBBBBBBBB####B##@@@@#GGGG##BGGBGGBB#####&&&&&&BGGGGG#&#&#########B#&&#~.~?~::::::::::::::::::::::.~5?::::::::...:::::.~G&&###BB##BBB#######BBBBBBBBB##
// BBBBBBBBBB####B#@@@#GGGB#BBGGGGBB#####&&&&&&&&BGGGBGB###&########BJG&&#7.:^::::::::::::::::::::::~Y?^::::::::::::::..7#&####BBBBBBB#######BBBBBBBBB###
// BBBBBBBBBB######@@#GGBBBBGGGGBB#####&&&&&&&&&#BGGGGGGB&##&########Y7JB&&J:.::::::::::::::::::::::!^:::::::::::::::.:Y&&#####BGBGBB######BBBBB#BBBB####
// BBBBBBBBBB######@&GGBBBGGGGB######&&&&&&&&&&&#BGGBBGGGB&##&#######YJ7~JB&P~.:::::::::::::::::::::.:::::::::::::::.!G&######BGBBGB######BBBB##BBBBB####
// BBBBBBBBBB######&BBBGGGGBB######&&&&&&&&&&&&&&BGGBBGGGGB&##&#####&P?YJ^~JPGJ:.:::::::~^^^::::::::::::::::::::::.:J#&######BGGBGB#####BBBBB##BBBBB#####
// BBBBBBBBBB######BBGGGGBB####BB#&&&&&&&&&&&&&&&BGGBBGGGGG#&#########J?Y57:^!??^::::::::^^~~^~~::::::::::::::::..!G&########BGGBB####BBBBBB#BBBBBB#####B
// BBBBBBBBBB###BBGGGGBB####BBGB#&&&&&&&&&&&&&&&&BGGBBGGGGBB#####&###&P??JPY~::^:::::::::::~~:::::::::::::::...^75#&####B###BGGB####BBBBBBBBBBBBBB####BBB
// BBBBBBBBBBBBBGGGGB####BBGGG#&&&&&&&&&&&&&&&&&&BGGB#GGGGBB###########Y??J5PJ~:.::::::::::::::::::::::...:^!JPB&&#####BB##BGGB##BGBBBBBBBBBBBBBB###BBBBB
// BBBBBBBBBBGGGGB####BGGGGGB#&&&&&&&&&&&&&&&&&&&BGGB#GGGGBG########&#&BJ??JYPPJ~:.::::::::::::::....:^~7YPB&&&########B###BGGBBBBGB#BBBBBBBBBB###BBBBBBB
// BBBBBBBGGGGB###BBGGGGGGGB#&&&&&&&&&&&&&&&&&&&&BGG#&GGGGGGB###B######&BYJ??J5PPJ~:.:::::::....:^!?5GB#&&&###########BB###BGGBBB#BGBBBBBBBBBB##BBBBBBBBB
// BBBBGGGBBB##BGGGGGGGGGB#&&&&&&&&&&&&&&&&&&&&&&BGG#&BGGGGGB###G#&###PP#B5YJJJJ5PPJ!:....::~!?5B&@&&&################BB##BGGGBBB#BGB#BBBBBBB#BBBBBBBBBBB
// BBGGBBBBBBBGGGGGGGGGGB#&&&&&&&&&&&&&&&&&&&&&&&BGG#&BGGGGGG###GB###&BJYPG5YYYJJJ5PPY7~!?Y55PY5&@&&#################BGB##BGGBBBB##BGB#BBBBBBBBBBBBBBBBBB
// BBBBBBBBBGGGGGGGGGGB#&&&&&&&&&&&&&&&&&&&&&&&&&BGG#@#GGGGGG#&#GGB&###5JJJYJYYYPBP555P5PP5YYYYJG@&&#################BGB##BGBB#BB##BGGBBBBBBBBBBBBBBBBBBB
// BBBBBBBGGGGGGGGGGB##&&&&&&&&&&&&&&&&&&&&&&&&@&BGG#@&GGGGBGB&#GGG###&BP#G5YJJG@@#&&P?P&&&5JYPYY#@&#################BGB#BGGBG##B###BGB#BBBBBBBBBBBBBBBBB
// BBBBBGGGGGGGGGGGB#&&&&&&&&&&&&&&&&&&&&&&&&&@&&BGG#&&BGGGBGB##GGGB&###&&&@&#B@@BYB@#5&&B@#B&@#YJG##################BGG#BGGBG##B####BGB#BBBBBBBBBBBBBBBB
// BBBGGGGGGGGGGGB#&&&&&&&&&&&&&&&&&&&&&@@&&&&&&&BGG#&&#GGGBGG##GGGG#&###YY5G&@@@#&@@@@@@&@@&55&&PJG@&&##############GGGBBGGBG##BB####BGB#BBBBBBBBBBBBBBB
// BGGGGGGGGGGGB##&&&&&&&&&&&&&&&&&&##&&###BB#####GG#&&#BGGBGG##GGGGB#&#&GYJP&@@@@@@@@@@@@@@@GJY#@BG@@@&&&##########BGGGBGGBBG##BB#####BGB#BBBBBBBBBBBBBB
// GGGGGGGGGGGB#&&&&&&&&&&&&&#####BBBBBBB#########GG#&&&BGGBGG#BGGGGG#&&&#5P@@@@@@@@@@@@@@@@@@BYYG&@@@&&&&&&&########BGGGGGBBG###BB#####BGB#BBBBBBBBBBBBB
// GGGGGGGGGB#&&&&&&&&&####BBBBBGBBB##############GGB&&&#GGGGGBBGGGGGB&&&&#&@@@@@@@@@@@@@@@@@@@&PY5&@@@&&&&&&########BGGGGGBBGB##BB#######BB#BBBBBBBBBBBB
// GGGGGGGB#&&&&&&###BBBGGGGBBB###################BGB###&BGGBGBBGGGGGG#&&&&&@@@@@@@@@@@@@@@@@@@@@#P@@@@&&&&&&########BGGGGGBBGB###BB#######BB#BBBBBBBBBBB
