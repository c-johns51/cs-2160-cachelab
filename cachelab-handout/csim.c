
/*
Carson King | SID: 109920861

Cameron Johnson | SID: 109667312

*/
//#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
// #include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// Cache structure

typedef struct cacheLine {
    int t;
    int v;
    int valid;
    
} CacheLine;

typedef struct cacheSet {
    CacheLine* lines;
    
} CacheSet;

typedef struct cache{
    int s;
    int E;
    int b;
    CacheSet *sets;

} Cache;


// Function protos
void printSummary(int first, int second, int third);
bool initializeCache(Cache* cache);
void cleanCache(Cache *cache);

//Starts main
int main()
{
    
    Cache cache;

    cache.s = 2;
    cache.E = 2;
    cache.b = 2;
    
    //Checks cache can be intialized, if so initalizes it
    if (initializeCache(&cache)) {

        printf("cool\n");
        cleanCache(&cache);


    //Calls print summary
    printSummary(0, 0, 0);
    }
    else {

        printf("Failed to initialize cache. End of program.");

    }


    //End of program
}//End of main

//Prints the summary of the calculations
void printSummary(int first, int second, int third) {
    printf("hits: %d misses: %d evictions: %d\n", first, second, third);
}//End of print summary

//Initalizes the cache
bool initializeCache(Cache* cache) {

    printf("made it into this initialize cache func\n");

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
    for (int i = 0; i < (i < cache->s); i++;) {
        free(cache->sets[i].lines);
    }

    //Frees the cache set
    free(cache->sets);
}//End of cleanCache