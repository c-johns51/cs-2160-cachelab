
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

#define MEM_SIZE 64

// Global variables
bool verbose = false;
int lruLine = 0;


// CacheLine structure
typedef struct cacheLine {
    int set;
    int tag;
    int lru;
    
} CacheLine;

// Function prototype
void buildCache(CacheLine* cache, int S, int E);
void simulateCache(CacheLine *cache, unsigned long long address, int E, int *hits, int *miss, int *evictions, int s, int b, int tag);
//Starts main
int main(int argc, char *argv[]) {

    int hits = 0;
    int misses = 0;
    int evictions = 0;
    int s,E,b;
    char *tracefile;
    int opt;
    char c;
    unsigned long long address;
    int size;
    while((opt = getopt(argc, argv, "s:E:b:t:v")) != -1){

        switch(opt){

            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                tracefile = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            default:
                exit(0);
        
        }
    } // End opt while loop

    int S = pow(2, s);
    int tag = MEM_SIZE - (s + b);

    // Allocates enough memory for the cache, using the number of sets and lines times the size of CacheLine struct
    // Then buildCache() builds the cache using this memory
    CacheLine *cache = malloc((S * E) * sizeof(CacheLine));
    buildCache(cache, S, E);

    // Opens a file on the given tracefile
    FILE *filePtr = fopen(tracefile, "r");

    // If the tracefile was successfully opened...
    if(filePtr != NULL){

        // While the tracefile still has lines to be read...
        while(fscanf(filePtr, " %c %llx,%d", &c, &address, &size) == 3){

            // Prints the current line in the tracefile if in verbose mode
            if(verbose){

                printf(" %c %llx,%d", c, address, size);

            }
            // Switch cases that perform a cache sim (or doesn't if operation is I)
            switch(c){
                case 'I':
                    break;
                case 'M':
                    // M is a special case where two cache hits or a miss and a hit and eviction are possible
                    // The cache sim is run twice to account
                    simulateCache(cache, address, E, &hits, &misses, &evictions, s, b, tag);
                    simulateCache(cache, address, E, &hits, &misses, &evictions, s, b, tag);
                    break;
                case 'S':
                    simulateCache(cache, address, E, &hits, &misses, &evictions, s, b, tag);
                    break;
                case 'L':
                    simulateCache(cache, address, E, &hits, &misses, &evictions, s, b, tag);
                    break;
                default:
                    break;
            }

            if(verbose){

                printf("\n");
            }

        }


    }

    printSummary(hits, misses, evictions);

    //Frees the memory allocated in the cache
    free(cache);


} // End of main

// Function that builds the cache given the pointer where the cache is located
void buildCache(CacheLine *cache, int S, int E){

    // Creates a new line and gives it a tag of -1 (basically no tag) and a LRU value of 0
    CacheLine currLine;
    currLine.tag = -1;
    currLine.lru = 0;

    // Uses pointer arithmetic inside a nested loop to traverse the 2D array, places currLine in each index
    for(int i = 0; i < S; i++){
        for(int j = 0; j < E; j++){

            cache[i * E + j] = currLine;

        }

    }

}

void simulateCache(CacheLine *cache, unsigned long long address, int E, int *hits, int *miss, int *evictions, int s, int b, int tag){

    // Creates a line to compare to cache values, using the given address, s, b, and tag
    CacheLine currLine;
    currLine.tag = address >> (s+b);
    currLine.set = address << tag;
    currLine.set = currLine.set >> (tag + b);

    //Creates some holder variables
    bool hit = false;
    bool empty = false;
    int emptyLine = 0;

    for (int i = 0; i < E; i++) {

        // If the tag line of the line in the cache is the same as the current line's tag line,
        // result in a hit and update hits counter, set hit to true, increment LRU counter by 1 and,
        // update the cache line's lru value
        if(cache[currLine.set * E + i].tag == currLine.tag) {
            (*hits)++;

            if(verbose){

                printf(" HIT");

            }
            hit = true;
            lruLine++;
            cache[currLine.set * E + i].lru = lruLine;
        }//End of if

        //Checks if line is empty
        if(cache[currLine.set * E + i].tag == -1) {
            empty = 1;
            emptyLine = i;
        }//End of if

    }//End of for loop


        // if a hit did not take place and there was an empty line,
        // set the cache line's tag and lru values to those of the current line's
        // and update miss counter
        if(hit == 0 && empty == 1) {
            cache[currLine.set * E + emptyLine].tag = currLine.tag;
            cache[currLine.set * E + emptyLine].lru = lruLine;
            (*miss)++;

            if(verbose){

                printf(" MISS");

            }
        }//End of if

        // If a hit did not take place and the line was not empty
        // Use 
        if(hit == 0 && empty == 0) {

            int smallest = cache[currLine.set * E + 0].lru;
            int leastUsed = 0;
            for(int i = 0; i < E; i++){

                if(smallest >= cache[currLine.set * E + i].lru){

                    smallest = cache[currLine.set * E + i].lru;
                    leastUsed = i;

                }

            }
        

            cache[currLine.set * E + leastUsed].tag = currLine.tag;
            cache[currLine.set * E + leastUsed].lru = lruLine;
            (*miss)++;
            (*evictions)++;

            if(verbose){

                printf(" MISS EVICT");

            }
        }//End of if    

}//End of cache sim

/*//Prints the summary of the calculations
void printSummary(int hits, int misses, int evictions) {
    printf("Total hits: %d Total misses: %d Total evictions: %d\n", hits, misses, evictions);
}//End of print summary*/
