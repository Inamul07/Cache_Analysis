#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hashmap/myhashmap.h"
#include "clock.h"

struct clock_node_ {
    int data;
    bool r_bit;
};

struct clock_cache_ {
    clock_node** cache;
    struct hashmap* map;
    int currIdx, currSize;
    int capacity;
    int hitCount, missCount;    
};

// Initializes the clock cache
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

// Performs clock cache operation on the cache with the given page
void clock_access(clock_cache* cache, int data) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    if(hmap_contains(cache->map, data)) {
        // hit
        int* page_idx = hmap_get(cache->map, data);
        cache->cache[*page_idx]->r_bit = 1;
        cache->hitCount++;
    } else {
        // miss
        if(cache->currSize == cache->capacity) {
            while(cache->cache[cache->currIdx]->r_bit != 0) {
                cache->cache[cache->currIdx]->r_bit = 0;
                cache->currIdx++;
                cache->currIdx %= cache->capacity;
            }
            hmap_remove(cache->map, cache->cache[cache->currIdx]->data);
            cache->currSize--;
        }
        hmap_insert(cache->map, data, copy_of(cache->currIdx));
        cache->cache[cache->currIdx]->data = data;
        cache->cache[cache->currIdx]->r_bit = 1;
        cache->currSize++;
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
    printf("(%d, %d)", node->data, node->r_bit);
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

// Prints the reference, hit and miss count
void clock_analysis(clock_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer = ");
    clock_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    double hitRatio = (cache->hitCount * 1.0) / (totalReference);
    printf("Hit Ratio = %f\n", hitRatio);
}

// Performs clock cache operation for each element, from the array, in a linear fashion
void clock_put_array(clock_cache* cache, int* pages, int size) {
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

double clock_get_hit_ratio(clock_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return -1.0;
    }
    int totalReference = cache->hitCount + cache->missCount;
    double hitRatio = (cache->hitCount * 1.0) / (totalReference);
    return hitRatio;
}