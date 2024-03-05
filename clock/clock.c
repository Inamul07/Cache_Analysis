#include "clock.h"
#include <stdbool.h>
#include "../header_files/hashmap.h"
#include <stdlib.h>
#include<stdio.h>

// CODE_REVIEW: single typedef is enough
typedef struct clock_node_ {
    int data;
    // CODE_REVIEW: give a proper name
    bool r;
} clock_node;

// CODE_REVIEW: single typedef is enough
typedef struct clock_cache_ {
    // CODE_REVIEW: array should be ** pointer
    clock_node* cache;
    struct hashmap* map;
    int currIdx, currSize;
    int capacity;
    int hitCount, missCount;    
} clock_cache;

clock_cache* clock_init(int capacity) {
    clock_cache* cache = (clock_cache*) malloc(sizeof(clock_cache));
    cache->cache = (clock_node*) malloc(capacity * sizeof(clock_node));
    cache->map = hmap_create();
    cache->currIdx = 0;
    cache->currSize = 0;
    cache->capacity = capacity;
    cache->hitCount = 0;
    cache->missCount = 0;
    return cache;
}

void clock_access(clock_cache* cache, int data) {
    if(hmap_contains(cache->map, data)) {
        // hit
        int page_idx = hmap_get(cache->map, data);
        cache->cache[page_idx].r = 1;
        cache->hitCount++;
    } else {
        // miss
        if(cache->currSize == cache->capacity) {
            while(cache->cache[cache->currIdx].r != 0) {
                cache->cache[cache->currIdx].r = 0;
                cache->currIdx++;
                cache->currIdx %= cache->capacity;
            }
            hmap_remove(cache->map, cache->cache[cache->currIdx].data);
        } else {
            cache->currSize++;
        }
        //CODE_REVIEW: hmap_insert expects void* but you are passing int
        hmap_insert(cache->map, data, cache->currIdx);
        cache->cache[cache->currIdx].data = data;
        cache->cache[cache->currIdx].r = 1;
        cache->currIdx++;
        cache->currIdx %= cache->capacity;
        cache->missCount++;
    }
}

void print_clock_node(clock_node node) {
    printf("(%d, %d)", node.data, node.r);
}

// CODE_REVIEW: print from data pointed by clock hand.
void clock_print_buffer(clock_cache* cache) {
    printf("[");
    for(int i = 0; i < cache->currSize; i++) {
        print_clock_node(cache->cache[i]);
        if(i != cache->currSize-1) {
            printf(",");
        }
    }
    printf("]\n");
}

void clock_analysis(clock_cache* cache) {
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    float hitRatio = (cache->hitCount * 1.0) / (totalReference);
    printf("Hit Ratio = %f\n", hitRatio);
}

void clock_put_array(clock_cache* cache, int* pages, int size) {
    for(int i = 0; i < size; i++) {
        clock_access(cache, pages[i]);
    }
}

void clock_destroy(clock_cache* cache) {
    free(cache->cache);
    hmap_free(cache->map);
    free(cache);
}