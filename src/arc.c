#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "arc.h"
#include "utils.h"

#include "hashmap/myhashmap.h"
#include "dbllist/dbllist.h"

// CODE_REVIEW: write comments and name funcions properly

struct arc {
    dbllist* t1;
    dbllist* t2;
    dbllist* b1;
    dbllist* b2;
    hashmap* t1Map;
    hashmap* t2Map;
    hashmap* b1Map;
    hashmap* b2Map;
    int p;
    int capacity;
    int hitCount, missCount;
};

// Initializes the ARC cache
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
}

int max(int a, int b) {
    return (a > b? a: b);
}

int min(int a, int b) {
    return (a < b? a: b);
}

void perform_replace(arc_cache* cache, int page) {
    int t1Size = dbllist_size(cache->t1);
    if(t1Size > 0 && (t1Size > cache->p || (hmap_contains(cache->b2Map, page) && t1Size == cache->p))) {
        int headVal = util_peek_head_value_and_remove(cache->t1, cache->t1Map);

        util_insert_node_at_tail_and_map(headVal, cache->b1, cache->b1Map);
    } else {
        int headVal = util_peek_head_value_and_remove(cache->t2, cache->t2Map);

        util_insert_node_at_tail_and_map(headVal, cache->b2, cache->b2Map);
    }
}

// Performs ARC Cache operation on the cache with the given page
void arc_access(arc_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    if(hmap_contains(cache->t1Map, page)) { // Page in T1 (hit)
        util_remove_from_list_and_map(page, cache->t1, cache->t1Map);
        
        util_insert_node_at_tail_and_map(page, cache->t2, cache->t2Map);

        cache->hitCount++;
    } else if(hmap_contains(cache->t2Map, page)) { // Page in T2 (hit)
        Node* node = hmap_get(cache->t2Map, page);
        dbllist_move_node_to_tail(cache->t2, node);

        cache->hitCount++;
    } else if(hmap_contains(cache->b1Map, page)) { // Page in B1 (miss)
        int b1Size = dbllist_size(cache->b1), b2Size = dbllist_size(cache->b2);
        int delta = 1;
        if(b2Size > b1Size) {
            delta = (b2Size / b1Size);
        }
        cache->p = min(cache->p + delta, cache->capacity);

        perform_replace(cache, page);

        util_remove_from_list_and_map(page, cache->b1, cache->b1Map);

        util_insert_node_at_tail_and_map(page, cache->t2, cache->t2Map);

        cache->missCount++;
    } else if(hmap_contains(cache->b2Map, page)) { // Page in B2 (miss)
        int b1Size = dbllist_size(cache->b1), b2Size = dbllist_size(cache->b2);
        int delta = 1;
        if(b1Size > b2Size) {
            delta = (b1Size / b2Size);
        }
        cache->p = max(cache->p - delta, 0);

        perform_replace(cache, page);

        util_remove_from_list_and_map(page, cache->b2, cache->b2Map);

        util_insert_node_at_tail_and_map(page, cache->t2, cache->t2Map);

        cache->missCount++;
    } else { // Page not in Cache (miss)
        int t1Size = dbllist_size(cache->t1);
        int t2Size = dbllist_size(cache->t2);
        int b1Size = dbllist_size(cache->b1);
        int b2Size = dbllist_size(cache->b2);
        int totalCount = t1Size + t2Size + b1Size + b2Size;
        if(t1Size + b1Size == cache->capacity) {
            if(t1Size < cache->capacity) {
                util_peek_head_value_and_remove(cache->b1, cache->b1Map);
                perform_replace(cache, page);
            } else {
                util_peek_head_value_and_remove(cache->t1, cache->t1Map);
            }
        } else {
            if(totalCount >= cache->capacity) {
                if(totalCount == 2 * cache->capacity) {
                    util_peek_head_value_and_remove(cache->b2, cache->b2Map);
                }
                perform_replace(cache, page);
            }
        }
        util_insert_node_at_tail_and_map(page, cache->t1, cache->t1Map);

        cache->missCount++;
    }
}

// Prints the ARC buffer at the current state
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

// Prints the reference, hit and miss count
void arc_analysis(arc_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer: \n");
    arc_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    float hitRatio = (cache->hitCount * 1.0) / (totalReference);
    printf("Hit Ratio = %f\n", hitRatio);
}

// Performs ARC cache operation for each element, from the array, in a linear fashion
void arc_put_array(arc_cache* cache, int* pages, int size) {
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
