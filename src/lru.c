#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "include/lru.h"
#include "hashmap/hashmap.h"
#include "dbllist/dbllist.h"

struct lru_cache_ {
    dbllist* list; 
    hashmap* map;
    int currSize, capacity;
    int hitCount, missCount;
};

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
}

void lru_access(lru_cache* cache, int data) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    if(hmap_contains(cache->map, data)) { // If page in cache
        Node* node = hmap_get(cache->map, data);
        dbllist_move_node_to_tail(cache->list, node);
        cache->hitCount++;
    } else { // Page not in cache
        if(cache->currSize == cache->capacity) {
            int node_val = dbllist_peek_head(cache->list);
            dbllist_remove_head(cache->list);
            hmap_remove(cache->map, node_val);
            cache->currSize--;
        }
        Node* node = node_create(data);
        dbllist_insert_node_at_tail(cache->list, node);
        hmap_insert(cache->map, data, node);
        cache->currSize++;
        cache->missCount++;
    }
}

void lru_print_buffer(lru_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    dbllist_print(cache->list);
}

void lru_analysis(lru_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    printf("Buffer = ");
    lru_print_buffer(cache);
    int totalReference = cache->hitCount + cache->missCount;
    printf("Total References = %d\nHit Count = %d\nMiss Count = %d\n", totalReference, cache->hitCount, cache->missCount);
    float hitRatio = (cache->hitCount * 1.0) / (totalReference);
    printf("Hit Ratio = %f\n", hitRatio);
}

void lru_put_array(lru_cache* cache, int* pages, int size) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        lru_access(cache, pages[i]);
    }
}

void lru_destroy(lru_cache* cache) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    dbllist_destroy(cache->list);
    hmap_free(cache->map);
    free(cache);
}