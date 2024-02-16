#include"mymalloc.h"
#define MEMLENGTH 512
//private static functions/variables
static double memory[MEMLENGTH];
static void* startindex = &memory[0];
static void* endindex = &memory[MEMLENGTH]; 
//determines if heap is initialized
static bool heapInit = true;
/**
 * get's the size of the header.
 */
static size_t get_size(void* ptr){
    return ((block_t*)ptr)->size;
}
static bool isFree(void* ptr){
    return ((block_t*)ptr)->isFree == 0;
} 
static void* get_next(void* ptr){
    return ((block_t*)ptr)->next;
}
static void* get_prev(void* ptr){
    return ((block_t*)ptr)->prev;
}
static void* bestfit(size_t size){
    void* toreturn = NULL;
    size_t min = -1;

    for(void* i = (void*)&memory[0]; i < endindex; i = get_next(i)) {
        //condition 1. this chunk needs to fit the data we need
        //condition 2. this space is straight up free
        //condition 3. arbitrary max. if its less than arb then we set the max to that one and set the minimum that satisfies.
        if (isFree(i) && (get_size(i) > size) && (get_size(i) < min)) {
            size = get_size(i);
            toreturn = i;
        }   
    }

    return toreturn;
}

//public functions available to client
void* mymalloc(size_t size, char *file, int line){

    // if the heap has not been initialized, initialize it
    if (heapInit) {
        block_t header;
        header.isFree = true;
        header.size = (MEMLENGTH*8) - sizeof(block_t);
        header.next = endindex;
        memcpy(&memory[0], &header, sizeof(header));
        heapInit = false;
        
    }

    void* destination = bestfit(size);

return ;
}


void myfree(void* ptr, char* file, int line){





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
