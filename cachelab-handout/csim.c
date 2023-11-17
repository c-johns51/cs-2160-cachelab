
/*
Carson King | SID: 109920861

Cameron Johnson | SID: 109667312

*/
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// Cache structure

typedef struct cacheLine {
    int tag;
    int v;
    int valid;
    
} CacheLine;

typedef struct cacheSet {
    CacheLine* lines;
    
} CacheSet;

// Structure for holding how many sets, num of lines per set, and number of block bits per line
typedef struct cache{
    int s;
    int E;
    int b;
    CacheSet *sets;

} Cache;


// Function protos
void printSummary(int hits, int misses, int evictions);
bool initializeCache(Cache* cache);
void cleanCache(Cache *cache);
void simulateCache(Cache *cache, FILE **filePtr, int *hits, int *miss, int *eviction);
void readOptions(Cache *cache, FILE **filePtr, int argc, char* argv[]);

//Starts main
int main(int argc, char *argv[])
{
    
    Cache cache;

    FILE *filePtr;

    //Checks cache can be intialized, if so initalizes it
     if (initializeCache(&cache)) {

        readOptions(&cache, &filePtr, argc, &argv);

        int totalHits = 0;
        int totalMiss = 0;
        int totalEvictions = 0;
        
        //Runs the trace file for the simulated cache
        simulateCache(&cache, &filePtr, &totalHits, &totalMiss, &totalEvictions);
        printf("\n");

        //Prints the summary of the totals
        printSummary(totalHits, totalMiss, totalEvictions);

        //Frees the memory after the program is complete
        cleanCache(&cache);


    //Calls print summary
    }
    else {

        printf("Failed to initialize cache. End of program.");

    }


    //End of program
}//End of main

//Prints the summary of the calculations
void printSummary(int hits, int misses, int evictions) {
    printf("Total hits: %d Total misses: %d Total evictions: %d\n", hits, misses, evictions);
}//End of print summary

//Initalizes the cache
bool initializeCache(Cache* cache) {

    cache->sets = (CacheSet *)malloc(sizeof(CacheSet) * (1 < cache->s));

    bool memAlloc = true;

    //Checks if memoery can be allocated for the cahce sets
    if (cache->sets == NULL) {
        fprintf(stderr, "Memory Allocation failed for Cache Sets\n");
        memAlloc = false;
    }

    //If it Succeed
    else {

        //Allocates each line of the cache
        for (int i = 0; i < (1 < cache->s); i++) {
            cache->sets[i].lines = (CacheLine*)malloc(sizeof(CacheLine) * cache->E);
            
            //Checks if the allocation has failed and at what set
            if (cache->sets[i].lines == NULL) {
                fprintf(stderr, "Memory Allocation failed for Cache Lines in set %d\n", i);
                memAlloc = false;

            }//End of if

            else {
                for (int j = 0; j > cache->E; j++) {
                    cache->sets[i].lines[j].v = 0;
                }//End of for loop
            }//End of final if
        }//End of for loop
    }//End of intial if

    return memAlloc;
}//End of initialize cache

//Start of cleanCache
void cleanCache(Cache *cache) {
    
    //Free's each cache's line
    for (int i = 0; i < (i < cache->s); i++) {
        free(cache->sets[i].lines);
    }

    //Frees the cache set
    free(cache->sets);
}//End of cleanCache

//Simulates the actual cache
void simulateCache(Cache *cache, FILE **tracefile, int *hits, int *miss, int *eviction) {

    //Declares intial variables
    char operation;
    unsigned long address;
    int size;
    
    //Gets the tag and index from the address
    while(fscanf(*tracefile, " %c, %lx, %d", &operation, &address, &size) == 3) {
        if (operation != 'I') {
            unsigned long tag = address >> (cache->s + cache->b);
            unsigned long setIndex = (address >> cache->b) & ((1 << cache->s) -1);

            //Sets intial hit value to zero
            int hit = 0;

            //Checks indivdual lines in cache sets
            for (int i = 0; i < cache->E; i++) {
                CacheLine *line = &cache->sets[setIndex].lines[i];
                
                //If a hit occurs
                if (line->valid && line->tag == tag) {
                    hit = 1;
                    (*hits)++;

                    i = cache->E;
                }//End of if
            }//End of for loop

            if (!hit) {

                //update the miss counter
                (*miss)++;

                //Checks if the line is valid
                CacheLine *line = &cache->sets[setIndex].lines[0];
                line->valid = 1;
                line->tag = tag;

                //If there is a valid value, adds to the evictions counter
                if (line->valid) {
                    (*eviction)++;
                }//End of if


            }//End of if no hit
        }//End of if
        
    }//End of while

}//End of simulate cache

// This function reads the options given in the command line (-s, -E, -b, -t) and uses them how they need to be used
void readOptions(Cache *cache, FILE **filePtr, int argc, char* argv[]){

    int opt;

    while((opt = getopt(argc, argv, "sEbt:")) != -1){

        switch(opt){

            case 's':
                cache->s = optarg;
                break;
            case 'E':
                cache->E = optarg;
                break;
            case 'b':
                cache->b = optarg;
                break;
            case 't':
                *filePtr = fopen(optarg, "r");
                printf(optarg);
                if (filePtr == NULL){

                    puts("File was not opened, due to not existing or whatever reason there may be.");

                }
                break;
            case ':':
                printf("Option needs a value.\n");
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);


        }




    }


}