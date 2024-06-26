#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashmap/myhashmap.h"
#include "dbllist/dbllist.h"

#include "utils.h"
#include "two_queue.h"

struct two_queue {
    // Am Buffer (LRU List)
    dbllist* am;
    hashmap* amMap;

    // A1In Buffer (FIFO List)
    dbllist* a1in;
    hashmap* a1inMap;

    // A1Out Buffer (FIFO List)
    dbllist* a1out;
    hashmap* a1outMap;

    int amSize, a1inSize, a1outSize; // Total Sizes of the buffers
    int hitCount, missCount; // Keeps track of number of hits and misses
};

/*
 * Creates and Initializes the Two Queue Cache
 * If capacity is less than 1. Returns NULL
*/
two_queue_cache* two_queue_init(int capacity) {
    if(capacity <= 0) {
        printf("Capacity must be greater than 0\n");
        return NULL;
    }
    int amSize = (capacity/ 2) + (capacity%2 == 0? 0: 1);
    int a1inSize = (capacity/ 2);
    int a1outSize = amSize;
    two_queue_cache* cache = malloc(sizeof(two_queue_cache));
    cache->am = dbllist_create();
    cache->a1in = dbllist_create();
    cache->a1out = dbllist_create();
    cache->amMap = hmap_create();
    cache->a1inMap = hmap_create();
    cache->a1outMap = hmap_create();
    cache->amSize = amSize;
    cache->a1inSize = a1inSize;
    cache->a1outSize = a1outSize;
    cache->hitCount = 0;
    cache->missCount = 0;
    return cache;
}

/*
 * This algorithm consists of 3 lists. Am (LRU List), A1In (FIFO List), A1Out (FIFO List)
 * Only Am & A1In are considered as caches, while A1Out is a History Buffer.
 * If page is in Am, the page is most to the Most Recently Used (tail) position.
 * If the page is in A1In, we do nothing. This is to avoid pages, with correlated refernce, into the Am List.
 * If the page is in A1Out (it is a miss), the page gets promoted to Am.
 * If a page is not in the cache, it is inserted to A1In.
*/
void two_queue_access(two_queue_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }

    if(hmap_contains(cache->amMap, page)) { // page in Am (hit)
        // The page is moved to the Most Recently Used (tail) Position.
        Node* node = hmap_get(cache->amMap, page);
        dbllist_move_node_to_tail(cache->am, node);
        cache->hitCount++;
    } 
    else if(hmap_contains(cache->a1inMap, page)) { // page in A1In (hit)
        cache->hitCount++;
    } 
    else if(hmap_contains(cache->a1outMap, page)) { // page in A1Out (miss)
        // Remove the page from A1Out
        util_remove_from_list_and_map(page, cache->a1out, cache->a1outMap);

        int amCurrSize = dbllist_size(cache->am);
        if(amCurrSize == cache->amSize) {

            // Am Size is full, So remove the Least Recently Used (head) page from Am.
            util_peek_head_value_and_remove(cache->am, cache->amMap);
        }

        // Insert the page to Most Recently Used (tail) position of Am.
        util_insert_node_at_tail_and_map(page, cache->am, cache->amMap);
        cache->missCount++;
    } else { // Page not in Cache (miss)
        int a1inCurrSize = dbllist_size(cache->a1in);
        if(a1inCurrSize == cache->a1inSize) {

            // A1In Size is full, So move the first page (head) in A1In to A1Out (tail).
            int headVal = util_peek_head_value_and_remove(cache->a1in, cache->a1inMap);

            int a1outCurrSize = dbllist_size(cache->a1out);
            if(a1outCurrSize == cache->a1outSize) {

                // A1Out is Full, So remove the first page (head) from A1Out.
                util_peek_head_value_and_remove(cache->a1out, cache->a1outMap);
            }

            util_insert_node_at_tail_and_map(headVal, cache->a1out, cache->a1outMap);
        }

        // Insert the page at the end (tail) of A1In.
        util_insert_node_at_tail_and_map(page, cache->a1in, cache->a1inMap);
        cache->missCount++;
    }
}

/*
 * Performs Two Queue cache operation for each element, from the array, in a linear fashion
 * This method calls the two_queue_access() method for each page in the array
*/
void two_queue_put_array(two_queue_cache* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        two_queue_access(cache, pages[i]);
    }
}

// Prints the Two Queue buffer at that current state
void two_queue_print_buffer(two_queue_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Am => ");
    dbllist_print(cache->am);
    printf("A1In => ");
    dbllist_print(cache->a1in);
    printf("A1Out => ");
    dbllist_print(cache->a1out);
}

/*
 * Prints the Buffer, Total Reference Count, Hit Count and Miss Count of the cache at that current state.
 * Reference count must be atleast one before calling this method.
*/
void two_queue_analysis(two_queue_cache* cache) {
    if(cache == NULL ) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer: \n");
    two_queue_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    double hitRatio = two_queue_get_hit_ratio(cache);
    printf("Hit Ratio = %f\n", hitRatio);
}

// Frees the memory occupied by the cache
void two_queue_destroy(two_queue_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    dbllist_destroy(cache->am);
    dbllist_destroy(cache->a1in);
    dbllist_destroy(cache->a1out);
    hmap_free(cache->amMap);
    hmap_free(cache->a1inMap);
    hmap_free(cache->a1outMap);
    free(cache);
}

/*
 * Calculates and returns the hit ratio at that current state.
 * Returns 0, if the cache is NULL or if there were no references before
*/
double two_queue_get_hit_ratio(two_queue_cache* cache) {
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
double two_queue_get_hashmap_time(two_queue_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return 0;
    }
    double totalTime = 0;
    totalTime += hmap_get_time_taken(cache->amMap);
    totalTime += hmap_get_time_taken(cache->a1inMap);
    totalTime += hmap_get_time_taken(cache->a1outMap);
    return totalTime;
}