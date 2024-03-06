#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "lru.h"
// CODE_REVIEW: change includes in this pattern. dbl/dbllist.h & hashmap/hashmap.h
#include "../header_files/dbllist.h"
#include "../header_files/hashmap.h"

// CODE_REVIEW:  handle null arguments

struct lru_cache_ {
    dbllist* list;
    // CODE_REVIEW: add a typedef   
    struct hashmap* map;
    int currSize, capacity;
    int hitCount, missCount;
};

lru_cache* lru_init(int capacity) {
    lru_cache* cache = (lru_cache*) malloc(sizeof(lru_cache));
    cache->list = dbllist_create();
    cache->map = hmap_create();
    cache->currSize = 0;
    cache->capacity = capacity;
    cache->hitCount = 0;
    cache->missCount = 0;
}

void lru_access(lru_cache* cache, int data) {
    if(hmap_contains(cache->map, data)) { // If page in cache
        Node* node = hmap_get(cache->map, data);
        dbllist_remove_node(cache->list, node);
        dbllist_insert_node(cache->list, node);
        cache->hitCount++;
    } else { // Page not in cache
        if(cache->currSize == cache->capacity) {
            Node* head = dbllist_remove_head(cache->list);
            hmap_remove(cache->map, node_val(head));
            free(head);
            cache->currSize--;
        }
        Node* node = node_create(data);
        dbllist_insert_node(cache->list, node);
        hmap_insert(cache->map, data, node);
        cache->currSize++;
        cache->missCount++;
    }
}

void lru_print_buffer(lru_cache* cache) {
    dbllist_print(cache->list);
}

void lru_analysis(lru_cache* cache) {
    printf("Buffer = ");
    lru_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    float hitRatio = (cache->hitCount * 1.0) / (totalReference);
    printf("Hit Ratio = %f\n", hitRatio);
}

void lru_put_array(lru_cache* cache, int* pages, int size) {
    for(int i = 0; i < size; i++) {
        lru_access(cache, pages[i]);
    }
}

void lru_destroy(lru_cache* cache) {
    dbllist_destroy(cache->list);
    hmap_free(cache->map);
    free(cache);
}