#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>

#include "hashmap/myhashmap.h"
#include "dbllist/dbllist.h"

#include "arc.h"
#include "utils.h"

struct arc {
    /*
     * T1 Buffer (DBL List + Hashmap)
     * Attempts to keep exactly (p) pages in the cache.
     * Keeps (p) most recent pages in the cache.
    */
    dbllist* t1;
    hashmap* t1Map;

    /*
     * T2 Buffer (DBL List + Hashmap)
     * Attempts to keep exactly (capacity - p) pages in cache.
     * keeps (c - p) most frequemt pages in the cache.
    */
    dbllist* t2;
    hashmap* t2Map;

    /*
     * B1 Buffer (DBL List + Hashmap)
     * This is a Ghost Cache (i.e) stores history of (evicted) pages from the T1 Buffer.
    */
    dbllist* b1;
    hashmap* b1Map;

    /*
     * B2 Buffer (DBL List + Hashmap)
     * This is a Ghost Cache (i.e) stores history of (evicted) pages from the T2 Buffer.
    */
    dbllist* b2;
    hashmap* b2Map;

    // Tunable parameter that adjusts the size of T1 & T2 Buffers
    int p;

    int capacity; // Cache Size
    int hitCount, missCount; // Keeps track of number of hits and misses

    double totalHashmapTime; // Maintains total time taken by hashmap operations
};

/*
 * Initializes the ARC cache
 * If capacity is less than 1. Returns NULL
*/
arc_cache* arc_init(int capacity) {
    if(capacity <= 0) {
        printf("Capacity must be greater than 0\n");
        return NULL;
    }
    arc_cache* cache = malloc(sizeof(arc_cache));
    cache->t1 = dbllist_create();
    cache->t2 = dbllist_create();
    cache->b1 = dbllist_create();
    cache->b2 = dbllist_create();
    cache->t1Map = hmap_create();
    cache->t2Map = hmap_create();
    cache->b1Map = hmap_create();
    cache->b2Map = hmap_create();
    cache->p = 0;
    cache->capacity = capacity;
    cache->hitCount = 0;
    cache->missCount = 0;
    cache->totalHashmapTime = 0;
    return cache;
}

// Returns the maximum of 2 integers
int max(int a, int b) {
    return (a > b? a: b);
}

// Returns the minimum of 2 integers
int min(int a, int b) {
    return (a < b? a: b);
}

// Replace method as decribed in the research paper.
void perform_replace(arc_cache* cache, int page) {
    int t1Size = dbllist_size(cache->t1);
    clock_t start = clock();
    if(t1Size > 0 && (t1Size > cache->p || (hmap_contains(cache->b2Map, page) && t1Size == cache->p))) {
        cache->totalHashmapTime += end_clock_time(start);
        // Move the Least Recently Used (head) of T1 to Most Recently Used (head) of B1.
        int headVal = util_peek_head_value_and_remove(cache->t1, cache->t1Map);
        cache->totalHashmapTime += util_insert_node_at_tail_and_map(headVal, cache->b1, cache->b1Map);
    } 
    else {
        cache->totalHashmapTime += end_clock_time(start);
        // Move the Least Recently Used (head) of T2 to Most Recently Used (head) of B2.
        int headVal = util_peek_head_value_and_remove(cache->t2, cache->t2Map);
        cache->totalHashmapTime += util_insert_node_at_tail_and_map(headVal, cache->b2, cache->b2Map);
    }
}

/*
 * Performs ARC Cache operation on the cache with the given page.
 * This algorithm contains Four Lists: T1, T2, B1, B2
 * The lists T1 & T2 are considered as cache. While, B1 & B2 (Ghost Caches) stores the history of (evicted) pages.
 * T1 & B1 Lists are together known as L1 and T2 & B2 Lists are together known as L2.
 * The T1 Buffer favours Recency and the T2 Buffer favours Frequency.
 * ARC specifies a Tunable Parameter, p, which defines the size of Buffers T1 & T2.
 * The Tunable Parameter, p, is updated only when there is a hit in the Ghost Caches (B1 & B2 Buffers).
 * When there is a hit in B1, The p value is increased (increment by 1, if B1 >= B2, else increment by a factor of B2 (i.e) B2/B1)
 * Wnen there is a hit in B2, The p value is decreased (decrement by 1, if B2 >= B1, else decrement by a factor of B1 (i.e) B1/B2)
 * If the p value is greater, it allows more recent pages into the cache.
 * If the p value is smaller, it allows more frequent pages into the cache.
*/
void arc_access(arc_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    clock_t start = clock();
    if(hmap_contains(cache->t1Map, page)) { // Page in T1 (hit)
        cache->totalHashmapTime += end_clock_time(start);
        // Promote the page to T2 List.
        cache->totalHashmapTime += util_remove_from_list_and_map(page, cache->t1, cache->t1Map);
        cache->totalHashmapTime += util_insert_node_at_tail_and_map(page, cache->t2, cache->t2Map);
        cache->hitCount++;
    } 
    else if(hmap_contains(cache->t2Map, page)) { // Page in T2 (hit)
        cache->totalHashmapTime += end_clock_time(start);
        // Move the page to the Most Recently Used (tail) position
        Node* node = hmap_get(cache->t2Map, page);
        dbllist_move_node_to_tail(cache->t2, node);
        cache->hitCount++;
    } 
    else if(hmap_contains(cache->b1Map, page)) { // Page in B1 (miss)
        cache->totalHashmapTime += end_clock_time(start);
        // Update P Value
        int b1Size = dbllist_size(cache->b1), b2Size = dbllist_size(cache->b2);
        int delta = 1;
        if(b2Size > b1Size) {
            delta = (b2Size / b1Size);
        }
        cache->p = min(cache->p + delta, cache->capacity);

        perform_replace(cache, page);

        // Move the Page from B1 List to Most Recently Used (tail) Position of the T2 List.
        cache->totalHashmapTime += util_remove_from_list_and_map(page, cache->b1, cache->b1Map);
        cache->totalHashmapTime += util_insert_node_at_tail_and_map(page, cache->t2, cache->t2Map);
        cache->missCount++;
    } 
    else if(hmap_contains(cache->b2Map, page)) { // Page in B2 (miss)
        cache->totalHashmapTime += end_clock_time(start);
        // Update P Value
        int b1Size = dbllist_size(cache->b1), b2Size = dbllist_size(cache->b2);
        int delta = 1;
        if(b1Size > b2Size) {
            delta = (b1Size / b2Size);
        }
        cache->p = max(cache->p - delta, 0);

        perform_replace(cache, page);

        // Move the Page from B2 List to Most Recently Used (tail) Position of the T2 List.
        cache->totalHashmapTime += util_remove_from_list_and_map(page, cache->b2, cache->b2Map);
        cache->totalHashmapTime += util_insert_node_at_tail_and_map(page, cache->t2, cache->t2Map);
        cache->missCount++;
    } 
    else { // Page not in any List (miss)
        cache->totalHashmapTime += end_clock_time(start);
        int t1Size = dbllist_size(cache->t1);
        int t2Size = dbllist_size(cache->t2);
        int b1Size = dbllist_size(cache->b1);
        int b2Size = dbllist_size(cache->b2);
        int totalCount = t1Size + t2Size + b1Size + b2Size;

        if(t1Size + b1Size == cache->capacity) {
            // L1 List is Full
            if(t1Size < cache->capacity) {

                // Remove Least Recently Used (head) page from B1 List.
                util_peek_head_value_and_remove(cache->b1, cache->b1Map);
                perform_replace(cache, page);
            } 
            else {

                // B1 is Empty, So remove Least Recently Used (head) page from T1 List.
                util_peek_head_value_and_remove(cache->t1, cache->t1Map);
            }
        } 
        else {

            // L1 List not Full
            if(totalCount >= cache->capacity) {
                if(totalCount == 2 * cache->capacity) {

                    // Total cache is Full, so remove the Least Recently Used (head) page from B2 List.
                    util_peek_head_value_and_remove(cache->b2, cache->b2Map);
                }
                perform_replace(cache, page);
            }
        }

        // Insert the page at the Most Recently Used (tail) position of the T1 List.
        cache->totalHashmapTime += util_insert_node_at_tail_and_map(page, cache->t1, cache->t1Map);
        cache->missCount++;
    }
}

// Prints the ARC buffer at that current state
void arc_print_buffer(arc_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("T1 => ");
    dbllist_print(cache->t1);
    printf("T2 => ");
    dbllist_print(cache->t2);
    printf("B1 => ");
    dbllist_print(cache->b1);
    printf("B2 => ");
    dbllist_print(cache->b2);
    printf("P = %d\n", cache->p);
}

/*
 * Prints the Buffer, Total Reference Count, Hit Count and Miss Count of the cache at that current state.
 * Reference count must be atleast one before calling this method.
*/
void arc_analysis(arc_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer: \n");
    arc_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    double hitRatio = arc_get_hit_ratio(cache);
    printf("Hit Ratio = %f\n", hitRatio);
}

/*
 * Performs ARC cache operation for each element, from the array, in a linear fashion
 * This method calls the arc_access() method for each page in the array
*/
void arc_put_array(arc_cache* cache, int pages[], int size) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        arc_access(cache, pages[i]);
    }
}

// Frees the memory occupied by the cache
void arc_destroy(arc_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    dbllist_destroy(cache->t1);
    dbllist_destroy(cache->t2);
    dbllist_destroy(cache->b1);
    dbllist_destroy(cache->b2);
    hmap_free(cache->t1Map);
    hmap_free(cache->t2Map);
    hmap_free(cache->b1Map);
    hmap_free(cache->b2Map);
    free(cache);
}

/*
 * Calculates and returns the hit ratio at that current state.
 * Returns 0, if the cache is NULL or if there were no references before
*/
double arc_get_hit_ratio(arc_cache* cache) {
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
double arc_get_hashmap_time(arc_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be NULL\n");
        return 0;
    }
    return cache->totalHashmapTime;
}