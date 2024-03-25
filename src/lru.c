#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>

#include "hashmap/myhashmap.h"
#include "dbllist/dbllist.h"

#include "lru.h"
#include "utils.h"

struct lru_cache_ {

    // Cache
    dbllist* list; 
    hashmap* map;
    
    int currSize; // Current Size of the Cache
    int capacity; // Total Cache Size
    int hitCount, missCount; // Keeps track of number of hits and misses

    double totalHashmapTime;
};

/*
 * Initializes the LRU cache
 * If capacity is less than 1. Returns NULL
*/
lru_cache* lru_init(int capacity) {
    if(capacity <= 0) {
        printf("Capacity must be greater than 0\n");
        return NULL;
    }
    lru_cache* cache = (lru_cache*) malloc(sizeof(lru_cache));
    cache->list = dbllist_create();
    cache->map = hmap_create();
    cache->currSize = 0;
    cache->capacity = capacity;
    cache->hitCount = 0;
    cache->missCount = 0;
    cache->totalHashmapTime = 0;
    return cache;
}

/*
 * Performs LRU cache operation on the cache with the given page
 * If the requested page is not available in the cache, the LRU algorithm adds the page at the Most Recently Used (tail) position in the cache.
 * If the requested page is available in the cache, then the page is moved to the Most Recently Used (tail) position in the cache.
 * When cache size is full, the Least Recently Used (head) page is removed (evicted) from the cache.
*/
void lru_access(lru_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }

    clock_t start = clock();
    if(hmap_contains(cache->map, page)) { // page in cache (hit)
        cache->totalHashmapTime += end_clock_time(start);
        // Move the page to Most Recently Used (tail) Position
        start = clock();
        Node* node = hmap_get(cache->map, page);
        cache->totalHashmapTime += end_clock_time(start);
        
        dbllist_move_node_to_tail(cache->list, node);
        cache->hitCount++;
    } 
    else { // Page not in cache
        cache->totalHashmapTime += end_clock_time(start);
        if(cache->currSize == cache->capacity) {

            // Cache is Full, Remove the Least Recently Used (head) page from the cache.
            util_peek_head_value_and_remove(cache->list, cache->map);
            cache->currSize--;
        }
        // Insert the page at the Most Recently Used (tail) position
        cache->totalHashmapTime += util_insert_node_at_tail_and_map(page, cache->list, cache->map);
        cache->currSize++;
        cache->missCount++;
    }
}

// Prints the LRU buffer at that current state.
void lru_print_buffer(lru_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    dbllist_print(cache->list);
}

/*
 * Prints the Buffer, Total Reference Count, Hit Count and Miss Count of the cache at that current state.
 * Reference count must be atleast one before calling this method.
*/
void lru_analysis(lru_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer = ");
    lru_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    double hitRatio = lru_get_hit_ratio(cache);
    printf("Hit Ratio = %f\n", hitRatio);
}

/*
 * Performs LRU cache operation for each element, from the array, in a linear fashion
 * This method calls the lru_access() method for each page in the array
*/
void lru_put_array(lru_cache* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        lru_access(cache, pages[i]);
    }
}

// Frees the memory occupied by the cache
void lru_destroy(lru_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    dbllist_destroy(cache->list);
    hmap_free(cache->map);
    free(cache);
}

/*
 * Calculates and returns the hit ratio at that current state.
 * Returns 0, if the cache is NULL or if there were no references before
*/
double lru_get_hit_ratio(lru_cache* cache) {
    if(cache == NULL || cache->missCount == 0) {
        printf(!cache? "Cache cannot be null\n": "No references have been made.\n");
        return 0;
    }
    int totalReference = cache->hitCount + cache->missCount;
    double hitRatio = (cache->hitCount * 1.0) / (totalReference);
    return hitRatio;
}

/*
 * This method returns the total time taken by hashmap during the lifetime of the cache.
 * Returns the total time in seconds.
 * Returns 0, if the cache is NULL
*/
double lru_get_hashmap_time(lru_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return 0;
    }
    return cache->totalHashmapTime;
}