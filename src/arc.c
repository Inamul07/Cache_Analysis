#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "arc.h"
#include "hashmap/myhashmap.h"
#include "dbllist/dbllist.h"

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

int peek_val_and_remove_head(dbllist* list, hashmap* map) {
    int headVal = dbllist_peek_head_val(list);
    dbllist_remove_head(list);
    hmap_remove(map, headVal);
    return headVal;
}

void insert_value_at_tail(int data, dbllist* list, hashmap* map) {
    Node* node = node_create(data);
    dbllist_insert_node_at_tail(list, node);
    hmap_insert(map, data, node);
}

void perform_replace(arc_cache* cache, int page) {
    int t1Size = dbllist_size(cache->t1);
    if(t1Size > 0 && (t1Size > cache->p || (hmap_contains(cache->b2Map, page) && t1Size == cache->p))) {
        int headVal = peek_val_and_remove_head(cache->t1, cache->t1Map);

        insert_value_at_tail(headVal, cache->b1, cache->b1Map);
    } else {
        int headVal = peek_val_and_remove_head(cache->t2, cache->t2Map);

        insert_value_at_tail(headVal, cache->b2, cache->b2Map);
    }
}

void remove_node(int data, dbllist* list, hashmap* map) {
    Node* node = hmap_get(map, data);
    dbllist_remove_node(list, node);
    hmap_remove(map, data);
}

void arc_access(arc_cache* cache, int page) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    if(hmap_contains(cache->t1Map, page)) { // Page in T1 (hit)
        remove_node(page, cache->t1, cache->t1Map);

        insert_value_at_tail(page, cache->t2, cache->t2Map);

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

        remove_node(page, cache->b1, cache->b1Map);

        insert_value_at_tail(page, cache->t2, cache->t2Map);

        cache->missCount++;
    } else if(hmap_contains(cache->b2Map, page)) { // Page in B2 (miss)
        int b1Size = dbllist_size(cache->b1), b2Size = dbllist_size(cache->b2);
        int delta = 1;
        if(b1Size > b2Size) {
            delta = (b1Size / b2Size);
        }
        cache->p = max(cache->p - delta, 0);

        perform_replace(cache, page);

        remove_node(page, cache->b2, cache->b2Map);

        insert_value_at_tail(page, cache->t2, cache->t2Map);

        cache->missCount++;
    } else { // Page not in Cache (miss)
        int t1Size = dbllist_size(cache->t1), t2Size = dbllist_size(cache->t2), b1Size = dbllist_size(cache->b1), b2Size = dbllist_size(cache->b2);
        if(t1Size + b1Size == cache->capacity) {
            if(t1Size < cache->capacity) {
                peek_val_and_remove_head(cache->b1, cache->b1Map);
                perform_replace(cache, page);
            } else {
                peek_val_and_remove_head(cache->t1, cache->t1Map);
            }
        } else {
            if(t1Size + t2Size + b1Size + b2Size >= cache->capacity) {
                if(t1Size + t2Size + b1Size + b2Size == 2 * cache->capacity) {
                    peek_val_and_remove_head(cache->b2, cache->b2Map);
                }
                perform_replace(cache, page);
            }
        }
        insert_value_at_tail(page, cache->t1, cache->t1Map);

        cache->missCount++;
    }
}

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

void arc_put_array(arc_cache* cache, int* pages, int size) {
    if(cache == NULL) {
        printf("Cache cannot be null\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        arc_access(cache, pages[i]);
    }
}

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
