
/*
Carson King | SID: 109920861

Cameron Johnson | SID: 109667312

*/
#include "cachelab.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_SIZE 64


// Cache structure

typedef struct cacheLine {
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
    int tag;
    int lru;
    CacheSet *sets;

} Cache;


// Function protos
void printSummary(int hits, int misses, int evictions);
bool initializeCache(Cache* cache);
void cleanCache(Cache *cache);
void simulateCache(Cache *cache, FILE *filePtr, int *hits, int *miss, int *eviction);
int LRU (CacheLine Line, Cache cache, unsigned long long set);
//Starts main
int main(int argc, char *argv[])
{
    
        int s = 4;
        int E = 1;
        int b = 4;
        int m = 64;

    CacheLine cache[s][E];
    FILE *filePtr;

    //Checks cache can be intialized, if so initalizes it
     if (initializeCache(&cache)) {

        

        int opt;
        char *tracefile = "traces/dave.trace";

    /*while((opt = getopt(argc, argv, "s:E:b:t:vh")) != -1){

        switch(opt){

            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                cache.E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                tracefile = optarg;
                break;
            case 'h':
                exit(0);
            default:
                exit(1);
        


        }


    
        

    } // End opt while loop*/

        cache.s = 16;
        cache.b = 16;
        cache.E = 1;
        int tag = m - (s + b);

        printf("%d, %d, %d\n", cache.s, cache.b, cache.E);

        puts(tracefile);
        filePtr = fopen(tracefile, "r");

        int totalHits = 0;
        int totalMiss = 0;
        int totalEvictions = 0;
        
        //Runs the trace file for the simulated cache
        simulateCache(&cache, filePtr, &totalHits, &totalMiss, &totalEvictions);
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

//Start of LRU function



//Initalizes the cache
/*bool initializeCache(Cache* cache) {

    puts("test1");

    cache->sets = (CacheSet *)malloc(sizeof(CacheSet) * (1 < cache->s));

    bool memAlloc = true;

    //Checks if memory can be allocated for the cahce sets
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
                    cache->sets[i].lines[j].lru = 0;
                    cache->sets[i].lines[j].tag = -1;
                }//End of for loop
            }//End of final if
        }//End of for loop
    }//End of intial if

    puts("test2");
    return memAlloc;
}//End of initialize cache*/
bool intializeCache(Cache line[MAX_SIZE][MAX_SIZE], int S, int E) {
    Cache currentLine;
    currentLine.tag = -1;
    currentLine.lru = 0;

    for (int i = 0; i < S; i++) {
        for (int j = 0; j < E; j++) {
            line[i][j] == currentLine;
        }
    }
}


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
void simulateCache(Cache *cache, FILE *tracefile, int *hits, int *miss, int *eviction) {

    puts("test3");
    //Declares intial variables
    char operation;
    unsigned long address;
    int size;
    
    if(tracefile != NULL){
        puts("test3.5");

        //Gets the tag and index from the address
    while(fscanf(tracefile, " %c %lx,%d", &operation, &address, &size) == 3) {

        puts("test4");
        if (operation != 'I') {
            unsigned long tag = address >> (cache->s + cache->b);
            unsigned long setIndex = (address >> cache->b) & ((1 << cache->s) -1);

            //Sets intial hit value to zero
            int hit = 0;
            int lruIndex = 0;
            int counter = 0;

            //Checks individual lines in cache sets
            for (int i = 0; i < cache->E; i++) {
                CacheLine *line = &cache->sets[setIndex].lines[i];
                
                
                //If a hit occurs
                if (line->valid && line->tag == tag) {
                    hit = 1;
                    (*hits)++;

                    line->lru = counter++;

                    i = cache->E;
                }//End of if
                
                if (line->lru < cache->sets[setIndex].lines[lruIndex].lru) {
                    lruIndex = i;
                }
            }//End of for loop

            if (!hit) {

                //update the miss counter
                (*miss)++;

                cache->sets[setIndex].lines[lruIndex].lru = counter++;

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

    }

    puts("test5");
}//End of simulate cache
