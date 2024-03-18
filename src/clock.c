#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hashmap/myhashmap.h"

#include "clock.h"

struct clock_node_ {
    int data;
    bool rBit;
};

struct clock_cache_ {
    clock_node** cache;
    struct hashmap* map;
    int currIdx, currSize;
    int capacity;
    int hitCount, missCount;    
};

/*
    Initializes the clock cache
    If capacity is less than 1. Returns NULL
*/
clock_cache* clock_init(int capacity) {
    if(capacity <= 0) {
        printf("Capacity must be greater than 0\n");
        return NULL;
    }
    clock_cache* cache = (clock_cache*) malloc(sizeof(clock_cache));
    cache->cache = (clock_node**) malloc(capacity * sizeof(clock_node*));
    for(int i = 0; i < capacity; i++) {
        cache->cache[i] = (clock_node*) malloc(sizeof(clock_node));
    }
    cache->map = hmap_create();
    cache->currIdx = 0;
    cache->currSize = 0;
    cache->capacity = capacity;
    cache->hitCount = 0;
    cache->missCount = 0;
    return cache;
}

int* copy_of(int currIdx) {
    int* idx = malloc(sizeof(int));
    memcpy(idx, &currIdx, sizeof(int));
    return idx;
}

/*
    Performs clock cache operation on the cache with the given page.
    This algorithm contains a clock hand (currIdx) that points to a page in the cache.
    Each page in cache has an additional reference bit.
    If the refernced page is in cache, its reference bit is set to one.
    If the page is not in the cache, and if the reference bit of the page pointed by the clock hand is 1, it is set to zero and the clock hand moves to the next block in cache.
    This step is repeated until the clock hand finds a page with a reference bit of 0. 
    Then the page is replaced with the referenced page and the reference bit of that pages is set to 1.
*/
void clock_access(clock_cache* cache, int data) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    if(hmap_contains(cache->map, data)) { // hit

        // Setting the reference bit to 1
        int* page_idx = hmap_get(cache->map, data);
        cache->cache[*page_idx]->rBit = 1;
        cache->hitCount++;
    } else { // miss
        if(cache->currSize == cache->capacity) {

            // Cache is Full, Move the clock hand till it finds a page with reference bit 0.
            while(cache->cache[cache->currIdx]->rBit != 0) {
                cache->cache[cache->currIdx]->rBit = 0;
                cache->currIdx++;
                cache->currIdx %= cache->capacity;
            }

            // Remove the page from the cache
            hmap_remove(cache->map, cache->cache[cache->currIdx]->data);
            cache->currSize--;
        }

        // Insert the page in the cache, at where the clock hand is pointing and set the reference bit to 1.
        hmap_insert(cache->map, data, copy_of(cache->currIdx));
        cache->cache[cache->currIdx]->data = data;
        cache->cache[cache->currIdx]->rBit = 1;
        cache->currSize++;

        // Move the clock hand
        cache->currIdx++;
        cache->currIdx %= cache->capacity;
        cache->missCount++;
    }
}

void print_clock_node(clock_node* node) {
    if(node == NULL) {
        printf("Node is NULL");
        return;
    }
    printf("(%d, %d)", node->data, node->rBit);
}

// Prints the clock buffer at the current state
void clock_print_buffer(clock_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("[");
    int idx = cache->currIdx % cache->currSize;
    for(int i = 0; i < cache->currSize; i++) {
        print_clock_node(cache->cache[idx]);
        idx++;
        idx %= cache->currSize;
        if(i != cache->currSize-1) {
            printf(",");
        }
    }
    printf("]\n");
}

/*
    Prints the Buffer, Total Reference Count, Hit Count and Miss Count of the cache at that current state.
    Reference count must be atleast one before calling this method.
*/
void clock_analysis(clock_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer = ");
    clock_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    double hitRatio = clock_get_hit_ratio(cache);
    printf("Hit Ratio = %f\n", hitRatio);
}

/*
    Performs clock cache operation for each element, from the array, in a linear fashion
    This method calls the clock_access() method for each page in the array
*/
void clock_put_array(clock_cache* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        clock_access(cache, pages[i]);
    }
}

// Frees the memory occupied by the cache
void clock_destroy(clock_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < cache->currSize; i++) {
        free(cache->cache[i]);
    }
    hmap_free(cache->map);
    free(cache);
}

/*
    Calculates and returns the hit ratio at that current state.
    Returns 0, if the cache is NULL or if there were no references before
*/
double clock_get_hit_ratio(clock_cache* cache) {
    if(cache == NULL || cache->missCount == 0) {
        printf(!cache? "Cache cannot be null\n": "No references have been made.\n");
        return 0;
    }
    int totalReference = cache->hitCount + cache->missCount;
    double hitRatio = (cache->hitCount * 1.0) / (totalReference);
    return hitRatio;
}